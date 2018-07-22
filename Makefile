CXX=g++
CXXFLAGS= -MMD -O2 -std=c++11
SRCS= $(wildcard *.cpp)
OBJS= $(SRCS:.cpp=.o)
DEPS= $(wildcard *.d)
TARGET= main
MDLS = $(widlcard ./mdl/*.mdl)

-include $(DEPS)
.PHONY:clean all run default

default:$(TARGET)

check:check_connected
	./check_connected.sh

check_connected:check_connected.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run:$(TARGET)
	./$(TARGET)

$(TARGET): $(TARGET).o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)
