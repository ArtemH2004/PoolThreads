#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            worker();
        });
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();
    for (thread& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(const function<void()>& task) {
    {
        unique_lock<mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void ThreadPool::worker() {
    while (true) {
        function<void()> task;
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
