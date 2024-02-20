/*
--------------------------------BRIEF OF EXPERIMENT---------------------------------------

The below code implements a simple experiment of communication between two CAN nodes. The nodes are labelled N1 and N2. 

The two nodes N1 and N2 can send data frames to each other. 
N1 sends a data frame to N2, indicating information as to which LED has to be glown on N2. N2 upon receiving data frame from N1, glows the corresponding LED.  
N1 also sends a remote frame to N2 requesting for 2 bytes of data. The remote frame is sent periodically by using TIM6
Upon receiving remote frame from N1, N2 then sends a dataframe to N1 comprising of two bytes {0xAB, 0xCD}


*/

#include "main.h"
#include <string.h>
#include <stdio.h>

// Declaring the function prototypes
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN1_Filter_Config(void);
void send_response(uint32_t std_id);
void LED_output(uint8_t led_no);

CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htim6;

CAN_RxHeaderTypeDef RxHeader;
uint8_t req_counter = 0;
uint8_t led_no;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM6_Init(void);


/*-----------------------CALLBACK FUNCTIONS TO HANDLE INTERRUPT BASED EVENTS FOR CAN------------------------*/

// callback function to trigger interrupt if TxMailBox-0 is empty
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    printf("Message transmitted from TxMailBox-0\n");
}

// callback function to trigger interrupt if TxMailBox-1 is empty
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    printf("Message transmitted from TxMailBox-1\n");
}

// callback function to trigger interrupt if TxMailBox-2 is empty
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    printf("Message transmitted from TxMailBox-2\n");
}

// callback function to get received message and process
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rcvd_msg[8];

    // Checking if the received frame is valid
    if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, rcvd_msg) != HAL_OK)
    {
        Error_Handler();
    }

    // processing data frame sent from N1 to N2
    if(RxHeader.StdId == 0x65D && RxHeader.RTR == 0)        // dataframe sent from N1 to N2 has the ID = 0x65D
    {
        LED_output(rcvd_msg[0]);
        printf("Message received : %d", rcvd_msg[0]);
    }

    // processing remote frame sent from N1 to N2           // remote frame sent from N1 to N2 has the ID = 0x651
    else if(RxHeader.StdId == 0x651 && RxHeader.RTR == 1)
    {
        send_response(RxHeader.StdId);
        return;
    }

    // processing data frame sent from N2 to N1             // dataframe sent from N2 to N1 has the ID = 0x651
    else if(RxHeader.StdId == 0x651 && RxHeader.IDE == 0)
    {
        printf("Message received : %d", rcvd_msg[0] << 8 | rcvd_msg[1]);        // print 0xAB, 0xCD to the console
    }

}

// Callback to generate interrupt when timer elapses
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t Txmailbox;
    uint8_t message;

    // Initializing the remote frame that N1 sends to N2 to request 2 bytes of data from N2
    if(req_counter == 4)
    {
        // N1 sending remote frame to N2
        TxHeader.DLC = 2; 		//demanding 2 bytes of reply
        TxHeader.StdId = 0x651;
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_REMOTE;

        if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &Txmailbox) != HAL_OK)
        {
            Error_Handler();
        }

        req_counter = 0;
    }

    else
    {
        CAN1_Tx();
        req_counter++;
    }
}


/*----------------------------------------------- MAIN AND OTHER USER DEFINED FUNCTIONS--------------------------------*/

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_CAN1_Init();
    MX_TIM6_Init();

    CAN1_Filter_Config();

    if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF) != HAL_OK)       // function to activate CAN interrupt functions
    {
        Error_Handler();
    }
        
    if(HAL_CAN_Start(&hcan1) != HAL_OK)     // starts the CAN peripheral
    {
        Error_Handler();
    }

    while (1)
    {
        // Empty while loop
    }

    return 0;

}

// Function to send dataframe from N1 to N2 to glow LED on N2
void CAN1_Tx(void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t Txmailbox;
    uint8_t message;

    // Defining the parameters for TxHeader
    TxHeader.StdId = 0x65D;
    TxHeader.DLC = 1;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;

    message = ++led_no;

    if(led_no == 4)
    {
        led_no = 0;
    }

    if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &Txmailbox) != HAL_OK)         // Adds CAN message to the mailbox scheduled for transmission upon win in arbitration
    {
        Error_Handler();
    }
}

// Function to configure CAN receiver filter (write first few lines)
void CAN1_Filter_Config(void)
{
    CAN_FilterTypeDef can1_filter_init;

    // Initializing the filter variables
    can1_filter_init.FilterActivation = CAN_FILTERMODE_IDMASK;
    can1_filter_init.FilterBank = 0;
    can1_filter_init.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can1_filter_init.FilterIdHigh = 0x0000;
    can1_filter_init.FilterIdLow = 0x0000;
    can1_filter_init.FilterMaskIdHigh = 0x01C0;
    can1_filter_init.FilterMaskIdLow = 0x0000;
    can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

    // configuring the filter
    if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
    {
        Error_Handler();
    }
}

// Function to initialise dataframe to be sent from N2 to N1 upon remote request. Comprises of two bytes of data {0xAB, 0xCD}
void send_response(uint32_t std_id)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t Txmailbox;
    uint8_t response[2] = {0xAB, 0xCD};

    TxHeader.DLC = 2;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.StdId = std_id;
    TxHeader.RTR = CAN_RTR_DATA;

    if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, response, &Txmailbox) != HAL_OK)
    {
        Error_Handler();
    }
}

// Function to glow corresponding LED on N2
void LED_output(uint8_t led_no)
{
    switch(led_no)
    {
        case 1:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

        case 2:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

        case 3:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

        case 4:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
    }
}

// CAN1 initialization function (write first few lines)
static void MX_CAN1_Init(void)
{
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 16;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
}


// TIM6 initialization function (write first few lines)
static void MX_TIM6_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 10000;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 1000;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

}

// GPIO initialization function (write first few lines)
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOD, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin, GPIO_PIN_RESET);


    GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}




