/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_bus8.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"



/*******************************************************************************
 * @fn    BUS8_GPIO_Init
 *
 * @brief  BUS8 GPIO ��ʼ��
 *
 * @return  None
 */
void BUS8_GPIO_Init(void)
{
	R32_PA_DIR = 0;
	R32_PA_PU  = 0xff;
	R32_PA_SMT = 0xffffffff;
	R32_PA_DIR |= bBUSRD | bBUSWR;       //R/W signal GPIO
	R32_PB_DIR |= 0x7fff;
	R32_PB_SMT |= 0x7fff;

}

/*******************************************************************************
 * @fn     BUS8_Init
 *
 * @brief  BUS8 ��ʼ��
 *
 * @param  addroe:0x00-none;0x04-[5:0];0x08-[9:0];0x0c-[14:0];
 *				   width: 0x00-3;0x10-5;0x20-9;0x30-16;
 *				   hold:  0x00-2;0x40-3;
 *				   setup: 0x00-2;0x80-3;
 *
 * @return  None
 */
void BUS8_Init(UINT8 addroe, UINT8 width, UINT8 hold, UINT8 setup)
{
	R8_XBUS_CONFIG =  addroe | width | hold | setup;
	R8_XBUS_CONFIG |= RB_XBUS_ENABLE;	//Enable
	BUS8_GPIO_Init();
}





