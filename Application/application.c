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
    float x, y, w;    // 前后 / 左右 / 自转
    int   start_flag; // 0: 停止 1: 启动
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

    if (PS2_BTN1_IS(&ps2_cmd, PS2_BTN_STRT)) {
        HAL_Delay(50); // 按键去抖
        if (PS2_BTN1_IS(&ps2_cmd, PS2_BTN_STRT)) {
            cmd.start_flag = !(cmd.start_flag); // 如果按下开始键，切换启动/停止状态
        }
    }

    // 如果启动，就获取指令
    if (cmd.start_flag) {
        cmd.x = -((float)ps2_cmd.LJoy_UD - 0x80); // 左摇杆上推 --> 前进
        cmd.y = ((float)ps2_cmd.LJoy_LR - 0x80);  // 左摇杆左推 --> 左移
        cmd.w = ((float)ps2_cmd.RJoy_LR - 0x80);  // 右摇杆左推 --> 左转
    } else {
        // 如果停止，就停止所有运动
        cmd.x = cmd.y = cmd.w = 0;
    }
}

float       w1, w2, w3, w4;
const float kw = 1.5;

// mecanum 解算
// 输入 x, y, w
// 输出 四个轮

void Chassis_Task(void) {
    w1 = (cmd.x + cmd.y + kw * cmd.w) / 128.f;
    w2 = (cmd.x - cmd.y + kw * cmd.w) / 128.f;
    w3 = (-cmd.x - cmd.y + kw * cmd.w) / 128.f;
    w4 = (-cmd.x + cmd.y + kw * cmd.w) / 128.f;

    Motor1_Set(w1);
    Motor2_Set(w2);
    Motor3_Set(w3);
    Motor4_Set(w4);
}

// 机械臂运动
// 输入遥控器给出的目标角度
// 输出舵机占空比
void Arm_Task(void) {
    // 其他舵机....
}
