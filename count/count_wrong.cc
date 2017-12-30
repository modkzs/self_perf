#include "config.h"

class WrongCounter : public Counter{
private:
  int count = 0;
public:
  void inc() override {++count;}
  int read() override {return count;}
};

WrongCounter count;

#include "count_test.h"
