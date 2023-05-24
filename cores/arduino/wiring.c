#include <Arduino.h>

static volatile unsigned long currTime = 0;
static unsigned long sysclock_div_1000 = 0;
static unsigned long sysclock_div_1000000 = 0;
void init() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    /* setup sysclock */
    SysTick->CTLR = 0;
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR = 0;
    SysTick->CNT = 0;
    /* 1000 Hz interrupts (aka once per millisecond) */
    sysclock_div_1000 = SystemCoreClock / 1000ul;
    sysclock_div_1000000 = SystemCoreClock / 1000000ul;
    SysTick->CMP = (SystemCoreClock / 1000ul) -1;;
    /* counter enable, counter interupt enable, counter clock source: HCLK */
    SysTick->CTLR = 0b0111;
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void SysTick_Handler(void) {
    currTime++;
    SysTick->SR = 0; // clear IRQ
    // advance to next compare
    SysTick->CMP += sysclock_div_1000;
}

unsigned long millis()
{
    return currTime;
}

unsigned long micros()
{
    uint32_t m0 = currTime;
    // get the number of systicks that happened since the previous compare
    // then convert that to microseconds and add on top of current millis
    // CNT - (CMP - sysclock_div_100) is equivalent to this
    uint32_t ticks_since_last_cmp = (SysTick->CNT + sysclock_div_1000) - SysTick->CMP;
    return m0 * 1000u + ticks_since_last_cmp / sysclock_div_1000000;
}

void delay(unsigned long ms)
{
    uint64_t targend = SysTick->CNT + ms*sysclock_div_1000;
    while( ((int64_t)( SysTick->CNT - targend )) < 0 );
}

/* number of cycles it takes to compute targend, empircally determined, in -Os mode, for CH32V307. */
#define CORRECTION_TERM_CYCLES 22
#define READ_SYSTICK_CNT_LOW *(volatile uint32_t*)(&SysTick->CNT)
#define READ_SYSTICK_CNT_HIGH *((volatile uint32_t*)(&SysTick->CNT) + 1)
/* try to read systick count properly */
static inline uint64_t read_systick_cnt() {
    uint32_t targendhi;
    uint32_t targendlo;
    do
    {
        targendhi = READ_SYSTICK_CNT_HIGH;
        targendlo = READ_SYSTICK_CNT_LOW;
    } while (READ_SYSTICK_CNT_HIGH != targendhi);
    return targendlo + ((uint64_t)targendhi << 32ULL);
}

void delayMicroseconds(unsigned int us)
{
    uint64_t targend = (read_systick_cnt() + us*sysclock_div_1000000 - CORRECTION_TERM_CYCLES);
    while( ((int64_t)( read_systick_cnt() - targend )) < 0 );
}