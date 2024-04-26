/*

Design a system HONEY BEE COUNTER with following specifications. The bees
are assumed to enter the bee hive in rectangular box through a small hole. Another
hole is made for the bees to exit. Assume suitable sensors are placed at entry & exit
holes. The system is designed to display the number of bees in hive at any time.
Assume initially there are no bees in hive.
Write block diagram & pseudo code of above system implementation.

*/

/*
SYSTEM DESIGN

1) THE SYSTEM COMPRISES OF TWO ULTRASONIC SENSORS AT THE ENTRY AND EXIT POINTS OF THE HONEYBEE COUNTER
2) THE ULTRASONIC SENSOR DETECTS THE PRESCENCE OF A HONEYBEE, AND INCREMENTS THE VALUE OF COUNT AT ENTRY AND EXIT
3) THE DIFFERENCE IN THE TWO VALUES INDICATE THE NUMBER OF HONEYBEES IN THE COUNTER

*/

/************************CODE FOR HONEYBEE COUNTER*******************************************/

#include<stdio.h>
#include<ultrasonic.h>
#include<LCD.h>

int count_entrance(int count_entry)
{
    if(ultrasonic_detect() == 1)
    {
        count_entry = count_entry + 1;
    }

    return count_entry;
}

int count_exit(int count_exit)
{
    if(ultrasonic_detect() == 1)
    {
        count_exit = count_exit - 1;
    }

    return count_exit;
}

void main()
{
    ultrasonic_init();
    LCD_init();

    int entry_count = 1;
    int exit_count = 0;

    int count_bees = 0;

    while (1)
    {
        entry_count = count_entrance(entry_count);
        exit_count = count_exit(exit_count);

        LCD_Refresh();

        if(entry_count > exit_count)
        {
            count_bees = (entry_count - exit_count);
        }

        else
        {
            count_bees = 0;
        }

        printf("Number of bees remaining : %d", count_bees);
        LCD_Display(count_bees);
        Delay(1000);
    }
    

}