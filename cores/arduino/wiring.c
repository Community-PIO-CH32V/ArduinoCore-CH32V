#include <Arduino.h>

static volatile unsigned long currTime = 0;
void init() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    /* setup sysclock */
    SysTick->CTLR = 0;
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR = 0;
    SysTick->CNT = 0;
    /* 1000 Hz interrupts (aka once per millisecond) */
    SysTick->CMP = (u64)(SystemCoreClock / 1000ul) -1;;
    /* counter enable, counter interupt enable, counter clock source: HCLK, auto-reload on */
    SysTick->CTLR = 0b1111;
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void SysTick_Handler(void) {
    currTime++;
    SysTick->SR = 0; // clear IRQ
}

unsigned long millis()
{
    return currTime;
}

unsigned long micros() {
    return currTime * 1000ul;
}


void delay(unsigned long ms)
{
    uint32_t start = micros();

    while (ms > 0) {
        yield();
        while ( ms > 0 && (micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
}

void delayMicroseconds(unsigned int us)
{
}