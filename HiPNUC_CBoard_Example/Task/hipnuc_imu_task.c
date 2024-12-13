#include "hipnuc_imu_task.h"


/*����HiPNUC ���ݾ��*/
hipnuc_raw_t chassis_hipnuc_imu;

/*����HiPNUC ������*/
TaskHandle_t hipnuc_imu_taskHandle;

/*����HIPNUC �����жϷ�����*/
void vHiPNUC_Receive_IDLE_ISR(UART_HandleTypeDef* huart, hipnuc_raw_t *hipnuc_raw)
{
	
	BaseType_t xHigherPerioityTaskwoken = pdFALSE;//�Ƿ���Ҫ�л�������
	
	if( (__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE)!= RESET) )//�жϴ���dma�����Ƿ��ڿ���״̬
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);//���DMA�����жϱ�־λ
		
		HAL_UART_DMAStop(huart); //ֹͣ����dma����
		
		vTaskNotifyGiveFromISR(hipnuc_imu_taskHandle, &xHigherPerioityTaskwoken);//����ֱ��֪ͨ

		HAL_UART_Receive_DMA(huart, hipnuc_raw->buf, HIPNUC_MAX_RAW_SIZE);//��������dma����	
	}
	portYIELD_FROM_ISR(xHigherPerioityTaskwoken);//�л�������
}

/*����HiPNUC ������*/
void hipnuc_imu_task(void *argument)
{
	HiPNUC_DMA_Init(&huart1, &chassis_hipnuc_imu);

	for(;;)
	{
		/*xClearCountOnExit:pdFALSE->ֵ֪ͨ���ݼ�,:pdTrue->ֵ֪ͨ������*/
		ulTaskNotifyTake(pdTRUE, 10);
		HiPNUC_Process(&chassis_hipnuc_imu);
	}
}
