/**
 * @file application.c
 * @author Serialist (ba3pt@qq.com)
 * @brief 
 * @version 0.2.0
 * @date 2026-10-28
 * 
 * @copyright Copyright (c) Serialist 2026
 * 
*/

#include "application.h"
#include "main.h"
#include "motor.h"
#include "ps2.h"
// #include "serve.h"

struct Command {
    float x, y, w; // 前后 / 左右 / 自转
} cmd;

ps2_data ps2_cmd;

void Command_Task(void); // 获取指令任务
void Chassis_Task(void); // 底盘控制任务
void Arm_Task(void);     // 控制机械臂任务

// =============================================================================================
// ================================           总任务            ================================
// =============================================================================================

void Application_Task(void) {
    Motor_Init();
    ps2_init();
    // Serve_Init();

    for (;;) {
        Command_Task(); // 获取指令任务
        Chassis_Task(); // 控制底盘任务
        // Arm_Task();     // 控制机械臂任务
        HAL_Delay(10);
    }
}

// =============================================================================================
// ================================           各自任务           ================================
// =============================================================================================

// 从遥控器获取数据，解析指令
void Command_Task(void) {
    ps2_read(&ps2_cmd);

    cmd.x = -((float)ps2_cmd.LJoy_UD - 0x80); // 左摇杆上推 --> 前进
    cmd.y = (float)ps2_cmd.LJoy_LR - 0x80;    // 左摇杆右推 --> 右移
    cmd.w = (float)ps2_cmd.RJoy_LR - 0x80;    // 右摇杆右推 --> 右转
}

// mecanum 解算
// 输入 x, y, w
// 输出 四个轮
void Chassis_Task(void) {
    float w1, w2, w3, w4;

    w1 = cmd.x + cmd.y + cmd.w;
    w2 = cmd.x - cmd.y + cmd.w;
    w3 = -cmd.x - cmd.y + cmd.w;
    w4 = -cmd.x + cmd.y + cmd.w;

    Motor1_Set(w1 / (0x80 * 3.0f));
    Motor2_Set(w2 / (0x80 * 3.0f));
    Motor3_Set(w3 / (0x80 * 3.0f));
    Motor4_Set(w4 / (0x80 * 3.0f));
}

// 机械臂运动
// 输入遥控器给出的目标角度
// 输出舵机占空比
void Arm_Task(void) {
    // 其他舵机....
}
