#include "hipnuc_demo.h"


hipnuc_raw_t hipnuc;
extern int decode_hipnuc(hipnuc_raw_t *raw);
extern uint16_t U2(uint8_t *p);
void hipnuc_Init(void)
{

	/*开启dma空闲中断接收*/
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1,hipnuc.buf,HIPNUC_MAX_RAW_SIZE);
}

/**
  * @func			void IDLE_IMU_Handler(void)
  * @brief          JY901接收空闲中断处理
  * @brief          该函数在stm32f4xx_it.c中被相应串口中断处理函数调用
  * @param[in]      none
  * @retval         none
  */

void IDLE_IMU_Handler(void)
{
	
	int32_t Data_lave,Data_exist; 
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!= RESET))//判断串口dma接收是否处于空闲状态
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除DMA空闲中断标志位
		
		HAL_UART_DMAStop(&huart1); //停止串口dma接收
		
		Data_lave = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx); //获取接收长度
		
		
		Data_exist = HIPNUC_MAX_RAW_SIZE - Data_lave; //计算剩余长度

		if(Data_exist >= 0 )  //判断是否接收到正确长度数据
		{
			/*接收成功后，处理相应接收数据*/
			hipnuc.len = U2(hipnuc.buf + 2);
			decode_hipnuc(&hipnuc);
		}
		HAL_UART_Receive_DMA(&huart1,hipnuc.buf,HIPNUC_MAX_RAW_SIZE);//重新启动dma接收
	}
}