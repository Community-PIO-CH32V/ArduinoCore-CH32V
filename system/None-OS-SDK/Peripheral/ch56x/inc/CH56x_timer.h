/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_timer.ch
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/


#ifndef __CH56x_TIMER_H__
#define __CH56x_TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif
	

/**
  * @brief  �����ȵ�����Ч�������
  */
typedef enum
{
	PWM_Times_1 = 0,					// PWM ��Ч����ظ�1����
	PWM_Times_4 = 1,						// PWM ��Ч����ظ�4����
	PWM_Times_8 = 2,						// PWM ��Ч����ظ�8����
	PWM_Times_16 = 3,						// PWM ��Ч����ظ�16����
}PWM_RepeatTsTypeDef;


/**
  * @brief  ���벶����ط�ʽ
  */
typedef enum
{
	CAP_NULL = 0,						// ����׽
	Edge_To_Edge = 1,						// �������֮��
	FallEdge_To_FallEdge = 2,				// �½��ص��½���
	RiseEdge_To_RiseEdge = 3,				// �����ص�������
}CapModeTypeDef;


/**
  * @brief  ֱ�ӷ��ʴ洢��ѭ��ģʽ
  */
typedef enum
{
	Mode_Single = 0,				// ����ģʽ
	Mode_LOOP = 1,						// ѭ��ģʽ
}DMAModeTypeDef;


/**
  * @brief  PWM���������
  */
typedef enum
{
	high_on_low = 0,						// Ĭ�ϵ͵�ƽ���ߵ�ƽ��Ч
	low_on_high = 1,						// Ĭ�ϸߵ�ƽ���͵�ƽ��Ч
}PWM_PolarTypeDef;


/****************** TMR0 */
// ��ʱ�ͼ���
void TMR0_TimerInit( UINT32 t );									/* ��ʱ���ܳ�ʼ�� */
void TMR0_EXTSignalCounterInit( UINT32 c );							/* �ⲿ�źż������ܳ�ʼ�� */
#define  TMR0_GetCurrentCount()		R32_TMR0_COUNT	 				/* ��ȡ��ǰ����ֵ��67108864 */

// �����ȵ��ƹ���
#define TMR0_PWMCycleCfg( cyc )	    (R32_TMR0_CNT_END=cyc)			/* PWM0 ͨ�����������������, ���67108864 */
void TMR0_PWMInit( PWM_PolarTypeDef pr, PWM_RepeatTsTypeDef ts );	/* PWM �����ʼ�� */
#define TMR0_PWMActDataWidth( d )   (R32_TMR0_FIFO = d)			/* PWM0 ��Ч��������, ���67108864 */

// ��׽����
#define TMR0_CAPTimeoutCfg( cyc )   (R32_TMR0_CNT_END=cyc)			/* CAP0 ��׽��ƽ��ʱ����, ���33554432 */
void TMR0_CapInit( CapModeTypeDef cap );							/* �ⲿ�źŲ�׽���ܳ�ʼ�� */
#define TMR0_CAPGetData()			R32_TMR0_FIFO					/* ��ȡ�������� */
#define TMR0_CAPDataCounter()		R8_TMR0_FIFO_COUNT				/* ��ȡ��ǰ�Ѳ������ݸ��� */

#define TMR0_Disable()				(R8_TMR0_CTRL_MOD &= ~RB_TMR_COUNT_EN)		/* �ر� TMR0 */
// refer to TMR0 interrupt bit define
#define	TMR0_ITCfg(s,f)				((s)?(R8_TMR0_INTER_EN|=f):(R8_TMR0_INTER_EN&=~f))		/* TMR0 ��Ӧ�ж�λ������ر� */
// refer to TMR0 interrupt bit define
#define TMR0_ClearITFlag(f)         (R8_TMR0_INT_FLAG = f)			/* ����жϱ�־ */
#define TMR0_GetITFlag(f)           (R8_TMR0_INT_FLAG&f)			/* ��ѯ�жϱ�־״̬ */


/****************** TMR1 */
// ��ʱ�ͼ���
void TMR1_TimerInit( UINT32 t );									/* ��ʱ���ܳ�ʼ�� */
void TMR1_EXTSignalCounterInit( UINT32 c );							/* �ⲿ�źż������ܳ�ʼ�� */
#define  TMR1_GetCurrentCount()		R32_TMR1_COUNT	 				/* ��ȡ��ǰ����ֵ�����67108864 */

