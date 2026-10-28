/**
 * @file application.c
 * @author Serialist (ba3pt@qq.com)
 * @brief 
 * @version 0.1.0
 * @date 2026-10-28
 * 
 * @copyright Copyright (c) Serialist 2026
 * 
*/

#include "application.h"
#include "main.h"
#include "motor.h"
#include "serve.h"

struct Command {
    float vx, vy, vw;
    float angle1, angle2;
} cmd;

void Command_Task(void); // 获取指令任务
void Chassis_Task(void); // 底盘控制任务
void Arm_Task(void);     // 控制机械臂任务

// =============================================================================================
// ================================           总任务            ================================
// =============================================================================================

void Application_Task(void) {
    Motor_Init();
    Serve_Init();

    for (;;) {
        Command_Task(); // 获取指令任务
        Chassis_Task(); // 控制底盘任务
        Arm_Task();     // 控制机械臂任务
        HAL_Delay(10);
    }
}

// =============================================================================================
// ================================           各自任务           ================================
// =============================================================================================

// 从遥控器获取数据，解析指令
void Command_Task(void) {
    cmd.vx = 0;
    cmd.vy = 0;
    cmd.vw = 0;
}

// mecanum 解算
// 输入 x, y, w
// 输出 四个轮
void Chassis_Task(void) {
    Motor1_Set(cmd.vx + cmd.vy + cmd.vw);
    Motor2_Set(cmd.vx - cmd.vy + cmd.vw);
    Motor3_Set(-cmd.vx - cmd.vy + cmd.vw);
    Motor4_Set(-cmd.vx + cmd.vy + cmd.vw);
}

// 机械臂运动
// 输入遥控器给出的目标角度
// 输出舵机占空比
void Arm_Task(void) {
    Serve1_Set(cmd.angle1);
    // 其他舵机....
}
