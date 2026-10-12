/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l010_hal.h"


#define I2C_IRQ_LEVEL   0
#define GPIO_IRQ_LEVEL  0
#define BTIM1_IRQ_LEVEL 0

#define ULTR_TRIG_GPIO_Port CW_GPIOB
#define ULTR_TRIG_GPIO_Pin  GPIO_PIN_5
#define ULTR_ECHO_GPIO_Port CW_GPIOB
#define ULTR_ECHO_GPIO_Pin  GPIO_PIN_6



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
