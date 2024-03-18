/*

Create a multitasking program to demonstrate event flags to synchronize task execution.  
Create four tasks to simulate the operation of stepper motor driver. 
Use four LEDs blinking to simulate the activation of the four output driver stages. 
Create another concurrently executing task to display text on GLCD. 
Control the execution of tasks used to simulate the stepper motor excitation by inserting events at suitable statements. 
Use APIs of RL-ARM real time kernel.

*/


#include<rtl.h>

#define NUM_SEMAPHORES 4

OS_TID id1, id2, id3, id4;

OS_SEM sempahores[NUM_SEMAPHORES]

// Task1 to turn the motor from 0 to 90
__task void task1(void)
{
    // creating the tasks 2, 3, and 4 from task1
    id1 = os_tsk_self();
    id2 = os_tsk_create();
    id3 = os_tsk_create();
    id4 = os_tsk_create();

    while(1)
    {
        // performing task1
        printf("Rotating motor from 0-90....");

        // rotating motor from 0-90
        Step_Response1();

        // toggling PD12 LED to indicate 0-90 rotation
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

        // delay for demo
        os_dly_wait(50);

        // sending event to start task2
        os_evt_set(0x0001)

        // waiting for task4 to finish
        os_evt_wait_or(0x0004, 0xFFFF);
    }
}

// Task 2 to rotate the motor from 90-180
__task void task2(void)
{
    while(1)
    {
        // waiting for task1 to finish
        os_evt_wait_or(0x0001, 0xFFFF);

        // performing task2
        printf("Rotating motor from 90-180......");

        // rotating motor from 90-180
        Step_Response2();

        // toggling LED to indicate motor rotated from 90-180
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

        // delay for demo porpose
        os_dly_wait(50);

        // sending event for task3 to start
        os_evt_set(0x0002);
    }
}

// Task 2 to rotate the motor from 180-270
__task void task3(void)
{
    while(1)
    {
        // waiting for task2 to finish
        os_evt_wait_or(0x0002, 0xFFFF);

        // performing task3
        printf("Rotating motor from 180-270......");

        // rotating motor from 180-270
        Step_Response3();

        // toggling LED to indicate motor rotated from 180-270
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);

        // delay for demo porpose
        os_dly_wait(50);

        // sending event for task4 to start
        os_evt_send(0x0003);
    }
}

// Task 2 to rotate the motor from 90-180
__task void task2(void)
{
    while(1)
    {
        // waiting for task3 to finish
        os_evt_wait_or(0x0003, 0xFFFF);

        // performing task4
        printf("Rotating motor from 90-180......");

        // rotating motor from 270-360
        Step_Response2();

        // toggling LED to indicate motor rotated from 270-360
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);

        // delay for demo porpose
        os_dly_wait(50);

        // sending event for task1 to start
        os_evt_send(0x0004);
    }
}

// Function to rotate motor from 0-90
void Step_Response1()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
}

// Function to rotate motor from 90-180
void Step_Response1()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
}

// Function to rotate motor from 180-270
void Step_Response1()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
}

// Function to rotate motor from 270-360
void Step_Response1()
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
}

void main()
{
    // Intializing HAL
    HAL_Init();

    // Initialising MCU clock and GPIO port D
    SystemClock_Config();
    MX_GPIO_Init();

    // Initialising RL-RTX kernel
    os_sys_init(task1);
}
