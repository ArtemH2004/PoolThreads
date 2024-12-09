#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <atomic>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator = (const ThreadPool&) = delete; 
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator = (ThreadPool&& other) = delete;
    void enqueue(const function<void()>& task);
    void printStatus() const;
    bool isEmpty();

private:
    void worker();
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutable mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop;
    size_t tasksCount = 0;
};