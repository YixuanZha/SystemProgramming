#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <cstring>
#include <vector>

using namespace std;

int main()
{
    //// Create a array on heap ----> can not share memory !!!!!!
    //// Because both parent process and child process have its own address space
    
    // Copy On Write: parent and child process share the same physical memory, until one of them write to the memory page, alloc a new page
    // and then copy the orignal data and wirte into its
    // ---> until parent process or child process write, they share the same physical memory  (Read Only !!!!)

    // int* nums = new int[10];
    // for(int i = 0; i < 10; i++)
    // {
    //     nums[i] = 0;
    // }

    // Use mmap() to create the shared memory
    // mmap() creates a new mapping in the virtual address space of the calling process
    int* nums = (int*)mmap(NULL, sizeof(int) * 10, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (nums == MAP_FAILED) {
        std::cerr << "mmap failed" << std::endl;
        return 1;
    }

    memset(nums, 0, sizeof(int) * 10);

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {   
        std::cout << "forked failed" << std:: endl;
    }
    // child process
    else if(pid == 0)
    {
        std::cout << "this is child, pid is: " << pid << std:: endl;
        // sleep(2);
        for(int i = 0; i < 10; i++)
        {
            nums[i] += 1;
        }
        cout<< "child process are exiting" << endl;
        exit(0);
    }

    // parents  process
    else
    {
        int status;
        for(int i = 0; i < 10; i++)
        {
            nums[i] += 2;
        }
        std::cout << "this is parent process, pid is: "<< pid << std:: endl;
        pid_t child_pid = wait(&status);
        if(WIFEXITED(status))
        {
            cout << "Child exited with status " << WIFEXITED(status) << endl;
        }
        else
        {
            cout << "child exited abnormally" << endl;
        }
    }

    for(int i = 0; i < 10;i++)
    {
        cout << nums[i] << ",";
    }
    cout << endl;
    // delete[] nums;

    return 0;
}