#pragma once

#include <memory>
#include <mutex>

template<typename Derived>
class Manager {
protected:
  Manager() = default;  // 保护构造函数
  virtual ~Manager() = default;  // 虚析构函数（支持多态）

public:
  // 禁止拷贝和赋值
  Manager(const Manager&) = delete;
  Manager& operator=(const Manager&) = delete;

  // 全局访问点（线程安全）
  static std::unique_ptr<Derived>& getInstance() {
    std::call_once(initFlag, [&] {
        instance.reset(new Derived());
    });
    return instance;
  }

protected:
  // 静态实例指针（由模板实例化时自动管理）
  inline static std::unique_ptr<Derived> instance = nullptr;
  inline static std::once_flag initFlag;  // 线程安全初始化标记
};
