/*
        Tutorial 3 : Interface RTC DS1307 with I2C

        Note : The code throws up errors at VSCode. Ignore them
*/



#include "main.h"
#include<stdio.h>

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

// array to store current time and date after burst read operation

uint8_t read_data[7];

// Variable to write current time to DS1307 to set it to 3PM on 1/Jan/2000

uint8_t current_time[2] = {0x02, 0x63};

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();

    // Setting the current time as 3Pm on 1/Jan/2000 to RTC
		
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0xD0, current_time, sizeof(current_time), HAL_MAX_DELAY);
	
	// Generating a delay of 5 minutes
		
	HAL_Delay(300000);

    while (1)
    {
        // Performing a burst read from the device
		
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[0], 1, HAL_MAX_DELAY);		// seconds
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[1], 1, HAL_MAX_DELAY);		// minutes
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[2], 1, HAL_MAX_DELAY);		// hours
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[3], 1, HAL_MAX_DELAY);		// day
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[4], 1, HAL_MAX_DELAY);		// date
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[5], 1, HAL_MAX_DELAY);		// month
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0xD1, &read_data[6], 1, HAL_MAX_DELAY);		// year

        printf("Date : %d", read_data[4]);
		printf("\n");
		
		printf("Month : %d", read_data[5]);
		printf("\n");
		
		printf("Year : %d", read_data[6]);
		printf("\n");
		
		printf("Hours : %d", read_data[2]);
		printf("\n");
		
		printf("Minutes : %d", read_data[1]);
		printf("\n");
		
		printf("Seconds : %d", read_data[0]);
		printf("\n");

        // Generating a delay of 5 minutes for performing next read_data
		
		HAL_Delay(300000);
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

  /* GPIO Ports Clock Enable */
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