CC = g++
FLAGS = -std=c++11
INCLUDE = -I.

SRCS = $(wildcard *.cpp)

PROGS = $(patsubst %.cpp,%,$(SRCS))

build: $(PROGS)

%: %.cpp
	 ${CC} ${FLAGS} -o $@ $<  ${INCLUDE}

clear:
	find . -maxdepth 1 -type f -executable -delete