#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

int num = 10;

void foo(int num)
{
    std::lock_guard<std::mutex> lock(mtx);
    num++;
    std::cout << "num is :" << num << std::endl;
}

int main()
{
    std::thread th1(foo, num);
    std::thread th2(foo, num);

    // th1 = std::thread(foo);
    // th2 = std::thread(foo);

    th1.join();
    th2.join();

    return 0;
}