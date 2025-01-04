#define SDL_MAIN_HANDLED  // 告知 SDL 不需要替换标准的 main 函数，允许开发者使用自定义的 main 函数。
#include "GameManager.h"
#include "MallocHook.h"


int main(int argc, char **argv)
{
	return GameManager::getInstance()->run(argc, argv);
}