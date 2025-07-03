#include <iostream>
#include <thread>
#include <chrono>

void do_work()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t;
    std::cout <<"1. default-constructed thread is joinable? " << std::boolalpha << t.joinable() << std::endl;

    t = std::thread(do_work);
    std::cout << "2. After creation, thread is joinable? " << t.joinable() << std::endl;

    t.join();
    std::cout << "3. After join, thread is joinable? " << t.joinable() << std::endl;

    t = std::thread(do_work);
    t.detach();
    std::cout << "4. After detach(), thread is joinable? " << t.joinable() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}