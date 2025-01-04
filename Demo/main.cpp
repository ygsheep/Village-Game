#define SDL_MAIN_HANDLED  // ��֪ SDL ����Ҫ�滻��׼�� main ��������������ʹ���Զ���� main ������

#include <iostream>       // ��׼ C++ ����������⣬���ڴ�ӡ������Ϣ�ȡ�
#include <string>
#include <fstream>
#include <sstream>
#include "cJSON.h"

#include <SDL.h>          // SDL ���Ŀ⣬�ṩ���ڡ��¼���ͼ�εȹ��ܡ�
#include <SDL_image.h>    // SDL_image ��չ�⣬���ڼ��غʹ���ͼ���� JPG��PNG����
#include <SDL_mixer.h>    // SDL_mixer ��չ�⣬���ڼ��غͲ�����Ƶ���� MP3����
#include <SDL_ttf.h>      // SDL_ttf ��չ�⣬���ڼ��غ���Ⱦ���塣
#include <SDL2_gfxPrimitives.h>


void test_json() {
	std::fstream file("test_json.json");
	if (not file.good())
		return;

	std::stringstream result;
	result << file.rdbuf();

	cJSON* json_root = cJSON_Parse(result.str().c_str());

	cJSON* name = cJSON_GetObjectItem(json_root, "name");
	cJSON* monery = cJSON_GetObjectItem(json_root, "monery");

	std::cout << name->string << " : " << name->valuestring << std::endl;
	std::cout << monery->string << " : " << monery->valueint << std::endl;

}



int main()
{
	test_json();
	// ��ʼ�� SDL ���Ŀ⣬���� SDL_INIT_EVERYTHING ��ʾ��ʼ��������ϵͳ����Ƶ����Ƶ����ʱ���ȣ���
	SDL_Init(SDL_INIT_EVERYTHING);

	// ��ʼ�� SDL_image �⣬������ʾ֧�� JPG �� PNG ��ʽ��ͼƬ���ء�
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	// ��ʼ�� SDL_mixer �⣬������ʾ֧�� MP3 ��ʽ����Ƶ��
	Mix_Init(MIX_INIT_MP3);

	// ��ʼ�� SDL_ttf �⣬����������Ⱦ��
	TTF_Init();

	// ��ʼ����Ƶ�豸��������Ϊ 44100Hz����ʽΪĬ�ϣ�ͨ����Ϊ 2��������������������СΪ 2048 �ֽڡ�
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// ����һ�����ڣ�����Ϊ�����ڡ������Ϊ 1280���߶�Ϊ 720����ʾ����Ļ���ģ�����ģʽΪ��ʾ��SDL_WINDOW_SHOWN����
	SDL_Window* window = SDL_CreateWindow(
		u8"����",                // ���ڱ��⣬֧�� UTF-8 ����
		SDL_WINDOWPOS_CENTERED,  // ����ˮƽλ�ã���Ļ����
		SDL_WINDOWPOS_CENTERED,  // ���ڴ�ֱλ�ã���Ļ����
		1280, 720,               // ���ڴ�С��1280x720
		SDL_WINDOW_SHOWN         // ���ڱ�־����ʾ����
	);

	// ����һ�� SDL ��Ⱦ���������ڴ����л���ͼ�Ρ�
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,                  // ָ���Ѿ������� SDL_Window ��ָ�룬��Ⱦ���Ὣͼ����Ƶ��ô����С�
		-1,                      // ʹ��Ĭ�ϵ�ͼ��������������Ϊ -1 ��ʾ�� SDL �Զ�ѡ����ѵ��������򣩡�
		SDL_RENDERER_ACCELERATED // ��Ⱦ����־��ʹ��Ӳ�����١��������ͼ�����ܣ����Ӳ��֧�֣���
	);

	bool is_open = false;

	SDL_Event event;
	int fps = 60;

	SDL_Surface* sur_face = IMG_Load("icon.jpeg");
	SDL_Texture* text_ure = SDL_CreateTextureFromSurface(renderer, sur_face);

	TTF_Font* font = TTF_OpenFont(u8"ipix.ttf", 18);
	SDL_Color font_color = { 255, 0, 0, 255 };

	SDL_Surface* sur_text = TTF_RenderUTF8_Blended(font, u8"�ı�ը��", font_color);
	SDL_Texture* font_ure = SDL_CreateTextureFromSurface(renderer, sur_text);

	Mix_Music* music = Mix_LoadMUS(u8"music.mp3");
	Mix_FadeInMusic(music, 1, 1500);

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_fred = SDL_GetPerformanceFrequency();


	SDL_Point pos_cursor = { 0, 0 };
	SDL_Rect rect_img, rect_text;
	rect_img.w = sur_face->w / 2;
	rect_img.h = sur_face->h / 2;

	rect_text.w = sur_text->w;
	rect_text.h = sur_text->h;


	while (!is_open)
	{

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_open = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter);
		current_counter = last_counter;
		if (delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));


		// ��������
		rect_img.x = pos_cursor.x - rect_img.w / 2;
		rect_img.y = pos_cursor.y - rect_img.h / 2;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		// ��Ⱦ��ͼ
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, text_ure, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 20, 0, 255, 0, 150);

		SDL_RenderCopy(renderer, font_ure, nullptr, &rect_text);

		SDL_RenderPresent(renderer);

	}






	// ���� SDL �����Դ�������ڴ�й©����Դռ�á�

	// ���ٴ�����Դ���ͷ��ڴ档
	SDL_DestroyWindow(window);

	// �ر� SDL_ttf �⣬�ͷ�������Ⱦ�����Դ��
	TTF_Quit();

	// �ر� SDL_mixer �⣬�ͷ���Ƶ�����Դ��
	Mix_Quit();

	// �ر� SDL_image �⣬�ͷ�ͼ����������Դ��
	IMG_Quit();

	// �ر� SDL ���Ŀ⣬�ͷ����� SDL ��ʼ����ϵͳ��Դ��
	SDL_Quit();

	// ���� 0 ��ʾ���������˳���
	return 0;
}
