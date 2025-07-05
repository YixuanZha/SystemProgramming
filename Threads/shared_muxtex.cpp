#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>

// Read and write lock, reader first

// unqiue_lock and shared_lock 

int shared_value = 0;
std::shared_mutex shared_mtx;

void read_func(int id)
{
    std::shared_lock<std::shared_mutex> lock(shared_mtx);
    std::cout << "(Reader " << id << ") enter critical area, the value is: " << shared_value << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "(Reader " << id << ") leave critical area" << std::endl;
}

void write_func(int id)
{
    std::unique_lock<std::shared_mutex> lock(shared_mtx);

    int new_value = ++shared_value;
    std::cout << "(Writer " << id << ") enter critical area, the value is: " << new_value << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "(Writer " << id << ") leave critical area" << std::endl;
}

int main()
{
    std::cout << "Start ....." << std::endl;

    std::vector<std::thread> threads;

    std::cout << "Start three reader threads" << std::endl;
    threads.push_back(std::thread(read_func, 1));
    threads.push_back(std::thread(read_func, 2));
    threads.push_back(std::thread(read_func, 3));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Start one writer thread" << std::endl;
    threads.push_back(std::thread(write_func,1));
    // threads.push_back(std::thread(write_func,2));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout <<"Start another two reader threads" << std::endl;
    threads.push_back(std::thread(read_func,4));
    threads.push_back(std::thread(read_func,5));

    for(auto& th: threads)
    {
        th.join();
    }
    std::cout << "\n all the threads are finished, the final value is: " << shared_value << std::endl;

    return 0;
}