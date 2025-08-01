/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ENABLE_1_Pin GPIO_PIN_0
#define ENABLE_1_GPIO_Port GPIOC
#define ENABLE_2_Pin GPIO_PIN_1
#define ENABLE_2_GPIO_Port GPIOC
#define DIRECTION_1_Pin GPIO_PIN_2
#define DIRECTION_1_GPIO_Port GPIOC
#define DIRECTION_2_Pin GPIO_PIN_3
#define DIRECTION_2_GPIO_Port GPIOC
#define STEP_1_Pin GPIO_PIN_0
#define STEP_1_GPIO_Port GPIOA
#define STEP_2_Pin GPIO_PIN_1
#define STEP_2_GPIO_Port GPIOA
#define ENCODER_1_A_Pin GPIO_PIN_6
#define ENCODER_1_A_GPIO_Port GPIOA
#define ENCODER_1_B_Pin GPIO_PIN_7
#define ENCODER_1_B_GPIO_Port GPIOA
#define PWM_A_Pin GPIO_PIN_9
#define PWM_A_GPIO_Port GPIOE
#define PWM_B_Pin GPIO_PIN_11
#define PWM_B_GPIO_Port GPIOE
#define A_IN_1_Pin GPIO_PIN_12
#define A_IN_1_GPIO_Port GPIOE
#define A_IN_2_Pin GPIO_PIN_13
#define A_IN_2_GPIO_Port GPIOE
#define B_IN_1_Pin GPIO_PIN_14
#define B_IN_1_GPIO_Port GPIOE
#define B_IN_2_Pin GPIO_PIN_15
#define B_IN_2_GPIO_Port GPIOE
#define ENCODER_2_A_Pin GPIO_PIN_12
#define ENCODER_2_A_GPIO_Port GPIOD
#define ENCODER_2_B_Pin GPIO_PIN_13
#define ENCODER_2_B_GPIO_Port GPIOD
#define KEY_Pin GPIO_PIN_15
#define KEY_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
