/*
The code demonstrates the working of a simple mutex to ensure data integrity between tasks trying to access a shared resource

task 1 tries to access the shared resource and modifies the shared data by adding 1 to each element
task 2 tries to access the shared resource and prints the sum after calculating sum of shared data elements

a mutex is used for resource synchronization so that at any time, the shared resource is accessed by one of the tasks to prevent resource synchronization faults

*/

#include<rtl.h>

#define SHARED_DATA_SIZE      100

int shared_data[SHARED_DATA_SIZE];

OS_MUT mutex;

OS_TID id1, id2;

__task void task1(void)
{
    // obtain id of task1
    id1 = os_tsk_self();

    // create task 2 inside task 1 block
    id2 = os_tsk_create(task2, 1);

    // Intialises the shared data
    for(int i = 0; i < SHARED_DATA_SIZE; i++)
    {
        shared_data[i] = 0;
    }

    // Intialise the mutex
    os_mut_init(&mutex);

    while(1)
    {
        // aquire the mutex before accessing shared data and modifying it
        os_mut_wait(&mutex, 0xFFFF);

        // after aquiring the mutex, go ahead and modify the shared data

        for(int i = 0; i < SHARED_DATA_SIZE; i++)
        {
            shared_data[i] = shared_data[i] + 1;
        }

        // release the mutex
        os_mut_release(&mutex);

        // Print status message to the monitor
        printf("The items in the shared data list have been modified");

        // Delay function to demo
        os_dly_wait(1000);
    }

}

__task void task2(void)
{
    while(1)
    {
        // aquire the mutex before accessing the shared resource
        os_mut_wait(&mutex, 0xFFFF);

        // After aquiring the mutex, process the shared data to calculate the sum
        int sum = 0;

        for(int i = 0; i < SHARED_DATA_SIZE, i++)
        {
            sum = sum + shared_data[i];
        }

        // release the mutex
        os_mut_release(&mutex);

        // print status to the monitor
        printf("Sum = %d", sum);

        // Delay for demo
        od_dly_wait(2000);

    }
}

void main()
{
    // Initialising the task1
    os_sys_init(task1);
}