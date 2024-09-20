/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_customhid.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/**Full key conflict free descriptor:
 *-----------------------------------------------------------------------------
 * Descriptor length:73Bytes
 * ____________________________________________________________________________
 *| Send Bytes Num:15Bytes                                                     |
 *| buffer[0] - bit0: Left CTRL                                                |
 *|           - bit1: Left SHIFT                                               |
 *|           - bit2: Left ALT                                                 |
 *|           - bit3: Left GUI                                                 |
 *|           - bit4: Right CTRL                                               |
 *|           - bit5: Right SHIFT                                              |
 *|           - bit6: Right ALT                                                |
 *|           - bit7: Right GUI                                                |
 *| buffer[1] - Padding = Always 0x00                                          |
 *| buffer[2] - (A & a) ~ (H & h)                                              |
 *| buffer[3] - (I & i) ~ (P & p)                                              |
 *| buffer[4] - (Q & q) ~ (X & x)                                              |
 *| buffer[5] - (Y & y) ~ (Z & z) | 1 ~ 6                                      |
 *| buffer[6] - 7 ~ 0 | Enter | Esc | Backspace | Tab                          |
 *|** This is the number key 1 ~ 0 in the main keyboard area                   |
 *| buffer[7] - Space | - | = | [ | ] | \ | \ | ;                              |
 *| buffer[8] - ' | ` | , | . | / | Cap | F1 ~ F2                              |
 *| buffer[9] - F3 ~ F10                                                       |
 *| buffer[A] - F11 ~ F12 | PRTSRC | ScrollLock | Pause | Insert | Home | PgUp |
 *| buffer[B] - Delete | End | PgDn | Right | Left | Down | Up | Lock          |
 *| buffer[C] - / | * | - | + | Enter | 1 ~ 3                                  |
 *| buffer[D] - 4 ~ 0 | .                                                      |
 *|** This is the number key 1 ~ 0 in the numeric keypad area                  |
 *| buffer[E] - (Keypad 6) ~ (Keyboard Application)                            |
 *|____________________________________________________________________________|
 *| Recv Bytes Num:1Bytes                                                      |
 *| buffer[0] - bit0: Num Lock       //States of Num Lock LED                  |
 *|           - bit1: Caps Lock      //States of Caps Lock LED                 |
 *|           - bit2: Scroll Lock    //States of Scroll Lock LED               |
 *|           - bit3: Compose        //States of Compose LED                   |
 *|           - bit4: Kana           //States of Kana LED                      |
 *|           - bit5-7: Additional LED                                         |                                              |
 *|____________________________________________________________________________|
 *-----------------------------------------------------------------------------
 **Following is the mapping from:
 *| [ASCII code characters] to [corresponding keyboard descriptors]
 * ____________________________________________________________________________
 *|        From ASCII to Keyboard descriptors Mapping Table                    |
 *| 1. This is a mapping in [half-width] + [lowercase] mode                    |
 *| 2. The upper four bits are the (buffer index:Ah[from 0 to 15]),            |
 *| 3. the highest bit of the lower four bits is                               |
 *|    whether to use the control key shift(Bh+8h/Bh+0h),                      |
 *| 4. and the remaining three bits are the                                    |
 *|    (byte numbers:Bh[from 0h to 7h])                                        |
 *|    corresponding to                                                        |
 *|    the descriptor of the ASCII character in the buffer array index         |
 *| 5. map[ASCII] = 0x(A)(B/B+8)                                               |
 *| 6. map[0]-[127] is standard mapping from ASCII to its descriptor           |
 *|    map[128]-[]  is Customize keyboard shortcuts to its descriptor:         |
 *|        (1).map[128]:Capslock's descriptor                                  |
 *|        (2).map[129]:Backspace's descriptor                                 |
 *|____________________________________________________________________________|
  uint8_t map[129]={
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x00, 0x00, 0x64, 0x00, 0x00, 0x64, 0x00, 0x00,
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x70, 0x5a, 0x88, 0x5c, 0x5d, 0x5e, 0x68, 0x80,
  		0x6a, 0x6b, 0x69, 0x7a, 0x82, 0x71, 0x83, 0x84,
  		0x63, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x60,
  		0x61, 0x62, 0x7f, 0x77, 0x8a, 0x72, 0x8b, 0x8c,
  		0x5b, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
  		0x2f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
  		0x3f, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
  		0x4f, 0x58, 0x59, 0x73, 0x75, 0x74, 0x5f, 0x79,
  		0x81, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
  		0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
  		0x37, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
  		0x47, 0x50, 0x51, 0x7b, 0x7d, 0x7c, 0x89, 0xb0,
  		0x85,     //ASCII[128]:Capslock
  		0x66,     //ASCII[129]:Backspace
  };
 *-----------------------------------------------------------------------------
 **Following are the descriptor set in Function:
 *                CUSTOM_HID_ReportDesc_FS in usbd_custom_hid_if.c
 //  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)//73U
 //  0x09, 0x06,                    // USAGE (Keyboard)
 //  0xa1, 0x01,                    // COLLECTION (Application)
 //  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
 //  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
 //  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x08,                    //   REPORT_COUNT (8)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x08,                    //   REPORT_SIZE (8)
 //  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
 //  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
 //  0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
 //  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x62,                    //   REPORT_COUNT (98)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x06,                    //   REPORT_SIZE (6)
 //  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
 //  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
 //  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
 //  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x05,                    //   REPORT_COUNT (5)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x03,                    //   REPORT_SIZE (3)
 //  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
 //  0xc0                           // END_COLLECTION
 *----------------------------------------------------------------------------
 **Following are init steps about this Project:
 * 1. usbd_conf.h: //can modify to USER CODE position
 *    #define USBD_CUSTOM_HID_REPORT_DESC_SIZE     73U
 *    #define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE    15U
 *    #define USBD_CUSTOMHID_INREPORT_BUF_SIZE     01U
 * 2. usbd_customhid.h:
 *    #ifndef USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
 *    #define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE  0x0fU //15Bytes
 *    #endif  //USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
 *    #ifndef USBD_CUSTOMHID_INREPORT_BUF_SIZE
 *    #define USBD_CUSTOMHID_INREPORT_BUF_SIZE   01U   //1Bytes
 *    #endif  //USBD_CUSTOMHID_INREPORT_BUF_SIZE
 *    #ifndef USBD_CUSTOM_HID_REPORT_DESC_SIZE
 *    #define USBD_CUSTOM_HID_REPORT_DESC_SIZE   73U   //73Bytes
 *    #endif  //USBD_CUSTOM_HID_REPORT_DESC_SIZE
 * 3. usbd_customhid.c:
 *    USB CUSTOM_HID device FS Configuration Descriptor:
 *        Descriptor of CUSTOM HID interface:
 *            0x01,  //nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
 * 4. usbd_custom_hid_if.c:
 *    Modify your code to USER CODE position:
 *    (1).Referencing external variables:
 *        extern uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];
 *        extern int InterruptFlag;
 *        extern USBD_HandleTypeDef hUsbDeviceFS;
 *    (2).Fill in custom keyboard device descriptors
 *    (3).Customize Callback function:[CUSTOM_HID_OutEvent_FS]
 *        Which deals with received data from host.
 * 5. stm32f0xx_hal_pcd.c:
 *    (1).Add these external declarations at the beginning of this file:
 *        #define USBD_CUSTOMHID_INREPORT_BUF_SIZE 1
 *        extern uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];
 *        extern int InterruptFlag;
 *        extern int NeedRollBack;
 *    (2).In function: HAL_StatusTypeDef HAL_PCD_EP_Transmit();
 *        Add following codes **before** USB_EPStartXfer function:
 *            //Determine whether to generate an interrupt:
 *            if((recv_buffer[0]&0x02) != 0x02)
 *                InterruptFlag = 1;
 *        Add following codes **after** USB_EPStartXfer function:
 *            //Output test to choose whether to RollBack:
 *            if((recv_buffer[0]&0x02) != 0x02)
 *                NeedRollBack = 1;
 *----------------------------------------------------------------------------*/
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MapLen 130
//You can adjust StrokeSlot to ensure the the correctness and stability of the output
#define StrokeSlot 35
uint8_t sent_buffer[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];

uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];

uint8_t Map[MapLen]={
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x64, 0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x70, 0x5a, 0x88, 0x5c, 0x5d, 0x5e, 0x68, 0x80,
		0x6a, 0x6b, 0x69, 0x7a, 0x82, 0x71, 0x83, 0x84,
		0x63, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x60,
		0x61, 0x62, 0x7f, 0x77, 0x8a, 0x72, 0x8b, 0x8c,
		0x5b, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
		0x2f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
		0x3f, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
		0x4f, 0x58, 0x59, 0x73, 0x75, 0x74, 0x5f, 0x79,
		0x81, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
		0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
		0x37, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
		0x47, 0x50, 0x51, 0x7b, 0x7d, 0x7c, 0x89, 0xb0,
		0x85, 0x66
};

int InterruptFlag = 0;
int InterruptCnt = 0;
int PrintCnt = 0;
int NeedRollBack = 0;
char AttackStr[256];

void Get_Descriptor(uint8_t ascii);
void SimulateKeyPress(uint8_t ascii);
void SimulateKeyRelease();
void SimulateKeyStroke(uint8_t ascii);
void SimulateKeyStrokes(char *str, int len, int *cntNow);
void PrintRecvBuf(uint8_t Recv_Buf[USBD_CUSTOMHID_INREPORT_BUF_SIZE]);
void InitKeyboardStatus();
void Convert2CapsMap(uint8_t LowerCaseMap[MapLen]);
void InterruptTrap(int *InterruptFlag);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
  Convert2CapsMap(Map);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int flag = 1;
  while(1){
	  InterruptTrap(&InterruptFlag);

	  if(flag == 1){
		  HAL_Delay(500);

		  strcpy(AttackStr, "!@#$%^&*()_+1234567890~`{}|:\"<>?[];',./ashdahskdhasjdeuwhuASDJDHJAJKDHBSXAHE\n");
		  SimulateKeyStrokes(AttackStr, strlen(AttackStr), &PrintCnt);
		  flag = 0;
	  }


  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Get_Descriptor(uint8_t ascii){
	memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	uint8_t pos = Map[ascii];
	sent_buffer[(uint8_t)(pos>>4)] |= (1<<((uint8_t)(pos&0x07)));
	if((pos&0x08) == 8)
		sent_buffer[0] |= 0x02;
}

void SimulateKeyPress(uint8_t ascii){
    //get key:ascii Descriptor
    Get_Descriptor(ascii);
    //Ensure that this instruction is executed in uppercase environment.
//    while((recv_buffer[0]&0x02) != 0x02)
//    	HAL_Delay(1);
    //Sent Descriptor report
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    if((recv_buffer[0]&0x02) != 0x02)
    	NeedRollBack = 1;
}

void SimulateKeyRelease(){
    //set 0
	memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
    //Sent Descriptor report
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
}

void SimulateKeyStroke(uint8_t ascii){
	SimulateKeyPress(ascii);
    HAL_Delay(StrokeSlot); //Wait StrokeSlot time
    SimulateKeyRelease();
    HAL_Delay(StrokeSlot); //Wait StrokeSlot time
}

void SimulateKeyStrokes(char *str, int len, int *cntNow){
    for(; *cntNow < len; (*cntNow)++){
    	//Ensure func SimulateKeyStroke is executed in uppercase environment.
    	if((recv_buffer[0]&0x02) != 0x02){
    		SimulateKeyStroke(128);
    	}
    	SimulateKeyStroke(str[*cntNow]);
    	//Determine if RollBack is necessary
    	if(NeedRollBack == 1){
    		NeedRollBack = 0;
    		(*cntNow)--;
    		SimulateKeyStroke(129);
    	}
    }
}

void PrintRecvBuf(uint8_t Recv_Buf[USBD_CUSTOMHID_INREPORT_BUF_SIZE]){
	HAL_Delay(StrokeSlot);
	for(int i = 0; i < USBD_CUSTOMHID_INREPORT_BUF_SIZE; i++){
		for(int j = 0; j < 8; j++){
			SimulateKeyStroke(((Recv_Buf[i]&(uint8_t)(0x01<<j))>>j)+'0');
		}
	}
	SimulateKeyStroke('\n');
}

void InitKeyboardStatus(){//Convert keyboard to uppercase mode
	SimulateKeyStroke(128);
	HAL_Delay(StrokeSlot);
	if((recv_buffer[0]&0x02) != 0x02){
		SimulateKeyStroke(128);
		PrintRecvBuf(recv_buffer);  //print Keyboard LED Status
	}
}

void Convert2CapsMap(uint8_t LowerCaseMap[MapLen]){
	for(uint8_t cnt = 'A'; cnt <= 'Z'; cnt++){
		LowerCaseMap[cnt] &= 0xf7;
		LowerCaseMap[cnt+'a'-'A'] |= 0x08;
	}
}

void InterruptTrap(int *InterruptFlag){
	if(*InterruptFlag == 1){
		*InterruptFlag = 0;
		//Trigger timer interrupt immediately by setting the value of the register
		TIM2->EGR |= TIM_EGR_UG;
		//Second entry interrupt
		MX_TIM2_Init();
		HAL_TIM_Base_Start_IT(&htim2);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
		if(InterruptCnt == 0){
			Get_Descriptor(128);
			USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		}else if(InterruptCnt == 1){
			SimulateKeyRelease();
			if((recv_buffer[0]&0x02) != 0x02)
				InterruptCnt = -1;
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		}else{
			TIM2->CR1 &= ~TIM_CR1_CEN;
		}
		InterruptCnt = (InterruptCnt+1)%3;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_6){//Key_Pin
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	}
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
