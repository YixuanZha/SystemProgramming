#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

std::mutex mtx;
// static int num = 10; // global variable -> shared in data area 
int* num = new int(10); // heap -> shared
int* num_array = new int[10];

int* num_arr[];
int num_arr[];

char nums[10] = {0};

std::vector<int> num_vec(100,0);

int* times = (int*)malloc(1); //->malloc

void Read()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "The num is: " << ++*num << std::endl;
}

int main()
{
    std::thread Thread1(Read);
    std::thread Thread2(Read);

    Thread1.join();
    Thread2.join();

    delete num; // used for new
    free(times); //used for malloc
    return 0;
}
