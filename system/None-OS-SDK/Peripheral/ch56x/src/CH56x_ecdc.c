/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_ecdc.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"


/*******************************************************************************
 * @fn     ECDC_Init
 *
 * @brief  ��ʼ��
 *
 * @param  ecdcmode -	0-SM4&ECB     1-AES&ECB     2-SM4&CTR     3-AES&CTR
 *         clkmode -     1-�ر�        2-240M        3-160M
 *         keylen -      0-128bit      1-192bit      2-256bit
 *         pkey -    ��Կֵָ��
 *         pcount -  ������ֵָ��
 *
 * @return   None
 */
void ECDC_Init( UINT8 ecdcmode, UINT8 clkmode, UINT8 keylen, PUINT32 pkey, PUINT32 pcount )
{
	R8_ECDC_INT_FG |= 0xFF;
	R16_ECEC_CTRL = 0;

	R16_ECEC_CTRL |= (ecdcmode&0x03)<<8;     		//����ģʽѡ��
	R16_ECEC_CTRL |= (keylen&0x03)<<10;			     //��Կ��������
	R16_ECEC_CTRL |= (clkmode&0x03)<<4;       		//�ӽ���ʱ�ӷ�Ƶϵ��,aes�ӽ��ܹ�����240Mhz��
	ECDC_SetKey(pkey, keylen);

	if(R16_ECEC_CTRL & RB_ECDC_CIPHER_MOD)			//ֻ��CTRģʽ��ִ�У�CTR��ECBģʽ���ʱ��Ψһ����
			ECDC_SetCount(pcount);

	R8_ECDC_INT_FG |= RB_ECDC_IF_EKDONE;
	R16_ECEC_CTRL |= RB_ECDC_KEYEX_EN;
	R16_ECEC_CTRL &= ~RB_ECDC_KEYEX_EN;

	while(!(R8_ECDC_INT_FG & RB_ECDC_IF_EKDONE));
	R8_ECDC_INT_FG |= RB_ECDC_IF_EKDONE;
}

/*******************************************************************************
 * @fn     ECDC_SetKey
 *
 * @brief  ������Կ
 *
 * @param  pkey -   ��Կֵָ��
 *         keylen - 0-128bit   1-192bit   2-256bit
 
 * @return   None
 */
void ECDC_SetKey( PUINT32 pkey, UINT8 keylen )
{
	keylen = keylen&0x03;

	R32_ECDC_KEY_31T0 = *pkey++;
	R32_ECDC_KEY_63T32 = *pkey++;
	R32_ECDC_KEY_95T64 = *pkey++;
	R32_ECDC_KEY_127T96 = *pkey++;

	if(keylen){
		R32_ECDC_KEY_159T128 = *pkey++;
		R32_ECDC_KEY_191T160 = *pkey++;
	}
	if(keylen>1){
		R32_ECDC_KEY_223T192 = *pkey++;
		R32_ECDC_KEY_255T224 = *pkey++;
	}
}

/*******************************************************************************
 * @fn     ECDC_SetCount
 *
 * @brief  ���ü�����
 *
 * @param  pcount -  ������ֵָ��
 *
 * @return   None
 */
void ECDC_SetCount( PUINT32 pcount )
{
	R32_ECDC_IV_31T0 = *pcount++;
	R32_ECDC_IV_63T32 = *pcount++;
	R32_ECDC_IV_95T64 = *pcount++;
	R32_ECDC_IV_127T96 = *pcount++;
}

/*******************************************************************************
 * @fn     ECDC_Excute
 *
 * @brief  ���÷����ģʽ
 *
 * @param  excutemode -	  RAMX����			-0x84
 *					      RAMX����			-0x8c
 *						  128bits���ݵ��μ���	-0x02
 *						  128bits���ݵ��ν���	-0x0a
 *					      ���赽RAMX ����  		-0x02
 *				    	  ���赽 RAMX ����   		-0x0a
 *						  RAMX���������   		-0x04
 *						  RAMX���������   		-0x0c
 *   	   endianmode -    big_endian-1      little_endian-0
 *
 * @return   None
 */
void ECDC_Excute( UINT8 excutemode, UINT8 endianmode )
{
	R16_ECEC_CTRL &= 0xDF71;
	R16_ECEC_CTRL |= excutemode;
	if(endianmode)
		R16_ECEC_CTRL |= RB_ECDC_DAT_MOD;
	else
		R16_ECEC_CTRL &= ~RB_ECDC_DAT_MOD;
}

/*******************************************************************************
 * @fn     ECDC_SingleRegister
 *
 * @brief  ���μĴ����ӽ���
 *
 * @param  pWdatbuff - д�������׵�ַ
 *         pRdatbuff - ��ȡ�����׵�ַ
 *
 * @return   None
 */
void ECDC_SingleRegister( PUINT32 pWdatbuff, PUINT32 pRdatbuff )
{
	R32_ECDC_SGSD_127T96 = pWdatbuff[3];			//�͵�ַ
	R32_ECDC_SGSD_95T64 = pWdatbuff[2];
	R32_ECDC_SGSD_63T32 = pWdatbuff[1];
	R32_ECDC_SGSD_31T0 = pWdatbuff[0];				//�ߵ�ַ

	while(!(R8_ECDC_INT_FG & RB_ECDC_IF_SINGLE));
	R8_ECDC_INT_FG |= RB_ECDC_IF_SINGLE;

	pRdatbuff[3] = R32_ECDC_SGRT_127T96;
	pRdatbuff[2] = R32_ECDC_SGRT_95T64;
	pRdatbuff[1] = R32_ECDC_SGRT_63T32;
	pRdatbuff[0] = R32_ECDC_SGRT_31T0;
}

/*******************************************************************************
 * @fn     ECDC_RAMX
 *
 * @brief  RAMX�ӽ���
 *
 * @param  ram_add - �׵�ַ
 * 		   ram_len -  ����
 * @return   None
 **/
void ECDC_SelfDMA( UINT32 ram_addr, UINT32 ram_len )
{
	R32_ECDC_SRAM_ADDR = ram_addr;
	R32_ECDC_SRAM_LEN = ram_len;                    //��ʼת��

	while(!(R8_ECDC_INT_FG & RB_ECDC_IF_WRSRAM));   //��ɱ�־λ
	R8_ECDC_INT_FG |= RB_ECDC_IF_WRSRAM;
}

/*******************************************************************************
 * @fn     ECDC_RloadCount
 *
 * @brief  CTRģʽ�£�ÿ����/����һ�飬�������������ֵ
 *
 * @param  pcount -  ������ֵָ��
 *
 * @return   None
 */
void ECDC_RloadCount( UINT8 excutemode, UINT8 endianmode, PUINT32 pcount )
{
	R16_ECEC_CTRL &= 0xDFF9;       //�ڶ�λ����λ��0
	ECDC_SetCount(pcount);
	ECDC_Excute(excutemode, endianmode);
}

