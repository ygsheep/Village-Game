#ifndef __GAMEMANAGER__
#define __GAMEMANAGER__

#include "singleton.h"
#include <SDL.h>          // SDL ���Ŀ⣬�ṩ���ڡ��¼���ͼ�εȹ��ܡ�
#include <SDL_image.h>    // SDL_image ��չ�⣬���ڼ��غʹ���ͼ���� JPG��PNG����
#include <SDL_mixer.h>    // SDL_mixer ��չ�⣬���ڼ��غͲ�����Ƶ���� MP3����
#include <SDL_ttf.h>      // SDL_ttf ��չ�⣬���ڼ��غ���Ⱦ���塣
#include <SDL2_gfxPrimitives.h>

class GameManager:public Manager<GameManager>
{
	friend class Manager<GameManager>;

private:
	int fps      = 60;
	bool is_quit = false;

	SDL_Event     event;
	SDL_Window*   window = nullptr;
	SDL_Renderer* renderer = nullptr;

public:
	void init_assert(bool flag, const char* error_msg) {
		if (flag) return;
	
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"��Ϸ�����ˣ�",
			error_msg,
			window
		);
		exit(-1);
	}
	/**
	* @brief ��Ϸ��ѭ��
	* @param argc ����1
	* @param argv ����2
	 * @return ����˵��
	 *     -<em>-1 or other</em> fail
	 *     -<em>1</em> succeed
	*/
	int run(int arge, char **argv) {
		Uint64 last_counter = SDL_GetPerformanceCounter();
		Uint64 counter_fred = SDL_GetPerformanceFrequency();

		while (!is_quit)
		{
			while (SDL_PollEvent(&event))
			{
				on_input();
			}
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter);
			current_counter = last_counter;
			if (delta * 1000 < 1000.0 / fps)
				SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));

			on_update(delta);

			// ��Ⱦ��ͼ
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			on_render();
			SDL_RenderPresent(renderer);
		}

		return 0;
	}
protected:
	GameManager() {
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL ��ʼ��ʧ��");

		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL Image ��ʼ��ʧ��");

		init_assert(Mix_Init(MIX_INIT_MP3), "SDL mixer��ʼ��ʧ��");

		init_assert(!TTF_Init(), "SDL TTf ��ʼ��ʧ��");

		// ��ʼ����Ƶ�豸��������Ϊ 44100Hz����ʽΪĬ�ϣ�ͨ����Ϊ 2��������������������СΪ 2048 �ֽڡ�
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		// ����һ�����ڣ���ʾ����Ļ���ģ�����ģʽΪ��ʾ��SDL_WINDOW_SHOWN����
		window = SDL_CreateWindow(
			u8"����",                // ���ڱ��⣬֧�� UTF-8 ����
			SDL_WINDOWPOS_CENTERED,  // ����ˮƽλ�ã���Ļ����
			SDL_WINDOWPOS_CENTERED,  // ���ڴ�ֱλ�ã���Ļ����
			1280, 720,               // ���ڴ�С��1280x720
			SDL_WINDOW_SHOWN         // ���ڱ�־����ʾ����
		);

		// SDL ��Ⱦ���������ڴ����л���ͼ�Ρ�
		renderer = SDL_CreateRenderer(
			window,                      // ��Ⱦ���Ὣͼ����Ƶ��ô����С�
			-1,                          // ʹ��Ĭ�ϵ�ͼ��������������Ϊ -1 ��ʾ�� SDL �Զ�ѡ����ѵ��������򣩡�
			SDL_RENDERER_ACCELERATED |   // Ӳ�����١�
			SDL_RENDERER_PRESENTVSYNC |  // ��ֱͬ��
			SDL_RENDERER_TARGETTEXTURE   // ������Ⱦ
		);

	};
	~GameManager() {
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
	};

	void on_input() {

	}

	void on_update(double delta) {

	}

	void on_render() {

	}




};



#endif // __GAMEMANAGER__!
