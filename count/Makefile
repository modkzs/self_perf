CC = g++
PROGS = count_atomic count_wrong count_thread_local

all: $(PROGS)

GCC_ARGS=-O3 -Wall -pthread -std=c++11

define make_cout
  $1: $1.cc count_test.h ../api.h config.h
	${CC} $(GCC_ARGS) -o $1 $1.cc
endef

$(foreach element, $(PROGS),$(eval $(call make_cout,$(element))))

clean: 
	rm -f $(PROGS)
