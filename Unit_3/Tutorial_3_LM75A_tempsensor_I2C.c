/*
Tutorial to interface LM75A temperature sensor over I2C
The project uses STM32F407 board to interface the LM75A temperature sensor. 
The I2C communication interface is used. The I2C peripheral used on the board is I2C-1 peripheral

SCL = PB6
SDA = PB7
*/

#include "main.h"
#include<stdio.h>

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);


// Initialisation function for I2C handler. Returns 0 for successful initialization. Returns 1 for unsuccessful initialization
uint32_t I2C_Init(void)			
{
	if(HAL_I2C_Init(&hi2c1) == HAL_OK)
	{
		HAL_Delay(500);
		return 0;
	}
	else
	{
		return 1;
	}
}


// Returns 1 to indicate that START bit has been transmitted
uint32_t I2C_Start(void)			
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_Delay(500);
	
	return 0;
}


// Returns 1 to indicate that STOP bit has been transmitted
uint32_t I2C_Stop(void)			
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(500);
	
	return 0;
}


// Transforms the 7 bit I2C device address into 8 bit address holding the R/W_bar bit
// For READ, dir = 1
// For WRITE, dir = 0
uint32_t I2C_Addr(uint8_t addr, uint8_t dir)
{
	uint32_t addr_complete = (addr << 1) + dir;		// transforming 7 but device address into 8 bit device address to hold direction information
	return addr_complete;
}


// Function used to write to slave device over I2C. Internally uses HAL_I2C_Master_Transmit() driver API
uint32_t I2C_Write(uint8_t byte)
{
	uint32_t address = I2C_Addr(0x01, 0);
	
	I2C_Start();
	
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)address, &byte, sizeof(byte), HAL_MAX_DELAY) == HAL_OK)
	{
		I2C_Stop();
		HAL_Delay(500);
		return 0;
	}
	
	else
	{
		I2C_Stop();
		HAL_Delay(500);
		return 1;
	}
}


// Function used to read from slave device over I2C. Internally uses HAL_I2C_Master_Receive() driver API
uint32_t I2C_Read(uint8_t ack, uint8_t *byte)
{
	uint32_t address = I2C_Addr(0x01, 1);
	
	I2C_Start();
	
	if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)address, byte, sizeof(byte), HAL_MAX_DELAY) == HAL_OK)
	{
		I2C_Stop();
		HAL_Delay(500);
		return 0;
	}
	
	else
	{
		I2C_Stop();
		HAL_Delay(500);
		return 1;
	}
}


uint8_t temperature;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    while (1)
    {
        uint32_t read_status = I2C_Read(1, &temperature);

        if(read_status == 0)
        {
            printf("I2C read is successful!/n");
            printf("Temperature = %d", temperature);
            
            HAL_Delay(1000);
        }

        else
        {
            printf("I2C read is unsuccessful :-/");
            HAL_Delay(1000);
        }


    }

}

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}


static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}


void Error_Handler(void)
{
    while (1)
    {

    }

}


