/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_conf.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Library configuration file.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CH32V30x_CONF_H
#define __CH32V30x_CONF_H

#include "ch32v30x_adc.h"
#include "ch32v30x_bkp.h"
#include "ch32v30x_can.h"
#include "ch32v30x_crc.h"
#include "ch32v30x_dac.h"
#include "ch32v30x_dbgmcu.h"
#include "ch32v30x_dma.h"
#include "ch32v30x_exti.h"
#include "ch32v30x_flash.h"
#include "ch32v30x_fsmc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_i2c.h"
#include "ch32v30x_iwdg.h"
#include "ch32v30x_pwr.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rtc.h"
#include "ch32v30x_sdio.h"
#include "ch32v30x_spi.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_usart.h"
#include "ch32v30x_wwdg.h"
#include "ch32v30x_misc.h"
#include "ch32v30x_eth.h"

/* RT Thread expects the interrupt header file from the project to be included */
/* Support both our unified name and the old name */
#if defined(__PIO_BUILD_RT_THREAD__)
#if __has_include("ch32v_it.h")
# include "ch32v_it.h"
#elif __has_include("ch32v30x_it.h")
# include "ch32v30x_it.h"
#endif
#endif

#endif /* __CH32V30x_CONF_H */


	
	
	
