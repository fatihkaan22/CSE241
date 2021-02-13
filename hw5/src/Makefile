CXX := g++

# for no color option compile with -Dno_color_hex
CXXFLAGS := --std=c++11 -g # -Dno_color_hex 

HDRS := $(wildcard *.h)

SRCS := $(wildcard *.cpp)

OBJS := $(SRCS:.cpp=.o)

EXEC := output

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
