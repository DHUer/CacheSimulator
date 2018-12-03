CPP=g++
SRCS=$(wildcard *.cpp)
BASE=$(basename $(SRCS))
OBJS=$(addsuffix .o, $(BASE))
CPPFLAGS=-ggdb3
myCacheSimulator:$(OBJS) $(wildcard *.h)
	g++ -o $@ $(CPPFLAGS) $(OBJS)
%.o:%.cpp
	g++ $(CPPFLAGS) -c $<

.PHONY:clean
clean:
	rm -f myCacheSimulator *.o *.cpp~ *.h~ tmp* test Makefile~