#include <thread>
#include <stdio.h>
#include <ctime>
#include <atomic>
#include <poll.h>

#include "../api.h"
#include "config.h"

#define WAITING 0
#define RUN 1
#define STOP 2

volatile int run_flag = WAITING;

std::atomic_int read_count{0};
std::atomic_int update_count{0};

void read_perf_test(int index){
  run_on(index);
  while(run_flag != RUN) {}
  while(run_flag == RUN){
    for(int i = 0; i < READ_COUNT; i++){
      count.read();
      barrier();
    }
    read_count++;
  }
}

void update_perf_test(int index){
  run_on(index);
  while(run_flag != RUN) {}
  while(run_flag == RUN){
    for(int i = 0; i < UPDATE_COUNT; i++){
      count.inc();
      barrier();
    }
    update_count++;
  }
}

int main(){
  std::thread t[TOTAL_THREADS];

  for (int i = 0; i < READ_THREADS; i++)
    t[i] = std::thread([=](){read_perf_test(i/2);});

  for (int i = 0; i < UPDATE_THREADS; i++)
    t[READ_THREADS + i] = std::thread([=](){
        count.init_thread(); 
        update_perf_test(i/2); 
        count.uninit_thread();});

  poll(NULL, 0, 100);

  std::clock_t start = std::clock();
  run_flag = RUN;

  poll(NULL, 0, DURATION * 1000);

  run_flag = STOP;
  double duration = ( std::clock() - start );

  for (int i = 0; i < (READ_THREADS + UPDATE_THREADS); i++)
    t[i].join();

  printf("avg %.3f updates per unit, avg %.3f reads per unit\n", 
      update_count.load() / duration, 
      read_count.load() / duration);

  printf("final accuracy : result %d, actual %d, error %f\n", 
      update_count.load() * UPDATE_COUNT, count.read(), 
      count.read()  * 1.0 / (update_count.load() * UPDATE_COUNT));

  return 0;
}
