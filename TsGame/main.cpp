#define SDL_MAIN_HANDLED  // ��֪ SDL ����Ҫ�滻��׼�� main ��������������ʹ���Զ���� main ������
#include "GameManager.h"
#include "MallocHook.h"


int main(int argc, char **argv)
{
	return GameManager::getInstance()->run(argc, argv);
}