#define SDL_MAIN_HANDLED 1 // 告知 SDL 不需要替换标准的 main 函数，允许开发者使用自定义的 main 函数。
#include "game_manager.h"

int main(const int argc, char **argv)
{
	return game_manager::getInstance()->run(argc, argv);
}