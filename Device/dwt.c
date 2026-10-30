#include "dwt.h"

#include "main.h"

static uint32_t cycles_per_us;

void dwt_init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 打开 DWT
    DWT->CYCCNT       = 0;
    DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;           // 开始计数
    cycles_per_us     = HAL_RCC_GetHCLKFreq() / 1000000U; // 72MHz -> 72
}

void dwt_delay_us(uint32_t us) {
    uint32_t start;
    uint32_t ticks;

    // 没调用过 dwt_init 的话 cycles_per_us 是 0，延时会被静默跳过，这里兜底
    if (cycles_per_us == 0U) {
        dwt_init();
    }

    start = DWT->CYCCNT;
    ticks = us * cycles_per_us;
    while ((DWT->CYCCNT - start) < ticks) {}
}

void dwt_delay_ms(uint32_t ms) {
    while (ms--)
        dwt_delay_us(1000);
}
