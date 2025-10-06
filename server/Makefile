CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -O2


TARGET = server


SRCS = server.cpp handlers.cpp


OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
        $(CXX) -o $@ $(OBJS) $(CXXFLAGS)

%.o: %.cpp
        $(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
        rm -f $(TARGET) $(OBJS)

.PHONY: all clean
