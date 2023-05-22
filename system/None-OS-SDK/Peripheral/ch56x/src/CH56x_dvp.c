/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_dvp.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
 * @fn     DVP_INTCfg
 *
 * @brief   DVP�ж�����
 *
 * @param  s:  �жϿ���״̬
					ENABLE  - ʹ����Ӧ�ж�
					DISABLE - �ر���Ӧ�ж�
				   i:  �ж�����
					RB_DVP_IE_STP_FRM  - ֡�����ж�
					RB_DVP_IE_FIFO_OV  - ����FIFO����ж�
					RB_DVP_IE_FRM_DONE - ֡�����ж�
					RB_DVP_IE_ROW_DONE - �н����ж�
					RB_DVP_IE_STR_FRM  - ֡��ʼ�ж�
 *
 * @return  None
 **/
void DVP_INTCfg( UINT8 s,  UINT8 i )
{
    if(s){
    	R8_DVP_INT_EN |= i;
    }
    else{
    	R8_DVP_INT_EN &= ~i;
    }
}

/*******************************************************************************
 * @fn    DVP_Mode
 *
 * @brief  DVPģʽ
 *
 * @param  s:  ����λ��
					RB_DVP_D8_MOD  - 8λģʽ
					RB_DVP_D10_MOD - 10λģʽ
					RB_DVP_D12_MOD - 12λģʽ
				   i:  ѹ������ģʽ
					Video_Mode - ʹ����Ƶģʽ
					JPEG_Mode  - ʹ��JPEGģʽ
 *
 * @return  None
 */
void DVP_Mode( UINT8 s,  DVP_Data_ModeTypeDef i)
{
	R8_DVP_CR0 &= ~RB_DVP_MSK_DAT_MOD;   //�ָ�Ĭ��ģʽ8bitģʽ

    if(s){
    	R8_DVP_CR0 |= s;
    }
    else{
    	R8_DVP_CR0 &= ~(3<<4);
    }

    if(i){
    	R8_DVP_CR0 |= RB_DVP_JPEG;
    }
    else{
    	R8_DVP_CR0 &= ~RB_DVP_JPEG;
    }
}

/*******************************************************************************
 * @fn      DVP_Cfg
 *
 * @brief   DVP����
 *
 * @param   s:  DMAʹ�ܿ���
					DVP_DMA_Enable  - DMAʹ��
					DVP_DMA_Disable - DMAʧ��
				   i:  ��־��FIFO�������
					DVP_FLAG_FIFO_RESET_Enable  - ��λ��־��FIFO
                    DVP_FLAG_FIFO_RESET_Disable - ȡ����λ����
				   j:  �����߼���λ����
					DVP_RX_RESET_Enable - ��λ�����߼���·
					DVP_RX_RESET_Disable - ȡ����λ����
 *
 * @return   None
 */
void DVP_Cfg( DVP_DMATypeDef s,  DVP_FLAG_FIFO_RESETTypeDef i, DVP_RX_RESETTypeDef j)
{
    switch( s )
    {
        case DVP_DMA_Enable:
        	R8_DVP_CR1 |= RB_DVP_DMA_EN;
            break;
        case DVP_DMA_Disable:
        	R8_DVP_CR1 &= ~RB_DVP_DMA_EN;
            break;
        default:
            break;
    }

    switch( i )
    {
        case DVP_RX_RESET_Enable:
        	R8_DVP_CR1 |= RB_DVP_ALL_CLR;
            break;
        case DVP_RX_RESET_Disable:
        	R8_DVP_CR1 &= ~RB_DVP_ALL_CLR;
            break;
        default:
            break;
    }

    switch( j )
    {
        case DVP_RX_RESET_Enable:
        	R8_DVP_CR1 |= RB_DVP_RCV_CLR;
            break;
        case DVP_RX_RESET_Disable:
        	R8_DVP_CR1 &= ~RB_DVP_RCV_CLR;
            break;
        default:
            break;
    }

}
