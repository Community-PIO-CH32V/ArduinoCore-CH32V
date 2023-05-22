#include <Arduino.h>
#include "wch/pinmap.h"

void pinMode(uint8_t pin, PinMode mode)
{
    GPIOMode_TypeDef gpio_mode;
    gpio_mode = mode == OUTPUT           ? GPIO_Mode_Out_PP
                : mode == INPUT          ? GPIO_Mode_IN_FLOATING
                : mode == INPUT_PULLDOWN ? GPIO_Mode_IPD
                : mode == INPUT_PULLUP   ? GPIO_Mode_IPU
                                         : GPIO_Mode_Out_OD;
    pinmap_initgpio(pin, gpio_mode);
}

void digitalWrite(uint8_t pin, PinStatus val)
{
    uint8_t gpio_port_num = pin >> 4u;
    uint8_t gpio_pin = pin & 0xfu;
    GPIO_TypeDef *GPIOx = GPIO_PORTS[gpio_port_num];
    GPIO_WriteBit(GPIOx, 1u << gpio_pin, val == LOW ? RESET : SET);
}

PinStatus digitalRead(uint8_t pin)
{
    uint8_t gpio_port_num = pin >> 4u;
    uint8_t gpio_pin = pin & 0xf;
    GPIO_TypeDef *GPIOx = GPIO_PORTS[gpio_port_num];
    return (PinStatus)GPIO_ReadInputDataBit(GPIOx, 1u << gpio_pin);
}