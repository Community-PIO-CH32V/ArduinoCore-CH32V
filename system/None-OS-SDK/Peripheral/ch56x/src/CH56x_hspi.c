/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_hspi.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
 * @fn     HSPI_Mode
 *
 * @brief  DVPģʽ
 *
 * @param  s -  ����λ��
 *					RB_HPIF_DAT8_MOD  - 8λģʽ
 *					RB_HPIF_DAT16_MOD - 16λģʽ
 *					RB_HPIF_DAT32_MOD - 32λģʽ
 *		   i -  ����ģʽ
 *					UP_Mode - ʹ���϶�ģʽ
 *					DOWN_Mode - ʹ���¶�ģʽ
 *
 * @return   None
 */
void HSPI_Mode( UINT8 s,  HSPI_ModeTypeDef i)
{
	R8_HSPI_CFG &= ~RB_HSPI_MSK_SIZE;   //�ָ�Ĭ��ģʽ8bitģʽ

    if(s){
    	R8_HSPI_CFG |= s;
    }
    else{
    	R8_HSPI_CFG &= ~RB_HSPI_MSK_SIZE;
    }

    if(i){
    	R8_HSPI_CFG |= RB_HSPI_MODE;
    }
    else{
    	R8_HSPI_CFG &= ~RB_HSPI_MODE;
    }
}

/*******************************************************************************
 * @fn     HSPI_INTCfg
 *
 * @brief  HSPI�ж�����
 *
 * @param  s -  �жϿ���״̬
 *					ENABLE  - ʹ����Ӧ�ж�
 *					DISABLE - �ر���Ӧ�ж�
 *		   i -  �ж�����
 *					RB_HSPI_IE_T_DONE  - ͻ�����з�������ж�
 *					RB_HSPI_IE_R_DONE  - ����FIFO����ж�
 *					RB_HSPI_IE_FIFO_OV - ������������ж�
 *				    RB_HSPI_IE_B_DONE  - ������������ж�
 *
 * @return   None
 */
void HSPI_INTCfg( UINT8 s,  UINT8 i )
{
    if(s){
    	R8_HSPI_INT_EN |= i;
    }
    else{
    	R8_HSPI_INT_EN &= ~i;
    }
}

