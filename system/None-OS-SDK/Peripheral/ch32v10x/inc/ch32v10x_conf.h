/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v10x_conf.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : Library configuration file.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CH32V10x_CONF_H
#define __CH32V10x_CONF_H

#include "ch32v10x_adc.h"
#include "ch32v10x_bkp.h"
#include "ch32v10x_crc.h"
#include "ch32v10x_dbgmcu.h"
#include "ch32v10x_dma.h"
#include "ch32v10x_exti.h"
#include "ch32v10x_flash.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_i2c.h"
#include "ch32v10x_iwdg.h"
#include "ch32v10x_pwr.h"
#include "ch32v10x_rcc.h"
#include "ch32v10x_rtc.h"
#include "ch32v10x_spi.h"
#include "ch32v10x_tim.h"
#include "ch32v10x_usart.h"
#include "ch32v10x_wwdg.h"
/* Harmony OS has build error from types defined in USB headers, can't be used together for the moment. */
#if !defined(__PIO_BUILD_HARMONY_LITEOS__)
#include "ch32v10x_usb.h"
#include "ch32v10x_usb_host.h"
#endif
#include "ch32v10x_misc.h"


/* RT Thread expects the interrupt header file from the project to be included */
/* Support both our unified name and the old name */
#if defined(__PIO_BUILD_RT_THREAD__)
#if __has_include("ch32v_it.h")
# include "ch32v_it.h"
#elif __has_include("ch32v10x_it.h")
# include "ch32v10x_it.h"
#endif
#endif

#endif /* __CH32V10x_CONF_H */


	
	
	
