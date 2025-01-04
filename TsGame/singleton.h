#pragma once
#include <memory>
#include <mutex>

// ����ģ��
template<typename T>
class Singleton
{

public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// �ṩ����ʵ���ķ��ʽӿ�
	static T& getInstance() {
		static std::once_flag initFlag; // ȷ���̰߳�ȫ�س�ʼ������
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

// ��һ�̵߳���
template<class T>
class Manager
{
public:
	Manager(const Manager&)   = delete;
	Manager operator=(const Manager&) = delete;
	// �ṩ����ʵ���ķ��ʽӿ�
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

