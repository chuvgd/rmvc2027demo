/**
 *   PS2数据定义
 *   BYTE   DATA   解释
 *   01     idle
 *   02     0x73   手柄工作模式
 *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
 *   04     data   SLCT  JOYR  JOYL  STRT   UP  RGIHT  DOWN  LEFT
 *   05     data    L2    R2    L1    R1    Y     B     A     X
 *   06     data   右边摇杆  0x00 = 左    0xff = 右
 *   07     data   右边摇杆  0x00 = 上    0xff = 下
 *   08     data   左边摇杆  0x00 = 左    0xff = 右
 *   09     data   左边摇杆  0x00 = 上    0xff = 下
 */

#ifndef PS2_H
#define PS2_H

#include <stdint.h>

// --------------------------------VAULE--------------------------------
// 变量定义PS2手柄键值数据结构体
typedef struct {
    uint8_t mode; /* 手柄的工作模式 */
                  /* Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7 */
    uint8_t btn1; /* SLCT  JOYR  JOYL  STRT   UP  RIGHT  DOWN  LEFT */
    uint8_t btn2; /*  L2    R2    L1    R1    Y     B     A     X   */

    uint8_t RJoy_LR; /* 右边摇杆  0x00 = 左    0xff = 右 */
    uint8_t RJoy_UD; /* 右边摇杆  0x00 = 上    0xff = 下 */
    uint8_t LJoy_LR; /* 左边摇杆  0x00 = 左    0xff = 右 */
    uint8_t LJoy_UD; /* 左边摇杆  0x00 = 上    0xff = 下 */

} ps2_data;

enum Btn_Info {
    // btn1
    PS2_BTN_SLCT  = 0x01,
    PS2_BTN_JOYR  = 0x02,
    PS2_BTN_JOYL  = 0x04,
    PS2_BTN_STRT  = 0x08,
    PS2_BTN_UP    = 0x10,
    PS2_BTN_RIGHT = 0x20,
    PS2_BTN_DOWN  = 0x40,
    PS2_BTN_LEFT  = 0x80,
    // btn2
    PS2_BTN_L2 = 0x01,
    PS2_BTN_R2 = 0x02,
    PS2_BTN_L1 = 0x04,
    PS2_BTN_R1 = 0x08,
    PS2_BTN_Y  = 0x10,
    PS2_BTN_B  = 0x20,
    PS2_BTN_A  = 0x40,
    PS2_BTN_X  = 0x80,
};

// --------------------------------FUNCTION-------------------------------- 函数定义
// ----------------INIT---------------- PS2初始化
void ps2_init(void);

// ----------------CONTROL----------------
void ps2_read(ps2_data* ps2Data); // PS2获取按键及摇杆数值

#define PS2_BTN1_IS(pdata, btninfo) ((pdata)->btn1 & btninfo)
#define PS2_BTN2_IS(pdata, btninfo) ((pdata)->btn2 & btninfo)
// usage:
// ps2_data ps2Data;
// if( PS2_BTN1_IS(&ps2Data, PS2_BTN_UP) )

#endif
