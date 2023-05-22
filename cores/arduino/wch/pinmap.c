#include "pinmap.h"

GPIO_TypeDef* const GPIO_PORTS[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE
};

void pinmap_initgpio(uint8_t pin, GPIOMode_TypeDef mode) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // encode port and pin in one byte, 4 bits each
    uint8_t gpio_port_num = pin >> 4u;
    uint8_t gpio_pin = pin & 0xfu;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gpio_port_num, ENABLE);
    GPIO_InitStructure.GPIO_Pin = 1u << gpio_pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_TypeDef *GPIOx = GPIO_PORTS[gpio_port_num];
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}