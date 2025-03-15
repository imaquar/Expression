CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17 

SRCS = expression.cpp tests.cpp 
OBJS = $(SRCS:.cpp=.o)

all: test 

test: tests.o expression.o
	$(CXX) $(CXXFLAGS) -o tests tests.o expression.o
	./tests

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) tests 

.PHONY: all clean