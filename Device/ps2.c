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

/**
 * PB5 -> DATA
 * PB6 -> CMD
 * PB7 -> CS
 * PB8 -> CLK
 */

#include "ps2.h"
#include "dwt.h"
#include "gpio.h"

// --------------------------------DEFINE-------------------------------- 参数定义
// delay
#define DELAY_US(us) dwt_delay_us(us)
// PS2手柄的输入输出口
#define DI() (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) // 数据输入引脚

#define CMD_H() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)   // 命令位高
#define CMD_L() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET) // 命令位低

#define CS_H() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)   // CS拉高(别名ATT)
#define CS_L() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET) // CS拉低 (别名ATT)

#define CLK_H() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)   // 时钟高
#define CLK_L() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET) // 时钟低

// --------------------------------VAULE-------------------------------- 变量定义

// --------------------------------FUNCTION-------------------------------- 函数定义
// ----------------INIT---------------- PS2初始化
void ps2_init(void) {
    dwt_init();
    // 关闭PS2手柄使能
    CS_H();
}

// ----------------CONTROL----------------
/**
 * @brief PS2通信
 *
 * @param send_data 发送数据
 * @return uint8_t 接收数据
 *
 * @note  通信过程中，先发送数据，再读取数据
 */
uint8_t ps2_comm(uint8_t sendData) {
    volatile uint8_t res = 0;
    volatile uint8_t ref;

    // 写入命令，并读取一个1字节数据
    for (ref = 0x01; ref > 0x00; ref <<= 1) {
        ////输出一位数据
        if (ref & sendData)
            CMD_H();
        else
            CMD_L();

        CLK_L();
        DELAY_US(16);

        // 读取一位数据
        if (DI())
            res |= ref;
        CLK_H();
        DELAY_US(16);
    }

    // 返回读出数据
    return res;
}

// PS2获取按键及摇杆数值
void ps2_read(ps2_data* ps2Data) {
    const uint8_t ps2_cmd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 请求获取数据命令
    uint8_t       ps2_receive[9];                                                      // 接收数据的缓存

    CS_L(); // 使能手柄

    for (uint8_t i = 0; i < 9; i++) { // 读取PS2数据
        ps2_receive[i] = ps2_comm(ps2_cmd[i]);
    }

    CS_H(); // 关闭使能

    /* 手柄没插好时 MISO 一直被上拉成 0xff，照常解析的话摇杆就是满量程，
     * 底盘会直接冲出去。读到全 0xff 就当作「中位、无按键」返回。 */
    {
        uint8_t all_ff = 1;
        uint8_t i;

        for (i = 1; i < 9; i++) {
            if (ps2_receive[i] != 0xFF) {
                all_ff = 0;
                break;
            }
        }
        if (all_ff) {
            ps2Data->mode    = 0;
            ps2Data->btn1    = 0;
            ps2Data->btn2    = 0;
            ps2Data->RJoy_LR = 0x80;
            ps2Data->RJoy_UD = 0x80;
            ps2Data->LJoy_LR = 0x80;
            ps2Data->LJoy_UD = 0x80;
            return;
        }
    }

    // 数值传递
    ps2Data->mode    = ps2_receive[1];
    ps2Data->btn1    = ~ps2_receive[3];
    ps2Data->btn2    = ~ps2_receive[4];
    ps2Data->RJoy_LR = ps2_receive[5];
    ps2Data->RJoy_UD = ps2_receive[6];
    ps2Data->LJoy_LR = ps2_receive[7];
    ps2Data->LJoy_UD = ps2_receive[8];
}
