CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17 

SRCS = expression.cpp main.cpp tests.cpp 
OBJS = $(SRCS:.cpp=.o)

all: differentiator test 

differentiator: main.o expression.o
	$(CXX) $(CXXFLAGS) -o differentiator main.o expression.o

test: tests.o expression.o
	$(CXX) $(CXXFLAGS) -o tests tests.o expression.o
	./tests

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) tests differentiator

.PHONY: all clean test