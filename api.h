#pragma once 

#include <atomic>
#include <mutex>
#include <sched.h>

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

static __inline__ void run_on(int cpu){
  cpu_set_t mask;

  CPU_ZERO(&mask);
  CPU_SET(cpu, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);
}
