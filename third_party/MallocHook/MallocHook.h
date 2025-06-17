#pragma once
#include <format>
#include <fstream>
#include <sstream>
#include <mutex>
#include <new>
#include <map>


struct AllocatedInfo {
  void *ptr;
  size_t size;
  const char *file;
  long line;
};

class GlobalDate {
public:
  std::map<void *, AllocatedInfo> allocated;
  std::ofstream log_file;
  size_t total_allocated = 0;
  size_t current_used = 0;
  size_t peak_used = 0;
  bool enable = false;

  template<class T>
  void log(const T &message) {
    if (log_file.is_open()) {
      log_file << message << std::endl;
    }
  }

  GlobalDate() {
    enable = true;
    log_file.open("memory_leak.log");
    log("内存检测开始...");
  }

  ~GlobalDate() {
    log("内存泄漏报告:");
    for (const auto &[ptr, info]: allocated) {
      std::stringstream ss;
      ss << "泄漏: " << info.file << ":" << info.line << " ptr=" << info.ptr << " size=" << info.size;
      log(ss.str());
    }
    log( std::format("峰值内存使用: {} bytes", std::to_string(peak_used)));
    log_file.close();
  }
} __my_globaldate;



struct EnableGlob {
  bool was_enable;
  EnableGlob() {
    was_enable = __my_globaldate.enable;
  };
  operator bool() const { return was_enable; };
  ~EnableGlob() { __my_globaldate.enable = was_enable; };
};

// operator new 实现
void *operator new(size_t size, const char *file, long line) {
  if (size == 0)
    ++size; // avoid std::malloc(0) which may return nullptr on success

  void *ptr = ::std::malloc(size);

  if (EnableGlob e; e) {
    __my_globaldate.log(std::format("new: ptr = {}, size = {}", ptr, size));
    __my_globaldate.allocated[ptr] = AllocatedInfo{ptr, size, file, line};
  }
  if (ptr == nullptr)
    throw std::bad_alloc{};

  return ptr;
}


// operator delete 实现
void operator delete(void *ptr) noexcept {
  if (EnableGlob e; e) {
    //__my_globaldate.log(std::format("地址(已销毁)：{}", ptr));
    //__my_globaldate.allocated.erase(ptr);
  }
  ::std::free(ptr);
}

#define new new (__FILE__, __LINE__)