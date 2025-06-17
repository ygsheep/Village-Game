#pragma once
#include <functional>

class Timer
{
public:
	Timer()  = default;
	~Timer() = default;

	void restart() {
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(double val) {
		wait_time = val;
	}

	void set_one_shot(bool flag) {
		one_shot = flag;
	}

	void set_function(std::function<void()> func) {
		on_timeout = func;
	}

	void pause() {
		paused = true;
	}

	void renums() {
		paused = false;
	}

	void update(double delte) {
		if (paused) return;

		pass_time += delte;

		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && shotted));
			shotted = true;
			if (can_shot && on_timeout) on_timeout();
			pass_time -= wait_time;

		}
	}


private:
	double pass_time = 0;
	double wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> on_timeout;

};
