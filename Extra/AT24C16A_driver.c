/* Interfacing code for AT24C16A */

#include "main.h"

I2C_HandleTypeDef hi2c1;

void System_ClockConfig(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

void AT24C16A_Read(uint8_t page, uint8_t word_addr);
void AT24C16A_Write(uint8_t page, uint8_t word_addr, uint8_t data_array[]);

void main()
{
    HAL_Init();

    System_ClockConfig(void);
    MX_GPIO_Init(void);
    MX_I2C1_Init(void);

    // To store the string "Embedded Systems Design Electronics and Communication Engineering"

    data_array[7] = {"Embedded", "Systems", "Design", "Electronics", "and", "Communication", "Engineering"};
    uint8_t page = 4;
    uint8_t memory_addr = 32                // Address of 0x20

    AT24C16A_Write(page, memory_addr, data_array);

    while(1)
    {
        // Do nothing
    }
}

void AT24C16A_Read(uint8_t page, uint8_t word_addr)
{
    uint8_t addr_write = ((1010 << 4) | (page << 1) | 0);

    // transmitting the word address data into AT24C16A for initial dummy write as per the timing diagram
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)addr_write, &word_addr, sizeof(word_addr), 0xFFFF);

    // Again passing device address to read data n from the device
    uint8_t addr_read = ((1010 << 4) | (page << 1) | 1);

    // Receiving the data from address
    char text[];
    HAL_I2C_Master_Receive(&hi2c1, (uint16_t)addr_read, &text, sizeof(text), 0xFFFF);

    printf("Data from memory : %s", text);
}

void AT24C16A_Write(uint8_t page, uint8_t word_addr, uint8_t data_array[])
{
    uint8_t addr_write = ((1010 << 4) | (page << 1) | 0);

    // Writing the address onto memory
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)addr_write, &word_addr, sizeof(word_addr), 0xFFFF);

    // Writing the array of data elements onto the location
    for(int i = 0; i < (sizeof(data_array) / sizeof(data_array[0])), i++)
    {
        HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)word_addr, &data_array[i], sizeof(data_array[i]), 0xFFFF);
    }

    printf("Successfully dumped data onto desired memory");
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
}

static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.OwnAddress2 = 0;
    . .
    . .
    . .
}
