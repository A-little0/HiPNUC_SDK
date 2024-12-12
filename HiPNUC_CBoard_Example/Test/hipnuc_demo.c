#include "hipnuc_demo.h"


hipnuc_raw_t hipnuc;
extern int decode_hipnuc(hipnuc_raw_t *raw);
extern uint16_t U2(uint8_t *p);
void hipnuc_Init(void)
{

	/*����dma�����жϽ���*/
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1,hipnuc.buf,HIPNUC_MAX_RAW_SIZE);
}

/**
  * @func			void IDLE_IMU_Handler(void)
  * @brief          JY901���տ����жϴ���
  * @brief          �ú�����stm32f4xx_it.c�б���Ӧ�����жϴ���������
  * @param[in]      none
  * @retval         none
  */

void IDLE_IMU_Handler(void)
{
	
	int32_t Data_lave,Data_exist; 
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!= RESET))//�жϴ���dma�����Ƿ��ڿ���״̬
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//���DMA�����жϱ�־λ
		
		HAL_UART_DMAStop(&huart1); //ֹͣ����dma����
		
		Data_lave = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx); //��ȡ���ճ���
		
		
		Data_exist = HIPNUC_MAX_RAW_SIZE - Data_lave; //����ʣ�೤��

		if(Data_exist >= 0 )  //�ж��Ƿ���յ���ȷ��������
		{
			/*���ճɹ��󣬴�����Ӧ��������*/
			hipnuc.len = U2(hipnuc.buf + 2);
			decode_hipnuc(&hipnuc);
		}
		HAL_UART_Receive_DMA(&huart1,hipnuc.buf,HIPNUC_MAX_RAW_SIZE);//��������dma����
	}
}