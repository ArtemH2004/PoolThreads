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
       if (worker.joinable())
           worker.join();
    }
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
            if (stop) 
                return;
            task = move(tasks.front());
            tasks.pop();
            tasksCount--;
        }
        task();
          printStatus();
    }
}

bool ThreadPool::isEmpty() {
    return tasks.empty();
}

void ThreadPool::printStatus() const {
    lock_guard<mutex> lock(queueMutex);
    if (tasksCount != 0)
        cout << "Tasks remaining in the queue: " << tasksCount << endl;
}
