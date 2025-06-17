/**

    @file      GameManager.h
    @brief			游戏管理器类，负责游戏的初始化、主循环、事件处理和渲染等功能。
    @details   GameManager::getInstance() 是一个单例模式的实现，确保全局只有一个游戏管理器实例。
    @author    yysheep
    @date      17.06.2025
    @copyright © yy sheep, 2025. All right reserved.

**/
#pragma once

#include <SDL.h> // SDL 核心库，提供窗口、事件和图形等功能。
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h> // SDL_image 扩展库，用于加载和处理图像（如 JPG、PNG）。
#include <SDL_mixer.h> // SDL_mixer 扩展库，用于加载和播放音频（如 MP3）。
#include <SDL_ttf.h> // SDL_ttf 扩展库，用于加载和渲染字体。
#include "ConfigManager.h"
#include "ResourcesManager.h"
#include "singleton.h"

class game_manager final :public Manager<game_manager>
{
	friend class Manager<game_manager>;
  friend struct ::std::default_delete<game_manager>;

private:
	int fps      = 180;
  int m_fps		 = 0;
	bool is_quit = false;

	SDL_Event     event{};
  // SDL_Window*   window = nullptr;
  // SDL_Renderer* renderer = nullptr;
  // SDL_Texture* tex_tilemap = nullptr;

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, SDL_DestroyRenderer};
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tex_tilemap{nullptr, SDL_DestroyTexture};

public:
	void init_assert(const bool flag, const char* error_msg) const {
		if (flag) return;

		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"游戏崩溃了！",
			error_msg,
			window.get()
		);
		exit(-1);
	}

  /**
   * @brief 游戏主循环
   * @param argc 参数1
   * @param argv 可变参数2
   * @return 0
   */
  int run(const int argc, char **argv) {
		const Uint64 last_counter = SDL_GetPerformanceCounter();
		Uint64 counter_fred = SDL_GetPerformanceFrequency();

		while (!is_quit)
		{
			while (SDL_PollEvent(&event))
			{
				on_input();
			}
			Uint64 current_counter = SDL_GetPerformanceCounter();
			const double delta = static_cast<double>(current_counter - last_counter);
			current_counter = last_counter;
			if (delta * 1000 < 1000.0 / fps)
				SDL_Delay(static_cast<Uint32>(1000.0 / fps - delta * 1000));

			on_update(delta);

			// 渲染绘图
			SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
			SDL_RenderClear(renderer.get());

			on_render();
			SDL_RenderPresent(renderer.get());
		}

		return 0;
	}

protected:
	game_manager() {
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL 初始化失败");

		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL Image 初始化失败");

		init_assert(Mix_Init(MIX_INIT_MP3), "SDL mixer初始化失败");

		init_assert(!TTF_Init(), "SDL TTf 初始化失败");

		const auto& config = ConfigManager::getInstance();
		init_assert(config->map.load("config/map.csv"), "游戏地图加载失败！");
		init_assert(config->load_level_config("config/level.json"), "加载关卡配置失败！");
		init_assert(config->load_game_config("config/config.json"), "加载游戏配置失败！");

		// 初始化音频设备，采样率为 44100Hz，格式为默认，通道数为 2（立体声），缓冲区大小为 2048 字节。
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		// 创建一个窗口，显示在屏幕中心，窗口模式为显示（SDL_WINDOW_SHOWN）。
		window.reset(SDL_CreateWindow(
			config->basic_template.window_title.c_str(),                // 窗口标题，支持 UTF-8 编码
			SDL_WINDOWPOS_CENTERED,  // 窗口水平位置：屏幕中心
			SDL_WINDOWPOS_CENTERED,  // 窗口垂直位置：屏幕中心
			config->basic_template.window_width, config->basic_template.window_height,
			SDL_WINDOW_SHOWN         // 窗口标志：显示窗口
		));

		// SDL 渲染器，用于在窗口中绘制图形。
		renderer.reset(SDL_CreateRenderer(
			window.get(),                      // 渲染器会将图像绘制到该窗口中。
			-1,                          // 使用默认的图形驱动程序（设置为 -1 表示让 SDL 自动选择最佳的驱动程序）。
			SDL_RENDERER_ACCELERATED |   // 硬件加速。
			SDL_RENDERER_PRESENTVSYNC |  // 垂直同步
			SDL_RENDERER_TARGETTEXTURE   // 纹理渲染
		));

		init_assert(ResourcesManager::getInstance()->load_from_file(renderer.get()), "渲染器加载失败！");
		generate_tile_map_texture();
		

	};
	~game_manager() override {
		// 清理 SDL 相关资源，避免内存泄漏或资源占用。
		// 销毁窗口资源，释放内存。
		// SDL_DestroyWindow(window.get());
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
		switch (event.type)
		{
		case SDL_QUIT:
			is_quit = true;
			break;
		default:
			break;
		}
	}

	void on_update(double delta) {
	  // const char* pc = new char[100];
	}

	void on_render() {
		const auto& instance = ConfigManager::getInstance();
		static SDL_Rect& rect_dst = instance->rect_tile_map;
		SDL_RenderCopy(renderer.get(), tex_tilemap.get(), nullptr, &rect_dst);
	}

	bool generate_tile_map_texture() {
		
		const map& map = ConfigManager::getInstance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::getInstance()->rect_tile_map;

		SDL_Texture* tex_tile_set = ResourcesManager::getInstance()->get_texture_pool().find(ResID::Tex_Tileset)->second;
		if (!tex_tile_set) {
			// 纹理指针为空
			std::cerr << "Error: Tileset texture is nullptr!" << std::endl;
			return false;
		}
		// 查询纹理尺寸
		int width_tex_tile_set, height_tex_tile_set = 0;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
		int num_tile_single_line = (int)std::ceill((double)width_tex_tile_set / SIZE_TILE);
	
		int width_tex_tile_map, height_tex_tile_map;
		width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
		height_tex_tile_map = (int)map.get_height() * SIZE_TILE;

		tex_tilemap.reset(SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ABGR8888,
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map));

		if (!tex_tilemap) return false;

		const auto& config = ConfigManager::getInstance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tilemap.get(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer.get(), tex_tilemap.get());
		
		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];

				const SDL_Rect rect_dst = {
					x * SIZE_TILE, y * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				SDL_Rect rect_src{
					(tile.terrian % num_tile_single_line) * SIZE_TILE,
					(tile.terrian / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				SDL_RenderCopy(renderer.get(), tex_tile_set, &rect_src, &rect_dst);

				if (tile.decoration >= 0)
				{
					rect_src = {
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE, SIZE_TILE
					};
					SDL_RenderCopy(renderer.get(), tex_tile_set, &rect_src, &rect_dst);
				}

			}
		}
		// 绘制基地
		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst =
		{
			idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE,
			SIZE_TILE, SIZE_TILE
		};
		SDL_RenderCopy(renderer.get(), ResourcesManager::getInstance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer.get(), nullptr);

		return true;
	}

};