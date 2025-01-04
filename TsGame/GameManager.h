#ifndef __GAMEMANAGER__
#define __GAMEMANAGER__

#include "singleton.h"
#include <SDL.h>          // SDL 核心库，提供窗口、事件和图形等功能。
#include <SDL_image.h>    // SDL_image 扩展库，用于加载和处理图像（如 JPG、PNG）。
#include <SDL_mixer.h>    // SDL_mixer 扩展库，用于加载和播放音频（如 MP3）。
#include <SDL_ttf.h>      // SDL_ttf 扩展库，用于加载和渲染字体。
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
			"游戏崩溃了！",
			error_msg,
			window
		);
		exit(-1);
	}
	/**
	* @brief 游戏主循环
	* @param argc 参数1
	* @param argv 参数2
	 * @return 返回说明
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

			// 渲染绘图
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			on_render();
			SDL_RenderPresent(renderer);
		}

		return 0;
	}
protected:
	GameManager() {
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL 初始化失败");

		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL Image 初始化失败");

		init_assert(Mix_Init(MIX_INIT_MP3), "SDL mixer初始化失败");

		init_assert(!TTF_Init(), "SDL TTf 初始化失败");

		// 初始化音频设备，采样率为 44100Hz，格式为默认，通道数为 2（立体声），缓冲区大小为 2048 字节。
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		// 创建一个窗口，显示在屏幕中心，窗口模式为显示（SDL_WINDOW_SHOWN）。
		window = SDL_CreateWindow(
			u8"窗口",                // 窗口标题，支持 UTF-8 编码
			SDL_WINDOWPOS_CENTERED,  // 窗口水平位置：屏幕中心
			SDL_WINDOWPOS_CENTERED,  // 窗口垂直位置：屏幕中心
			1280, 720,               // 窗口大小：1280x720
			SDL_WINDOW_SHOWN         // 窗口标志：显示窗口
		);

		// SDL 渲染器，用于在窗口中绘制图形。
		renderer = SDL_CreateRenderer(
			window,                      // 渲染器会将图像绘制到该窗口中。
			-1,                          // 使用默认的图形驱动程序（设置为 -1 表示让 SDL 自动选择最佳的驱动程序）。
			SDL_RENDERER_ACCELERATED |   // 硬件加速。
			SDL_RENDERER_PRESENTVSYNC |  // 垂直同步
			SDL_RENDERER_TARGETTEXTURE   // 纹理渲染
		);

	};
	~GameManager() {
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
	};

	void on_input() {

	}

	void on_update(double delta) {

	}

	void on_render() {

	}




};



#endif // __GAMEMANAGER__!
