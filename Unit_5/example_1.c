/*
A SIMPLE PROGRAM TO SHOW THE CONCURRET OPERATION OF TWO TASKS ON RTX-RTOS MIDDLEWARE

CODE FUNCTIONALITY
1) THE CODE CREATES TWO TASKS, task1 & task2 AND ALLOTS THEM THEIR IDS id1 AND id2
2) TASK 1 AND TASK 2 ARE TO BE REPEATED AFTER A DELAY OF 50ms. 
3) ALSO, THERE EXISTS A DELAY OF 20ms BETWEEN BOTH TASKS TASK 1 AND TASK 2

*/

#include<rtl.h>

OS_TID id1, id2;                    // Initializing the IDs of the two tasks that are to be created

__task void task1(void);            // Prototype for task1
__task void task2(void);            // Prototype for task1

__task void task1(void)
{
    id1 = os_tsk_self();            // Retrieves the ID of task-1

    id2 = os_tsk_create(task2, 0);      // Creates task2 dynamically. Hence, task2 is comfortably created inside task1

    for(;;)
    {
        // place some task 1 related processing code here
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);     // Task 1 is to toggle the GPIOD-15 pin on the MCU

        // Scheduling tasks
        os_evt_set(0x0004, 0xFFFFF);                // After the completion of the event, the task-1 sets the event flag register to 0x0004 to indicate the 4th event flag has been set
        os_wait_evt_or(0x0004, 0xFFFF);             // The task1 waits for a period of 20ms here in the process of waiting for 4th event flag from task2 to be set. This is because the task2 has slept for 20ms after it had detected 4th event flag
        os_dly_wait(5);                             // After sensing that the 4th event flag is set by task2, the task 1 now sleeps for 50ms before it resumes its task again
    }

} 

__task void task2(void)
{
    for(;;)
    {
        // place some task 2 related code here
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);

        // Scheduling task
        os_wait_evt_or(0x0004, 0xFFFF);             // task2 waits for the 4th event flag to be set so as to continue its execution. 0xFFFF indicates infinite timeout in the process of waiting for the event flag to be set
        os_dly_wait(2);                             // After waiting till flag 4 was set, the task2 pauses for a period of 20ms
        os_set_evt(0x0004, 0xFFFF);                 // Task2 sets the 4th event flag after sleeping for 20ms. It sets an infinite timeout for 0x0004

    }
}

void main()
{
    os_sys_init(task1);                             // Intiialises the kernel and starts to execute task1 in the ready queue
}