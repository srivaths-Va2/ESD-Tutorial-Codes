/*

The MCUs are used for control automation of chemical plant. A MCU is used to
control the liquid flow of blast furnace. Another MCU is used control the
temperature of blast furnace. The liquid level & temperature of blast furnace is
displayed in master room powered by another MCU.
Show the block diagram connecting different MCUs. Comment on the interface to
be used.

*/

/*
SYSTEM DESIGN

1) AS THERE ARE THREE MCUs USED, WE MUST INCORPORATE A COMMUNICATION LAYER TO INTERFACE ALL THREE MCUs TOGETHER
2) MCU_A CHECKS THE WATER FLOW LEVEL USING A FLOW SENSOR
3) MCU_B CHECKS THE TEMPERATURE OF THE PLANT USING A TEMPERATURE SENSOR
4) MCU_C IS CONNECTED TO THE CONTROL ROOM, USED TO DISPLAY THE TEMPERATURE OVER AN LCD DISPLAY
5) ALL THE THREE MCU NODES ARE CONNECTED OVER WIRED INTERFACE USING SENDER/TARGET INFRASTRUCTURE (UART WITH ADDITIONAL CAPABILITIES OF MENTIONING SENDER AND TARGET DEVICES)

*/

/***************************************** CODE ************************************************/

#include<stdio.h>
#include<flow.h>
#include<LM75A.h>
#include<LCD.h>
#include<gate.h>
#include<heater.h>
#include<cooler.h>

// defining the temperature and flow thresholds for stable range
#define FLOW_LOWER_THRESHOLD    50
#define FLOW_UPPER_THRESHOLD    200
#define TEMP_LOWER_THRESHOLD    25
#define TEMP_UPPER_THRESHOLD    60


MCU_Handle_t mcu_a, mcu_b, mcu_c;               // handlers to denote different MCU notes in distributed system

// function to read flow value and adjust flow in plant 
void flow_read(*MCU_Handle_t mcu)
{
    int flow_val = flow_sensor_read();
    data_comm_send(&mcu_a, &mcu_c, flow_val);                   // sending flow data from mcu_a to mcu_c for display on LCD

    if(flow_val < FLOW_LOWER_THRESHOLD)
    {
        gate_open();                                            // opening gate to increase flow
        delay(5000);
    }

    else(flow_val > FLOW_UPPER_THRESHOLD)
    {
        gate_close();                                           // closing gate to decrease flow
        delay(5000);
    }
}

// function to read temperature value and adjust temperature in plant
void temp_read(*MCU_Handle_t mcu))
{
    int temp_val = LM75A_temp_read();
    data_comm_send(&mcu_b, &mcu_c, temp_val);                                   // sending flow data from mcu_a to mcu_c for display on LCD

    if(temp_val < TEMP_LOWER_THRESHOLD)
    {
        heater_enable();                                                       // enabling heater to increase temperature
        delay(5000);
    }

    else(temp_val > TEMP_UPPER_THRESHOLD)
    {
        cooler_enable();                                                       // enabling cooler to decrease temperature
        delay(5000);
    }
}

// function to display temperature and flow on LCD in runtime
void LCD_monitor(*MCU_Handle_t mcu)
{
    int flow_val = data_comm_rcv(&mcu_a, &mcu_c);                           // receiving flow data from mcu_a to mcu_c for display on LCD
    int temp_val = data_comm_rcv(&mcu_b, &mcu_c);                          // receiving temperature data from mcu_b to mcu_c for display on LCD    

    LCD_Diplay(0, 0, flow_val);
    LCD_Diplay(1, 0, temp_val);

    delay(2000);
}

void main()
{
    flow_sensor_init();
    LM75A_init();
    LCD_init();

    int flow_value = 0;
    int temp_value = 0;

    while(1)
    {
        flow_read(&mcu_a);
        temp_read(&mcu_b);
        LCD_monitor(&mcu_c);
    }
}


