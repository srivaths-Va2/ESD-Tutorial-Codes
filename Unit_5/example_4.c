/*
Example code to show the working of a mailbox on RL-RTX 

*/

#include<rtl.h>

OS_TID id1, id2;

OS_MUT mutex;

// Creating the mailbox object
os_mbx_declare(mailbox1, 20);           // declared a mailbox with name mailbox1 and size 20 bytes


__task void task1(void)
{
    // Obtaining the Id of task1
    id1 = os_tsk_self();

    // Creating task 2 within task1
    id2 = os_tsk_create(task2, 1);

    // Initializing the mailbox
    os_mbx_init(&mailbox1, sizeof(mailbox1));

    // intializing the mutex
    os_mut_init(&mutex);

    while(1)
    {
        // Waiting to aquire the mutex
        os_mut_wait(&mutex);

        // After aquiring the mutex, populate the mailbox with data
        char msg[6] = "Hello";
        os_mbx_send(&mailbox1, &msg, 0xFFFF);               // 0xFFFF to indicate an infinite timeout

        //releasing the mutex after job completion
        os_mut_release(&mutex);

        // Delay for demo
        os_dly_wait(1000);
    }

}

__task void task2(void)
{
    while(1)
    {
        // Receive the message from mailbox and print
        char text_r[6];
        os_mbx_wait(&mailbox1, &text_r, 0xFFFF);

        // print the received message
        printf("Received message = %s", text_r);

        // delay for demo
        os_dly_demo(2000);
    }
}

void main()
{
    // Initialising the RTX kernel and starting task1
    os_sys_init(task1);
}

