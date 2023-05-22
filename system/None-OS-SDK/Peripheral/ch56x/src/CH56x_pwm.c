/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_pwm.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
 * @fn     PWMX_CycleCfg
 *
 * @brief  PWM0-PWM3��׼ʱ������
 *
 * @param  cyc -
 *
 * @return   None
 */
void PWMX_CycleCfg( PWMX_CycleTypeDef cyc )
{
    switch( cyc )
    {
        case PWMX_Cycle_256:
        	R8_PWM_CTRL_CFG &= ~RB_PWM_CYCLE_SEL;    //PWM ���ÿ��ƼĴ���,ʱ������ѡ��
            break;

        case PWMX_Cycle_255:
        	R8_PWM_CTRL_CFG |= RB_PWM_CYCLE_SEL;
            break;

        default :
            break;
    }
}

/*******************************************************************************
 * @fn     PWMX_ACTOUT
 *
 * @brief  PWM0-PWM3ͨ�������������
 *
 * @param  ch -	select channel of pwm
 *					refer to channel of PWM define
 *	       da -	effective pulse width
 *		   pr -  select wave polar
 *					refer to PWMX_PolarTypeDef
 *		   s  -  control pwmx function
 *					ENABLE  - ���PWM
 *					DISABLE - �ر�PWM
 * @return   None
 */
void PWMX_ACTOUT( UINT8 ch, UINT8 da, PWMX_PolarTypeDef pr, UINT8 s)
{
    UINT8 i;

    if(s == DISABLE)	R8_PWM_CTRL_MOD &= ~(ch);                        //�ж�PWM�Ƿ�������ʹ��
    else
    {

    	(pr)?(R8_PWM_CTRL_MOD|=(ch<<4)):(R8_PWM_CTRL_MOD&=~(ch<<4));     //PWM������Կ���       1��Ĭ�ϸߵ�ƽ������Ч��0��Ĭ�ϵ͵�ƽ������Ч��
        for(i=0; i<4; i++){
            if((ch>>i)&1)		*((PUINT8V)((&R8_PWM0_DATA)+i)) = da;
        }
        R8_PWM_CTRL_MOD |= (ch);
    }
}





