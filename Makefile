CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp ThreadPool.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=PoolThreadsExecutable

.PHONY: all clean

all: $(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@