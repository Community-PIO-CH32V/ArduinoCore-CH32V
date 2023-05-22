#include <Arduino.h>

static const GPIO_TypeDef* GPIO_PORTS[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE
};

void pinMode(uint8_t pin, PinMode mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    uint8_t gpio_port_num = pin >> 4u; // encode port and pin in one byte, 4 bits each
    uint8_t gpio_pin = pin & 0xfu;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << gpio_port_num, ENABLE);
    GPIO_InitStructure.GPIO_Pin = 1u << gpio_pin; 
    GPIO_InitStructure.GPIO_Mode = 
        mode == OUTPUT ? GPIO_Mode_Out_PP : 
        mode == INPUT ? GPIO_Mode_IN_FLOATING :
        mode == INPUT_PULLDOWN ? GPIO_Mode_IPD :
        mode == INPUT_PULLUP ? GPIO_Mode_IPU : GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_TypeDef *GPIOx = GPIO_PORTS[gpio_port_num];
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}


void digitalWrite(uint8_t pin, PinStatus val)
{
    uint8_t gpio_port_num = pin >> 4u; // encode port and pin in one byte, 4 bits each
    uint8_t gpio_pin = pin & 0xfu;
    GPIO_TypeDef *GPIOx = GPIO_PORTS[gpio_port_num];
    GPIO_WriteBit(GPIOx, 1u << gpio_pin, val == LOW ? RESET : SET);
}


PinStatus digitalRead(uint8_t pin)
{
    uint8_t gpio_port_num = pin >> 4u; // encode port and pin in one byte, 4 bits each
    uint8_t gpio_pin = pin & 0xf;
    GPIO_TypeDef *GPIOx = 
        gpio_port_num == 0 ? GPIOA :
        gpio_port_num == 1 ? GPIOB:
        gpio_port_num == 2 ? GPIOC:
        gpio_port_num == 3 ? GPIOD : GPIOE;
    return (PinStatus) GPIO_ReadInputDataBit(GPIOx, 1u << gpio_pin);
}