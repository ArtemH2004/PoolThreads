CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp ThreadPool.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=PoolThreadsExecutable

all: $(SOURCES) $(EXECUTABLE)

clear:
 rm -f ThreadPool *.o

$(EXECUTABLE): $(OBJECTS)
 $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
 $(CC) $(CFLAGS) $< -o $@

.PHONY: all clear