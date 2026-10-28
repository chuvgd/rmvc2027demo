/**
 * @file serve.c
 * @author Serialist (ba3pt@qq.com)
 * @brief 180度舵机，操控函数的定义
 * @version 0.1.0
 * @date 2026-10-28
 * 
 * @copyright Copyright (c) Serialist 2026
 * 
*/

#include "serve.h"
#include "tim.h"

void Serve_Init(void) {
    //  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    // 启动其他舵机的输出口的PWM
}

void Serve1_Set(float angle) {
    // 限幅
    if (angle > 180) {
        angle = 180;
    } else if (angle < 0) {
        angle = 0;
    }

    // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (angle / 180) * 2000 + 500);
}
