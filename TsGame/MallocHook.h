#ifndef __MALLOCHOOK__
#define __MALLOCHOOK__

// 添加要在此处预编译的标头
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <new>
#include <mutex>
#include <map>

struct AllocatedInfo
{
	void* ptr;
	size_t size;
	const char* file;
	long line;
};


struct GlobalDate
{
	std::map<void*, AllocatedInfo> allocated;
	std::mutex allocated_mutex;              // 保护 allocated 的互斥锁
	bool enable = false;
	
	GlobalDate() {
		enable = true;
		printf("内存检测...\n");
	}
	~GlobalDate()
	{
		enable = false;
		// 检查内存泄漏
		for (const auto& [ptr, info] : allocated) {
			printf("内存泄漏：%s\n第%ld行，ptr = %p, size = %zu\n", info.file, info.line, info.ptr, info.size);
		}
	}
} __my_globaldate;

struct EnableGlob
{
	bool was_enable;
	EnableGlob() {
		was_enable =  __my_globaldate.enable;
	}
	operator bool() {
		return was_enable;
	}
	~EnableGlob()
	{
		__my_globaldate.enable = was_enable;

	}
	
};

void* operator new(size_t size, const char* file, long line) {
	EnableGlob e;
	void* ptr = malloc(size);

	if (e) {
		printf("new: ptr = %p, size = %zd\n", ptr, size);
		__my_globaldate.allocated[ptr] = AllocatedInfo{ ptr, size, file, line };
	}
	if (ptr == nullptr)
		throw std::bad_alloc();

	return ptr;

}

#define new new(__FILE__, __LINE__)


void operator delete(void* ptr) {
	EnableGlob e;
	if (e) {
		printf("Delete: ptr = %p\n", ptr);
		__my_globaldate.allocated.erase(ptr);
	}
	free(ptr);
}


#endif //__MALLOCHOOK__
