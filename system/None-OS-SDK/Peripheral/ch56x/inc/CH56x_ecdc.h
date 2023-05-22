/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_ecdc.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/


#ifndef __CH56x_ECDC_H__
#define __CH56x_ECDC_H__

#ifdef __cplusplus
 extern "C" {
#endif


/* �ӽ���ģʽ */
#define   MODE_SM4_ECB        0
#define   MODE_AES_ECB        1
#define   MODE_SM4_CTR        2
#define   MODE_AES_CTR        3

/* ��С��ģʽ */
#define   MODE_BIG_ENDIAN     1
#define   MODE_LITTLE_ENDIAN  0

/* ��Կ���� */
#define   KEYLENGTH_128BIT    0
#define   KEYLENGTH_192BIT    1
#define   KEYLENGTH_256BIT    2

/* �ӽ����ٶ� */
#define   ECDCCLK_DISABLE     1
#define   ECDCCLK_240MHZ      2
#define   ECDCCLK_160MHZ      3

/* �����ģʽ*/
#define   SELFDMA_ENCRY				0x84
#define   SELFDMA_DECRY				0x8c
#define   SINGLEREGISTER_ENCRY		PERIPHERAL_TO_RAM_ENCRY
#define   SINGLEREGISTER_DECRY		PERIPHERAL_TO_RAM_DECRY
#define   PERIPHERAL_TO_RAM_ENCRY   0x02
#define   PERIPHERAL_TO_RAM_DECRY   0x0a
#define   RAM_TO_PERIPHERAL_ENCRY   0x04
#define   RAM_TO_PERIPHERAL_DECRY   0x0c

void ECDC_Init( UINT8 ecdcmode, UINT8 clkmode, UINT8 keylen, PUINT32 pkey, PUINT32 pcount );
void ECDC_SetKey( PUINT32 pkey, UINT8 keylen );
void ECDC_SetCount( PUINT32 pcount );
void ECDC_Excute( UINT8 excutemode, UINT8 endianmode );
void ECDC_SingleRegister( PUINT32 pWdatbuff, PUINT32 pRdatbuff );
void ECDC_SelfDMA( UINT32 ram_addr, UINT32 ram_len );
void ECDC_RloadCount( UINT8 excutemode, UINT8 endianmode, PUINT32 pcount );

	
	
#ifdef __cplusplus
}
#endif

#endif  // __CH56x_ECDC_H__	

