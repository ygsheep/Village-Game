#define SDL_MAIN_HANDLED  // 告知 SDL 不需要替换标准的 main 函数，允许开发者使用自定义的 main 函数。

#include <iostream>       // 标准 C++ 输入输出流库，用于打印调试信息等。
#include <string>
#include <fstream>
#include <sstream>
#include "cJSON.h"

#include <SDL.h>          // SDL 核心库，提供窗口、事件和图形等功能。
#include <SDL_image.h>    // SDL_image 扩展库，用于加载和处理图像（如 JPG、PNG）。
#include <SDL_mixer.h>    // SDL_mixer 扩展库，用于加载和播放音频（如 MP3）。
#include <SDL_ttf.h>      // SDL_ttf 扩展库，用于加载和渲染字体。
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
	// 初始化 SDL 核心库，参数 SDL_INIT_EVERYTHING 表示初始化所有子系统（视频、音频、计时器等）。
	SDL_Init(SDL_INIT_EVERYTHING);

	// 初始化 SDL_image 库，参数表示支持 JPG 和 PNG 格式的图片加载。
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	// 初始化 SDL_mixer 库，参数表示支持 MP3 格式的音频。
	Mix_Init(MIX_INIT_MP3);

	// 初始化 SDL_ttf 库，用于字体渲染。
	TTF_Init();

	// 初始化音频设备，采样率为 44100Hz，格式为默认，通道数为 2（立体声），缓冲区大小为 2048 字节。
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// 创建一个窗口，标题为“窗口”，宽度为 1280，高度为 720，显示在屏幕中心，窗口模式为显示（SDL_WINDOW_SHOWN）。
	SDL_Window* window = SDL_CreateWindow(
		u8"窗口",                // 窗口标题，支持 UTF-8 编码
		SDL_WINDOWPOS_CENTERED,  // 窗口水平位置：屏幕中心
		SDL_WINDOWPOS_CENTERED,  // 窗口垂直位置：屏幕中心
		1280, 720,               // 窗口大小：1280x720
		SDL_WINDOW_SHOWN         // 窗口标志：显示窗口
	);

	// 创建一个 SDL 渲染器，用于在窗口中绘制图形。
	SDL_Renderer* renderer = SDL_CreateRenderer(
		window,                  // 指向已经创建的 SDL_Window 的指针，渲染器会将图像绘制到该窗口中。
		-1,                      // 使用默认的图形驱动程序（设置为 -1 表示让 SDL 自动选择最佳的驱动程序）。
		SDL_RENDERER_ACCELERATED // 渲染器标志：使用硬件加速。可以提高图形性能（如果硬件支持）。
	);

	bool is_open = false;

	SDL_Event event;
	int fps = 60;

	SDL_Surface* sur_face = IMG_Load("icon.jpeg");
	SDL_Texture* text_ure = SDL_CreateTextureFromSurface(renderer, sur_face);

	TTF_Font* font = TTF_OpenFont(u8"ipix.ttf", 18);
	SDL_Color font_color = { 255, 0, 0, 255 };

	SDL_Surface* sur_text = TTF_RenderUTF8_Blended(font, u8"蹦蹦炸弹", font_color);
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


		// 处理数据
		rect_img.x = pos_cursor.x - rect_img.w / 2;
		rect_img.y = pos_cursor.y - rect_img.h / 2;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		// 渲染绘图
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, text_ure, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 20, 0, 255, 0, 150);

		SDL_RenderCopy(renderer, font_ure, nullptr, &rect_text);

		SDL_RenderPresent(renderer);

	}






	// 清理 SDL 相关资源，避免内存泄漏或资源占用。

	// 销毁窗口资源，释放内存。
	SDL_DestroyWindow(window);

	// 关闭 SDL_ttf 库，释放字体渲染相关资源。
	TTF_Quit();

	// 关闭 SDL_mixer 库，释放音频相关资源。
	Mix_Quit();

	// 关闭 SDL_image 库，释放图像加载相关资源。
	IMG_Quit();

	// 关闭 SDL 核心库，释放所有 SDL 初始化的系统资源。
	SDL_Quit();

	// 返回 0 表示程序正常退出。
	return 0;
}
