/*
The above program demonstrates the IPC communication between 4 different tasks executing a print on the console
The program uses semaphores to synchronize the scheduling of the tasks

task1 is used to cooridinate the working of all the tasks and semd semaphores to execute task2 and task3

Task 2 and task 3 execute upon trigger from task 1 and also trigger a semaphore release to task 1 after the process is complete

Task 4 runs independently and is not affected by the operations of task 1 and task 2
*/

#include<rtl.h>

#define SEMAPHORE_COUNT 4

// Task IDS of task 1, task 2, task 3 and task 4

OS_TID id1, id2, id3, id4;


// Initialise a list to store sempahore objects
OS_SEM semaphores[SEMAPHORE_COUNT];

// Task-1 functionality
__task void task1(void)
{
    // outputting the id1 of task 1
    id1 = os_tsk_self();

    // Creating task 2, task 3 and task 4 in runtime of task 1
    id2 = os_tsk_create(task2, 1);
    id3 = os_tsk_create(task3, 1);
    id4 = os_tsk_create(task4, 1);

    while(1)
    {
        // Write any kind of code to process for task1
        printf("Task 1 is running....");

        // adding delay for demo
        os_dly_wait(1000);

        // use semaphore of task2 to start task2
        os_sem_send(&semaphore[0]);

        // wait for task2 to complete
        os_sem_wait(&sempahore[1], 0xFFFF);

        //use semaphore of task3 to start task3
        os_sem_send(&sempahore[2]);

        //wait for task3 to complete
        os_sem_wait(&semaphore[3], 0xFFFF);

}

// Task 2 functionality
__task void task2(void)
{
    while(1)
    {
        // First wait for semaphore token from task1
        os_sem_wait(&semaphore[0], 0xFFFF);

        // Execute statements for task2
        printf("Task 2 is running.....");

        // adding delay for demo
        os_dly_wait(2000);

        // signal task 1 that it has finished
        os_sem_send(&semaphore[1]);
    }
}

// Task 3 functionality
__task void task3(void)
{
    while(1)
    {
        // First wait for the semaphore token from task1
        os_sem_wait(&semaphore[2], 0xFFFF);

        //Execute the statements for task3
        printf("Task 3 is running.....");

        // adding delay for demo
        os_dly_wait(3000);

        // signal that task 3 is complete
        os_sem_send(&semaphore[3]);
    }
}

// Task 4 is an independent task
__task void task4(void)
{
    while(1)
    {
        printf("Task 4 runs independently");

        // adding delay for demo
        os_dly_wait(4000);
    }
}


void main()
{
// Initialise the semaphore object having 4 tokens (as an array structure)

    for(int i = 0; i < SEMAPHORE_COUNT, i++)
    {
        os_sem_init(&semaphores[i], 0);
    }

    // Create the 4 tasks
    os_tsk_create(task1, 1);
    os_tsk_create(task2, 1);
    os_tsk_create(task3, 1);
    os_tsk_create(task4, 1);

    // Initialise the RTX Kernel
    os_sys_init(taks1);
}