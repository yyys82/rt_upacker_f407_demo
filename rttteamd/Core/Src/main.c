/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "board.h"
#include "rtthread.h"
#include "upacker.h"
upacker_inst msg_packer;
uint16_t state;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/**
  * @brief  串口dma发送接口
  * @note   
  * @param  *d: 
  * @param  size: 
  * @retval None
  */
static void uart_send(uint8_t *d, uint16_t size)
{
   // dbuff_push(&uart3_dbuff, d, size);
		HAL_UART_Transmit(&huart2,d,size,1000);
 // HAL_UART_Transmit_DMA(&huart2,d,size);
}


/**
  * @brief  消息解析回调
  * @note   
  * @param  *d: 
  * @param  size: 
  * @retval None
  */
uint16_t a;
static void handle_cb(uint8_t *d, uint16_t size)
{
    //接收到payload
    //rt_kprintf("pack len%d", size);
	
		//	HAL_UART_Transmit(&huart2,"okwwwww",7,2000);
		  HAL_UART_Transmit_DMA(&huart2,"okwwwww",7);
	    a=d[0];
}


//init packer













/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




/* �����߳̿��ƿ� */
static struct rt_thread led1_thread;

/* �����߳̿�ջʱҪ��RT_ALIGN_SIZE���ֽڶ��� */
ALIGN(RT_ALIGN_SIZE)
/* �����߳�ջ */
static rt_uint8_t rt_led1_thread_stack[1024];


static void led1_thread_entry(void* parameter);


#define RX_BUF_SIZE 30
uint8_t rx_buf[RX_BUF_SIZE]; // 全局接收缓冲区



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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    rt_thread_init(&led1_thread,                 /* �߳̿��ƿ� */
                "led1",                       /* �߳����� */
                led1_thread_entry,            /* �߳���ں��� */
                RT_NULL,                      /* �߳���ں������� */
                &rt_led1_thread_stack[0],     /* �߳�ջ��ʼ��ַ */
                sizeof(rt_led1_thread_stack), /* �߳�ջ��С */
                3,                            /* �̵߳����ȼ� */
                20);                          /* �߳�ʱ��Ƭ */
    upacker_init(&msg_packer, handle_cb, uart_send);
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT); // 关闭过半中断
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buf, RX_BUF_SIZE);
	uint8_t buff[2];
			  buff[0] = 0x88;
        buff[1] = 0x89;
        upacker_pack(&msg_packer,(uint8_t *)buff, 2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		   /*
    * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
    * ����component.c�ļ��е�rtthread_startup()����������ˡ�
    * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
    */


    rt_thread_startup(&led1_thread);             /* �����̣߳��������� */
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */






 uint32_t tick;
uint16_t led;
static void led1_thread_entry(void* parameter)
{
    while (1)
    {
        //LED1_ON;
			led=100;
        rt_thread_delay(500);   /* ��ʱ500��tick */
      led=200;
       // LED1_OFF;
        rt_thread_delay(500);   /* ��ʱ500��tick */
     tick=rt_tick_get();
			
		
			state++;
		//HAL_UART_Transmit_DMA(&huart2,"123",3);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance == USART2 && Size > 0)
  {
    // 1. 数据回显（DMA发送）
   // HAL_UART_Transmit_DMA(&huart2, rx_buf, Size);
		
		
		upacker_unpack(&msg_packer, (uint8_t *)rx_buf,sizeof(rx_buf)-4);
		//upacker_unpack(&msg_packer, (uint8_t *)rx_buffer, msg.size);
   // state =1;
//    // 2. 指令解析（支持任意位置包含指令）
//    if(memmem(rx_buf, Size, "ON", 2) != NULL) {
//      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//    }
//    else if(memmem(rx_buf, Size, "OFF", 3) != NULL) {
//      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//    }
    
    // 3. 重启接收（关键！）
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buf, RX_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT); // 关闭过半中断
  }
}














/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
