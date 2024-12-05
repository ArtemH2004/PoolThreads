CXX = g++
CXXFLAGS = -std=c++11 -pthread

TARGET = threadpool_example

SRCS = main.cpp ThreadPool.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) output.txt
