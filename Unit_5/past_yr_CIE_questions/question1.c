/*

Create a multitasking application program on STM32F407 to demonstrate creation of concurrently executing tasks. 
Task1 is expected to control the blinking of two LEDs and send a text “RVCE” to GLCD(font size:16x24, blue color text with
background red). 

Task2 is to change font and colour of the textual display(Embedded
Systems) on GLCD concurrently. 

Use APIs of RL-RTX real time kernel and semaphores for resource synchronization.

*/

#include<rtl.h>
#include<LED.h>             // Using the BSP of LED
#include<GLCD.h>            // Using the BSP of GLCD

#define NUM_SEMPAHORES  2

OS_TID id1, id2;

OS_SEM semaphores[NUM_SEMPAHORES];

// task1 funtion to control the blinking of two LEDs and send text "RVCE" to the display

__task void task1(void)
{
    id1 = os_tsk_self();
    id2 = os_tsk_create(task2, 1);

    // Initialise the LEDs and GLCD display on the board
    LED_Init();
    GLCD_Init();

    while(1)
    {
        printf("Task 1 is running......");

        // Control the blinking of two LEDs (toggle the LEDs just once to show its control)
        LED_On(1);              // Turn on LED1
        os_dly_wait(1000);      // Internal delay to make the LED glow for a second
        LED_Off(1);             // Turn off LED
        os_dly_wait(1000);      // Internal delay to make the LED off for a second

        // Configuring the GLCD to have blue text and red background
        GLCD_SetTextColor(GLCD_TEXT_COLOR_BLUE);
        GLCD_SetBackColor(GLCD_BG_RED);

        // Initialising text RVCE
        char[5] text = "RVCE";

        // Printing RVCE on the GLCD display
        GLCD_DisplayString(0, 0, GLCD_FONT_MEDIUM, text);

        // Delay for demo porposes
        os_dly_wait(5000);          // Generating 5s delay

        // use semaphore of task2 to start task2
        os_sem_send(&semaphore[0]);

        // wait for task2 to complete
        os_wait(&semamphore[1], 0xFFFF);
    }


}

// task2 funtionality to display "Embedded Systems" wit different font and color on the display

__task void task2(void)
{
    while(1)
    {
        // Waiting for semaphore from task1
        os_sem_wait(&semaphore[0], 0xFFFF);

        //printing status for task2
        printf("Task 2 is running........");

        // Configure the GLCD to have red background and yellow text
        GLCD_SetTextColor(GLCD_TEXT_COLOR_YELLOW);
        GLCD_SetBackColor(GLCD_BG_RED);

        // Initializing text "Embedded Systems"
        char[17] text = "Embedded Systems";

        // Printing text on display
        GLCD_DisplayString(0, 0, GLCD_FONT_SMALL, text);

        // Delay for demo porposes
        os_dly_wait(10000)      // Generating 10s delay

        // use semaphoreof task2 to start task1
        os_sem_send(&semaphore[1]);
    }
}

void main()
{
    // Initalising the semaphores
    for(int i = 0; i < NUM_SEMPAHORES; i++)
    {
        os_sem_init(&semaphores[i], 0);
    }

    // Initialising the kernel
    os_sys_init(task1);
}