#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> task_queue;
bool task_finished = false;

void Producer()
{
    std::cout << "This is producer: Produce 10 tasks in 3 secs" << std::endl;

    for (int i = 1; i <= 10; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push(i);
            std::cout << "Producer: produce one task: " << "'" << i << "'" << std::endl;
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        task_finished = true;
        std::cout << "Producer: all of the 10 tasks have been finished" << std::endl;
    }
    cv.notify_one();
}

void Consumer()
{
    std::cout << "Consumer: consumer start to spend the tasks" << std::endl;
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (!task_finished && task_queue.empty())
        {
            cv.wait(lock);
        }
        if (task_queue.empty() && task_finished)
        {
            std::cout << "Consumer: the task queue is empty -> END" << std::endl;
            break;
        }

        int task = task_queue.front();
        task_queue.pop();
        lock.unlock();
        std::cout << "Consumer spenet the task:" << task << std::endl;
    }
}

int main()
{
    std::thread Producer_thread(Producer);
    std::thread Consumer_thread(Consumer);

    Producer_thread.join();
    Consumer_thread.join();

    std::cout << "END...." << std::endl;
    return 0;
}



