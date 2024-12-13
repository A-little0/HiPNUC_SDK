#ifndef __HIPNUC_IMU_TASK_H 
#define __HIPNUC_IMU_TASK_H

#include "hipnuc_dec.h"
#include "FreeRTOS.h"
#include "task.h"

/*����HiPNUC ���ݾ��*/
extern hipnuc_raw_t chassis_hipnuc_imu;

/*����HiPNUC ������*/
extern TaskHandle_t hipnuc_imu_taskHandle;

/*����HIPNUC �����жϷ�����*/
void vHiPNUC_Receive_IDLE_ISR(UART_HandleTypeDef* huart, hipnuc_raw_t *hipnuc_raw);
void hipnuc_imu_task(void *argument);


#endif
