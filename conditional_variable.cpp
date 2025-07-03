#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> task_queue;
bool production_finished = false;

void Producer()
{
    std::cout << "Producer start, produce 10 tasks in 3 secs" << std::endl;
    for (int i = 1; i <= 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        // ---------------------------------------------------------------------
        // lock_guard's scope
        {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push(i);
            std::cout << "Producer has prodeced the task: " << i << std::endl;
        }
        // ---------------------------------------------------------------------

        cv.notify_all();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        production_finished = true;
        std::cout << "The producer has produced all the tasks" << std::endl;
    }
    cv.notify_all();
}

void Consumer()
{
    std::cout << "Cusumer start.... waiting for tasks.........." << std::endl;

    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, []
                { return !task_queue.empty() || production_finished; });

        //------------------------------------------critial area ---------------------------------------------------------
        if (task_queue.empty() && production_finished)
        {
            std::cout << "Consumer queue is empty, produce finished, exit .........." << std::endl;
            break;
        }

        int task = task_queue.front();
        task_queue.pop();

        std::cout << "Consumer has spend the task: " << task << std::endl; 
    }
}

int main()
{
    std::cout <<"Start....... start one producer thread and a consumer thread" << std::endl;

    std::thread producer_thread(Producer);
    std::thread consumer_thread1(Consumer);
    std::thread consumer_thread2(Consumer);

    producer_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();

    std::cout << "End...." << std::endl;
    return 0;
}