#pragma once 

#include <atomic>
#include <mutex>

#define barrier() __asm__ __volatile__("": : :"memory")

std::atomic_int g_thread_id{0};

inline int get_thread_id(){
  int id = g_thread_id.load();
  ++g_thread_id;
  return id;
}

class SpinLock{
private:
  std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) {}
  }

  void unlock() {
    locked.clear(std::memory_order_release);
  }
};
