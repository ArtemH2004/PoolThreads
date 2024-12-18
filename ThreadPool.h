#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <fstream>
#include <cassert>

#if defined (_WIN32) || defined (_WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

using namespace std;

using func = std::function<void(int)>;

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(const function<void()>& task);
    bool isEmpty();
    unsigned int getThreadCount();

private:
    void worker();
    static unsigned int workerWrapper(void* arg); 
    static void* workerWrapperPosix(void* arg);

    vector<void*> workers;
    queue<function<void()>> tasks;
    mutable mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop;
    size_t tasksCount = 0;

#if defined (_WIN32) || defined (_WIN64)
    static unsigned int getCurrentThreadId();
#else
    static int getCurrentThreadId();
#endif
};
