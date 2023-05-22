/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_pwr.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
 * @fn     PWR_PeriphClkCfg
 *
 * @brief  ����ʱ�ӿ���λ
 * @param  s -
 *                  ENABLE  - ������ʱ��
 *                  DISABLE - �ر�����ʱ��
 *                  perph -
 *                     please refer to Peripher CLK control bit define
 
 * @return   None
 */
void PWR_PeriphClkCfg( UINT8 s, UINT16 perph )
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R32_SLEEP_CONTROL |= perph;
    }
    else
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R32_SLEEP_CONTROL &= ~perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
 * @fn     PWR_PeriphWakeUpCfg
 *
 * @brief  ˯�߻���Դ����
 *
 * @param  s -
 *                  ENABLE  - �򿪴�����˯�߻��ѹ���
 *                  DISABLE - �رմ�����˯�߻��ѹ���
 *         perph -
 *                 RB_SLP_USBHS_WAKE -  USB2.0Ϊ����Դ
 *                 RB_SLP_USBSS_WAKE -  USB3.0Ϊ����Դ
 *                 RB_SLP_GPIO_WAKE  -  GPIOΪ����Դ
 *                 RB_SLP_ETH_WAKE   -  ETHΪ����Դ
 *                 ALL              -  ��������
 * @return   None
 */
void PWR_PeriphWakeUpCfg( UINT8 s, UINT16 perph )
{
    if( s == DISABLE )
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R8_SLP_WAKE_CTRL &= ~perph;
    }
    else
    {
        R8_SAFE_ACCESS_SIG = 0x57;
        R8_SAFE_ACCESS_SIG = 0xA8;
        R8_SLP_WAKE_CTRL |= perph;
    }
    R8_SAFE_ACCESS_SIG = 0;
}

/*******************************************************************************
 * @fn     LowPower_Idle
 *
 * @brief  �͹���-Idleģʽ
 *
 * @return   None
 */
void LowPower_Idle( void )
{

	PFIC->SCTLR &= ~1<<2;				// �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ0
    __WFI();                            // ���û���������ִ��__WFI()

}

/*******************************************************************************
 * @fn     LowPower_Halt
 *
 * @brief  �͹���-Haltģʽ
 * 
 * @return   None
 */
void LowPower_Halt( void )
{

	PFIC->SCTLR |= 1<<2;                      // �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ1
	R8_SLP_POWER_CTRL |= RB_SLP_USBHS_PWRDN;  // RB_SLP_USBHS_PWRDN��1
	__WFI();                                  // ���û���������ִ��__WFI()
}

/*******************************************************************************
 * @fn     LowPower_Sleep
 *
 * @brief  �͹���-Sleepģʽ
 *
 * @return   None
 */
void LowPower_Sleep( void )
{

	PFIC->SCTLR |= 1<<2;                      // �����ں�PFIC_SCTLR�Ĵ�����SleepDeep�ֶ�Ϊ1
	R8_SLP_POWER_CTRL |= RB_SLP_USBHS_PWRDN;  // RB_SLP_USBHS_PWRDN��1
	R8_SLP_WAKE_CTRL &= ~RB_SLP_USBSS_WAKE;   // RB_SLP_USBSS_WAKE��0
	__WFI();                                  // ���û���������ִ��__WFI()

}