// �����ȵ��ƹ���
#define TMR1_PWMCycleCfg( cyc )	    (R32_TMR1_CNT_END=cyc)			/* PWM1 ͨ�����������������, ���67108864 */
void TMR1_PWMInit( PWM_PolarTypeDef pr, PWM_RepeatTsTypeDef ts );	/* PWM1 �����ʼ�� */
#define TMR1_PWMActDataWidth( d )   (R32_TMR1_FIFO = d)			/* PWM1 ��Ч��������, ���67108864 */

// ��׽����
#define TMR1_CAPTimeoutCfg( cyc )   (R32_TMR1_CNT_END=cyc)			/* CAP1 ��׽��ƽ��ʱ����, ���33554432 */
void TMR1_CapInit( CapModeTypeDef cap );							/* �ⲿ�źŲ�׽���ܳ�ʼ�� */
#define TMR1_CAPGetData()			R32_TMR1_FIFO					/* ��ȡ�������� */
#define TMR1_CAPDataCounter()		R8_TMR1_FIFO_COUNT				/* ��ȡ��ǰ�Ѳ������ݸ��� */

void TMR1_DMACfg( UINT8 s, UINT16 startAddr, UINT16 endAddr, DMAModeTypeDef m );    /* DMA����  */

#define TMR1_Disable()				(R8_TMR1_CTRL_MOD &= ~RB_TMR_COUNT_EN)		/* �ر� TMR1 */
// refer to TMR1 interrupt bit define
#define	TMR1_ITCfg(s,f)				((s)?(R8_TMR1_INTER_EN|=f):(R8_TMR1_INTER_EN&=~f))		/* TMR1 ��Ӧ�ж�λ������ر� */
// refer to TMR1 interrupt bit define
#define TMR1_ClearITFlag(f)         (R8_TMR1_INT_FLAG = f)			/* ����жϱ�־ */
#define TMR1_GetITFlag(f)           (R8_TMR1_INT_FLAG&f)			/* ��ѯ�жϱ�־״̬ */


/****************** TMR2 */
// ��ʱ�ͼ���
void TMR2_TimerInit( UINT32 t );									/* ��ʱ���ܳ�ʼ�� */
void TMR2_EXTSignalCounterInit( UINT32 c );							/* �ⲿ�źż������ܳ�ʼ�� */
#define  TMR2_GetCurrentCount()		R32_TMR2_COUNT	 				/* ��ȡ��ǰ����ֵ�����67108864 */

// �����ȵ��ƹ���
#define TMR2_PWMCycleCfg( cyc )	    (R32_TMR2_CNT_END=cyc)			/* PWM2 ͨ�����������������, ���67108864 */
void TMR2_PWMInit( PWM_PolarTypeDef pr, PWM_RepeatTsTypeDef ts );	/* PWM2 �����ʼ�� */
#define TMR2_PWMActDataWidth( d )   (R32_TMR2_FIFO = d)			/* PWM2 ��Ч��������, ���67108864 */

// ��׽����
#define TMR2_CAPTimeoutCfg( cyc )   (R32_TMR2_CNT_END=cyc)			/* CAP2 ��׽��ƽ��ʱ����, ���33554432 */
void TMR2_CapInit( CapModeTypeDef cap );							/* �ⲿ�źŲ�׽���ܳ�ʼ�� */
#define TMR2_CAPGetData()			R32_TMR2_FIFO					/* ��ȡ�������� */
#define TMR2_CAPDataCounter()		R8_TMR2_FIFO_COUNT				/* ��ȡ��ǰ�Ѳ������ݸ��� */

void TMR2_DMACfg( UINT8 s, UINT16 startAddr, UINT16 endAddr, DMAModeTypeDef m );    /* DMA����  */

#define TMR2_Disable()				(R8_TMR2_CTRL_MOD &= ~RB_TMR_COUNT_EN)		/* �ر� TMR2 */
// refer to TMR2 interrupt bit define
#define	TMR2_ITCfg(s,f)				((s)?(R8_TMR2_INTER_EN|=f):(R8_TMR2_INTER_EN&=~f))		/* TMR2 ��Ӧ�ж�λ������ر� */
// refer to TMR2 interrupt bit define
#define TMR2_ClearITFlag(f)         (R8_TMR2_INT_FLAG = f)			/* ����жϱ�־ */
#define TMR2_GetITFlag(f)           (R8_TMR2_INT_FLAG & f)			/* ��ѯ�жϱ�־״̬ */



#ifdef __cplusplus
 }
#endif

#endif // __CH56x_TIMER_H__





