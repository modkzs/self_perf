#include "../api.h"
#include "config.h"

class ThreadLocalCounter : public Counter{
private:
  thread_local static int count;
  int zero = 0;
  int final_count = 0;
  int* countp[TOTAL_THREADS];
  thread_local static int thread_id;
  SpinLock lock;

public:
  ThreadLocalCounter() {
    for(int i = 0; i < TOTAL_THREADS; i++){
      countp[i] = &zero;
    }
  }
 
  void inc() override {
    ThreadLocalCounter::count++;
  }
  
  int read() override {
    int sum = final_count;
    for(int i = 0; i < TOTAL_THREADS; i++){
      sum += *(countp[i]);
    }
    return sum;
  }
  
  void init_thread() {
    ThreadLocalCounter::thread_id = get_thread_id();
    countp[thread_id] = &count;
  }
  
  void uninit_thread() {
    lock.lock(); 
    final_count += ThreadLocalCounter::count; 
    lock.unlock();
    countp[thread_id] = &zero;
  }
};

thread_local int ThreadLocalCounter::count = 0;
thread_local int ThreadLocalCounter::thread_id = 0;
ThreadLocalCounter count;

#include "count_test.h"
