#include "ThreadPool.h"
#include <iostream>
#include <functional>

using namespace std;

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
#if defined (_WIN32) || defined (_WIN64)
        unsigned int threadID;
        HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &workerWrapper, this, 0, &threadID);
        if (thread == 0) {
            cerr << "Failed to create thread: Error Code " << GetLastError() << endl;   
        }
        else {
            workers.push_back(thread);
        }
#else
        pthread_t thread;
        if (pthread_create(&thread, nullptr, &workerWrapperPosix, this) != 0) {
            cerr << "Failed to create thread: Error Code " << strerror(errno) << endl;
        }
        else {
            workers.push_back((void*)thread);
        }
#endif
    }
    cout << "ThreadPool started with " << numThreads << " threads." << endl;
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();

    for (auto& worker : workers) {
#if defined (_WIN32) || defined (_WIN64)
        WaitForSingleObject((HANDLE)worker, INFINITE);
        CloseHandle((HANDLE)worker);
#else
        pthread_join((pthread_t)worker, nullptr);
#endif
    }

    cout << "ThreadPool shutdown" << endl;
}

void ThreadPool::enqueue(const function<void()>& task) {
    {
        lock_guard<mutex> lock(queueMutex);
        tasks.push(task);
        tasksCount++;
    }
    condition.notify_one();
}

void ThreadPool::worker() {
    while (!stop) {
        function<void()> task;
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop) return;
            task = move(tasks.front());
            tasks.pop();
            tasksCount--;
        }
        task();
    }
}

bool ThreadPool::isEmpty() {
    return tasks.empty();
}

unsigned int ThreadPool::getThreadCount() {
    return (unsigned int)workers.size();
}

#if defined (_WIN32) || defined (_WIN64)
unsigned int __stdcall ThreadPool::workerWrapper(void* arg) {
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    if (pool) { 
        pool->worker();
    }
    return 0;
}

unsigned int ThreadPool::getCurrentThreadId() {
    return GetCurrentThreadId();
}
#else
void* ThreadPool::workerWrapperPosix(void* arg) {
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    if (pool) {
        pool->worker();
    }    
    return nullptr;
}

int ThreadPool::getCurrentThreadId() {
    return (int)pthread_self();
}
#endif
