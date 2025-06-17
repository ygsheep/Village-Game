#pragma once
#include <functional>
#include "SDL.h"
#include "Timer.h"

class Animation
{
public:
	Animation() {
		timer.set_one_shot(false);
		timer.set_function([&]() {
			idx_frame++;
			if (idx_frame >= rect_src_list.size() )
			{
				idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
				if (!is_loop && on_finished) on_finished();
			}
			});

	};
	~Animation() {
	
	};
	using PlayCallBack = std::function<void()>;

	void reset() {
		idx_frame = 0;
		timer.restart();
	}

	void set_frame_data(SDL_Texture* texture, const int num_h, const int num_v, const std::vector<int>& idx_list) {
		int width_tex, height_tex;
		SDL_QueryTexture(texture, NULL, NULL, &width_tex, &height_tex);
		this->texture = texture;

		this->width_frame = width_tex / num_h;
		this->height_frame = height_tex / num_v;

		rect_src_list.resize(idx_list.size());

		for (size_t i = 0; i < idx_list.size(); i++)
		{
			int idx = idx_list[i];
			int x = idx % num_h;
			int y = idx / num_h;
			rect_src_list[i] = { x * width_frame, y * height_frame, width_frame, height_frame };
		}
	}

	void set_loop(const bool flag) {
		is_loop = flag;
	}

	void set_interval(const double val) {
		timer.set_wait_time(val);
	}
	
	void set_callback(const PlayCallBack &callback) {
		on_finished = callback;
	}
	
	void update(const double delta) {
		timer.update(delta);
	}
	
	void render(SDL_Renderer* renderer,const SDL_Point& pos, double angio = 0) {
		static SDL_Rect rect_dst = { 0, 0, width_frame, height_frame };

		rect_dst.x = pos.x;
		rect_dst.y = pos.y;
		
		SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angio, NULL, SDL_FLIP_NONE);
	}


private:
	Timer timer;
	bool is_loop = true;
	size_t idx_frame = 0;
	PlayCallBack on_finished;
	SDL_Texture* texture;
	std::vector<SDL_Rect> rect_src_list;
	int width_frame, height_frame;
};

