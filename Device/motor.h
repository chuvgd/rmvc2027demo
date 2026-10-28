/**
 * @file motor.h
 * @author Serialist (ba3pt@qq.com)
 * @brief 声明怎么直接使用电机
 * @version 0.1.0
 * @date 2026-10-28
 * 
 * @copyright Copyright (c) Serialist 2026
 * 
*/

#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include <stdint.h>

void Motor_Init(void);

void Motor1_Set(int32_t v);
void Motor2_Set(int32_t v);
void Motor3_Set(int32_t v);
void Motor4_Set(int32_t v);

#endif
