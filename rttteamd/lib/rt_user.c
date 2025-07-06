#include "rtthread.h"
#include "stm32f4xx_hal.h"
#include "upacker.h"
#include "usart.h"
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread led1_thread;
static rt_uint8_t rt_led1_thread_stack[1024];

static void led1_thread_entry(void* parameter);
//upacker_____________________________________________________________start
upacker_inst msg_packer;


#define RX_BUF_SIZE 30
uint8_t rx_buf[RX_BUF_SIZE]; // 全局接收缓冲区

/**
  * @brief  串口dma发送接口
  * @note   
  * @param  *d: 
  * @param  size: 
  * @retval None
  */
static void uart_send(uint8_t *d, uint16_t size)
{
  
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

static void handle_cb(uint8_t *d, uint16_t size)
{
   
	
		//	HAL_UART_Transmit(&huart2,"okwwwww",7,2000);
		  HAL_UART_Transmit_DMA(&huart2,"back_ok",7);
	  
}
//upacker_____________________________________________________________end




void main_init()
{
	  //绑定函数
	    upacker_init(&msg_packer, handle_cb, uart_send);
	 
	  //开启空闲中断dma
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buf, RX_BUF_SIZE);
	 // 关闭过半中断
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT); 
	
	//demo
	      uint8_t buff[2];
			  buff[0] = 0x88;
        buff[1] = 0x89;
        upacker_pack(&msg_packer,(uint8_t *)buff, 2);



}




void rt_process_init()
{
	    main_init();
	
	
	
	    rt_thread_init(&led1_thread,                 /* �߳̿��ƿ� */
                "led1",                       /* �߳����� */
                led1_thread_entry,            /* �߳���ں��� */
                RT_NULL,                      /* �߳���ں������� */
                &rt_led1_thread_stack[0],     /* �߳�ջ��ʼ��ַ */
                sizeof(rt_led1_thread_stack), /* �߳�ջ��С */
                3,                            /* �̵߳����ȼ� */
                20);                          /* �߳�ʱ��Ƭ */
	
	

}

void rt_process_entry()
{

    rt_thread_startup(&led1_thread);             /* �����̣߳��������� */

}





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
			
		
			
		//HAL_UART_Transmit_DMA(&huart2,"123",3);
    }
}


//
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



