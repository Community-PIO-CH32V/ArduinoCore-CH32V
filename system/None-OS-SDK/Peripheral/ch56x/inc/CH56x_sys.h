/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_sys.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description        : This file contains all the functions prototypes for
*                      SystemCoreClock, UART Printf , Delay functions .
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/


#ifndef __CH56x_SYS_H__
#define __CH56x_SYS_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "CH56xSFR.h"


/**
  * @brief  SYSTEM Information State
  */
typedef enum
{
	INFO_RESET_EN = 2,			// RST#�ⲿ�ֶ���λ���빦���Ƿ���
	INFO_BOOT_EN,				// ϵͳ�������� BootLoader �Ƿ���
	INFO_DEBUG_EN,				// ϵͳ������Խӿ��Ƿ���
	INFO_LOADER,				// ��ǰϵͳ�Ƿ���Bootloader ��
}SYS_InfoStaTypeDef;


#define SYS_GetChipID()				R8_CHIP_ID									/* ��ȡоƬID�࣬һ��Ϊ�̶�ֵ */
#define SYS_GetAccessID()			R8_SAFE_ACCESS_ID							/* ��ȡ��ȫ����ID��һ��Ϊ�̶�ֵ */
UINT8 SYS_GetInfoSta( SYS_InfoStaTypeDef i );									/* ��ȡ��ǰϵͳ��Ϣ״̬ */

void Delay_Init(uint32_t systemclck);
void mDelayuS(uint32_t n);
void mDelaymS(uint32_t n);

//refer to SYS_ResetStaTypeDef
#define SYS_GetLastResetSta()		(R8_RST_BOOT_STAT&RB_RESET_FLAG)				/* ��ȡϵͳ�ϴθ�λ״̬ */
void SYS_ResetExecute( void );													/* ִ��ϵͳ�����λ */
#define SYS_ResetKeepBuf( d )		(R8_GLOB_RESET_KEEP = d)					/* �����ֶ���λ�� �����λ�� ���Ź���λ������ͨ���Ѹ�λ��Ӱ�� */

//WWDG
#define  WWDG_SetCounter( c )		(R8_WDOG_COUNT = c)							/* ���ؿ��Ź�������ֵ�������� */
void  WWDG_ITCfg( UINT8 s );							/* ���Ź�����ж�ʹ�� */
void  WWDG_ResetCfg( UINT8 s );							/* ���Ź������λʹ�� */
#define  WWDG_GetFlowFlag()			(R8_RST_WDOG_CTRL&RB_WDOG_INT_FLAG)			/* ��ȡ��ǰ���Ź���ʱ�������־ */
void WWDG_ClearFlag(void);														/* ������Ź��жϱ�־�����¼��ؼ���ֵҲ����� */



#ifdef __cplusplus
}
#endif

#endif  // __CH56x_SYS_H__	

