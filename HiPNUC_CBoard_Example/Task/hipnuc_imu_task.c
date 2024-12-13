#include "hipnuc_imu_task.h"


/*创建HiPNUC 数据句柄*/
hipnuc_raw_t chassis_hipnuc_imu;

/*创建HiPNUC 任务句柄*/
TaskHandle_t hipnuc_imu_taskHandle;

/*创建HIPNUC 空闲中断服务函数*/
void vHiPNUC_Receive_IDLE_ISR(UART_HandleTypeDef* huart, hipnuc_raw_t *hipnuc_raw)
{
	
	BaseType_t xHigherPerioityTaskwoken = pdFALSE;//是否需要切换上下文
	
	if( (__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE)!= RESET) )//判断串口dma接收是否处于空闲状态
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);//清除DMA空闲中断标志位
		
		HAL_UART_DMAStop(huart); //停止串口dma接收
		
		vTaskNotifyGiveFromISR(hipnuc_imu_taskHandle, &xHigherPerioityTaskwoken);//任务直达通知

		HAL_UART_Receive_DMA(huart, hipnuc_raw->buf, HIPNUC_MAX_RAW_SIZE);//重新启动dma接收	
	}
	portYIELD_FROM_ISR(xHigherPerioityTaskwoken);//切换上下文
}

/*创建HiPNUC 任务句柄*/
void hipnuc_imu_task(void *argument)
{
	HiPNUC_DMA_Init(&huart1, &chassis_hipnuc_imu);

	for(;;)
	{
		/*xClearCountOnExit:pdFALSE->通知值将递减,:pdTrue->通知值将置零*/
		ulTaskNotifyTake(pdTRUE, 10);
		HiPNUC_Process(&chassis_hipnuc_imu);
	}
}
