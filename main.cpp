#include <iostream>
#include <thread>
#include <chrono>
#include "ThreadPool.h"

using namespace std;

// Example
void writeFile(int id) {
    ofstream outFile("output.txt", ios_base::app);
    if (outFile.is_open()) {
        outFile << "Task " << id << " is being processed in thread "
            << this_thread::get_id() << endl;
        outFile.close();
    } 
    this_thread::sleep_for(chrono::seconds(1));
}

int main() {
    size_t numThreads = thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 2;
    }

    cout << "Number of threads: " << numThreads << endl;

    ThreadPool threadPool(numThreads);

    for (int i = 0; i < 10; ++i) {
        threadPool.enqueue([i] {
            writeFile(i);
        });
    }

    this_thread::sleep_for(chrono::seconds(10));

    return 0;
}
