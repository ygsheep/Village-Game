#pragma once
#include <memory>
#include <mutex>

// 单例模板
template<typename T>
class Singleton
{

public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// 提供单例实例的访问接口
	static T& getInstance() {
		static std::once_flag initFlag; // 确保线程安全地初始化单例
		std::call_once(initFlag, []() {
			instance.reset(new T());
			});
		return *instance;
	}


protected:
	Singleton() = default;
	~Singleton() = default;

private:
	inline static std::unique_ptr<T> instance = nullptr;
};

// 单一线程单例
template<class T>
class Manager
{
public:
	Manager(const Manager&)   = delete;
	Manager operator=(const Manager&) = delete;
	// 提供单例实例的访问接口
	static T* getInstance() {
		if (instance == nullptr)
			instance = new T();
		return instance;
	}

protected:
	Manager()   = default;
	~Manager()  = default;

private:
	inline static T* instance = nullptr;
};

