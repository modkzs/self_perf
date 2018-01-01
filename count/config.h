#pragma once

#define READ_COUNT 1000
#define UPDATE_COUNT 1000

#define DURATION 1

#define READ_THREADS  1
#define UPDATE_THREADS 10
#define TOTAL_THREADS READ_THREADS+UPDATE_THREADS

class Counter{
public:
  void init_thread(){}
  void uninit_thread(){}

  virtual void inc() = 0;
  virtual int read() = 0;  
};
