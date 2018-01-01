#include "../api.h"
#include "config.h"

thread_local int local_count = 0;
thread_local int thread_id;

class ThreadLocalCounter : public Counter{
private:
  int zero = 0;
  int final_count = 0;
  int* countp[TOTAL_THREADS];
  SpinLock lock;

public:
  ThreadLocalCounter() {
    for(int i = 0; i < TOTAL_THREADS; i++){
      countp[i] = &local_count;
    }
  }
 
  void inc() override {
    local_count++;
  }
  
  int read() override {
    int sum = final_count;
    for(int i = 0; i < TOTAL_THREADS; i++){
      sum += *(countp[i]);
    }
    return sum;
  }
  
  void init_thread() {
    thread_id = get_thread_id();
    countp[thread_id] = &local_count;
  }
  
  void uninit_thread() {
    lock.lock(); 
    final_count += local_count; 
    lock.unlock();
    countp[thread_id] = &zero;
  }
};

ThreadLocalCounter count;

#include "count_test.h"
