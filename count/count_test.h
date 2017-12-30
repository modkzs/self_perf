#include <thread>
#include <stdio.h>
#include <ctime>
#include "../api.h"
#include "config.h"

#define WAITING 0
#define RUN 1
#define STOP 2

volatile int run_flag = WAITING;

void read_perf_test(){
  while(run_flag != RUN) {}
  for(int i = 0; i < READ_COUNT; i++){
    count.read();
    barrier();
  }
}

void update_perf_test() {
  while(run_flag != RUN) {}
  for(int i = 0; i < UPDATE_COUNT; i++){
    count.inc();
    barrier();
  }
}

int main(){
  std::thread t[TOTAL_THREADS];

  for (int i = 0; i < UPDATE_THREADS; i++)
    t[READ_THREADS + i] = std::thread([](){count.init_thread(); update_perf_test(); count.uninit_thread();});

  for (int i = 0; i < READ_THREADS; i++)
    t[i] = std::thread([](){read_perf_test();});

  std::clock_t start = std::clock();
  run_flag = RUN;

  for (int i = 0; i < (READ_THREADS + UPDATE_THREADS); i++)
    t[i].join();

  double duration = ( std::clock() - start );

  printf("avg %.3f updates per second, avg %.3f reads per second\n", 
      (UPDATE_COUNT * UPDATE_THREADS) / duration, 
      (READ_COUNT * READ_THREADS)/duration);
  printf("final accuracy : result %d, actual %d, error %f\n", 
      UPDATE_THREADS * UPDATE_COUNT, count.read(), 
      count.read() / (UPDATE_THREADS * UPDATE_COUNT * 1.0));

  return 0;
}
