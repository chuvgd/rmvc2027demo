/**
 * @file motor.h
 * @author Serialist (ba3pt@qq.com)
 * @brief 声明怎么直接使用电机
 * @version 0.2.0
 * @date 2026-10-28
 * 
 * @copyright Copyright (c) Serialist 2026
 * 
*/

#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include <stdint.h>

void Motor_Init(void);

// 输入占空比为 -1~1，正负代表转向，超出范围自动限幅
void Motor1_Set(float duty);
void Motor2_Set(float duty);
void Motor3_Set(float duty);
void Motor4_Set(float duty);

#endif
