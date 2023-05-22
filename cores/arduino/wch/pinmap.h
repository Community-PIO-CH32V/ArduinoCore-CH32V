#ifndef WCH_PINMAP_H
#define WCH_PINMAP_H

#include "ch32vxxx.h"

#ifdef __cplusplus
extern "C" {
#endif

extern GPIO_TypeDef* const GPIO_PORTS[];

#define GPIO_PORTNUM_TO_PORT(portnum) GPIO_PORTS[(portnum)]

void pinmap_initgpio(uint8_t pin, GPIOMode_TypeDef mode);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* WCH_PINMAP_H */