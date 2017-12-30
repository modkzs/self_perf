#include <atomic>
#include "config.h"

class AtomicCounter : public Counter{
private:
  std::atomic_int count{0};
public:
  void inc() override {++count;}
  int read() override {return count.load();}
};

/*std::atomic_int count{0};

void init_thread(){}
void uninit_thread(){}
void global_init(){}

void inc(){
  ++count;
}

int read(){
  return count.load();
}*/

AtomicCounter count;

#include "count_test.h"
