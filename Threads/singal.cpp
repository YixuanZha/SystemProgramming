#include <sys/wait.h>
#include <signal.h>
#include <iostream>

using namespace std;

void sig2(int sig)
{
    cout << __func__ << "(),"<< __LINE__ << ", sig: " << sig << endl;
    exit(0);
}

int main()
{
    signal(SIGINT,sig2);
    while (1)
    {
        sleep(1);
    }
    
    return 0;
}