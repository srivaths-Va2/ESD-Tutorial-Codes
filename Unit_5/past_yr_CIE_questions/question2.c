/*

Create a multitasking program on STM32F407VG evaluation board to demonstrate Inter task communication. 
Create a task to read temperature value and send to another concurrently executing task. 
Display temperature value on GLCD.

Synchronize the execution of tasks. Use APIs of RTX kernel.

*/

#include<rtl.h>
#include<TH_LM75.h>
#include<GLCD.h>

#define NUM_SEMAPHORE 2

OS_TID id1, id2;

OS_MUT semaphores[NUM_SEMAPHORE];

// Declaring the mailbox and sizing it to 20 bytes of data
os_mbx_declare(mailbox1, 20);


// Task1 API to read temperature from the sensor
__task1 void task1(void)
{
    // obtaining the ID of task1
    id1 = os_tsk_self()

    // creating the task2
    id2 = os_tsk_create(task2, 1);

    // Initializing the mailbox object
    os_mbx_init(&mailbox1, sizeof(mailbox1));

    // Initialising the temperature sensor
    TH_Init();

    // Initialising a variable to hold the temperture sampled
    uint16_t temp_value;

    while(1)
    {
        printf("Task 1 is running.......");

        // Reading the temperature and delay of 1s to completely read temperature
        temp_value = TH_GetTemp();
        os_dly_wait(1000);

        // Storing the temperature obtained into a string variable using itoa function
        char str_temp[5];
        itoa(temp_value, str_temp, 10);             // itoa takes 3 parameters - (integer var, string to which converted integer will be stored, base of number system(here, base 10))

        // sending the data over the mailbox
        os_mbx_send(&mailbox1, &str_temp, 0xFFFF);
        
        // delay for demo porposes
        os_dly_wait(2000);

        // sending semaphore to start task2
        os_sem_send(&sempahores[0]);

        // waiting for task 2 to complete
        os_sem_wait(&semaphores[1]);
    }
}

// Task 2 API to display temperature value on the GLCD display
__task void task2(void)
{
    // Configure the GLCD for blue text and red background and initializing its settings
    GLCD_Init();
    GLCD_SetTextColor(GLCD_TEXT_COLOR_BLUE);
    GLCD_SetBackColor(GLCD_BG_RED);

    while(1)
    {
        // waiting for task1 to complete
        os_sem_wait(&semaphores[0]);

        // starting task2
        printf("Task 2 is running.......");

        // intermediate string to store temperature value from mailbox
        char str_temp1[5];

        // Reading the temperature from the mailbox
        os_mbx_wait(&mailbox1, &str_temp1, 0xFFFF);

        // Printing temoerature on display
        GLCD_DisplayString(0, 0, GLCD_FONT_MEDIUM, str_temp1);

        // delay for demo porposes
        os_dly_wait(3000);

        // sending sempahore to start task1 again
        os_sem_send(&semaphores[1]);
    }
}

void main()
{
    // Intializing the semaphores
    for(int i = 0; i < NUM_SEMAPHORE; i++)
    {
        os_sem_init(&semaphores[i], 0);
    }

    // Initialising the RTX kernel
    os_sys_init(task1);

}