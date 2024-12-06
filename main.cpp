#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std;

mutex consoleMutex;
mutex fileMutex;

// Example
void writeFile(int id) {
    lock_guard<mutex> lock(fileMutex);
    ofstream outFile("output.txt", ios_base::app);
    
    if (outFile.is_open()) {
        outFile << "Task " << id << " is being processed in thread "
            << this_thread::get_id() << endl;
        outFile.close();
    }
}

int main() {
    size_t numThreads = thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 2;
    }

    cout << "On your computer the number of threads = " << numThreads << endl;

    {
        ThreadPool threadPool(numThreads);

        for (int i = 0; i < 100; ++i) {
            threadPool.enqueue([i] {
                writeFile(i);
                });
        }

        while (!threadPool.isEmpty());
    }
     
    cout << "Queued tasks are complete" << endl;
    cout << "Tasks and the threads in which they were executed are recorded in the file 'output.txt'" << endl;

    return 0;
}
