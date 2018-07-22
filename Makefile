CXX=g++
CXXFLAGS= -MMD -O2 
SRCS= $(wildcard *.cpp)
OBJS= $(SRCS:.cpp=.o)
DEPS= $(wildcard *.d)
TARGET= main

-include $(DEPS)
.PHONY:clean all run

run:$(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)
