/*

Design MCU based system to control temperature of the furnace with following
specifications. The furnace temperature has to maintain at (30±1) C. Connect
suitable sensors & actuators. Display the temperature on LCD. The power
consumption has to be minimized. Show the design & implementation (diagram
+Program).

*/

/*

SYSTEM IMPLEMENTATION-
1) TEMPERATURE SENSOR TO MONITOR THE TEMPERATURE OF THE FURNACE
2) AIR COOLER TO BRING DOWN THE TEMPERATURE IF TEMPERATURE RISES
3) HEATER TO INCREASE THE TEMPERATURE IF TEMPERATURE FALLS

*/

/******************************* CODE*****************************************/

#include<stdio.h>
#include<heater.h>                  // library for heater actuator
#include<cooler.h>                  // library for cooler actuator
#include<LCD.h>                     // library for LCD

void heater_activate()              // function to heat furnace
{
    heater_enable();
    delay(5000);
}

void cooler_activate()              // function to heat furnace
{
    cooler_enable();
    delay(5000);
}

void main()
{
    // initialisation functions
    heater_init();
    cooler_init();
    LM75A_temp_init();
    LCD_init();

    int temp_val = 0;

    while(1)
    {
        LCD_Clear();                                      // clear screen on LCD
        temp_val = LM75A_read();

        if(temp_val < 29)                                   // system cooler than normal
        {
            LCD_Display("Furnace cool! Rising temperature");
            heater_activate();
            delay(1000);
        }

        else if(temp_val > 31)                              // system hotter than normal
        {
            LCD_Display("Furnace overheated! Lowering temperature");
            cooler_activate();
            delay(1000);
        }

        else                                                // system stable
        {
            LCD_Display("Furnace stable!");
        }

    }
}