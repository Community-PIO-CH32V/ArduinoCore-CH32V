/*
  UART.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Created: 09.11.2017 07:29:09
  Author: M44307
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "UART.h"
#include "wch/pinmap.h"


// this next line disables the entire UART.cpp,
// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3) 

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// UART instance if the user doesn't also refer to it.
#if defined(HAVE_HWSERIAL0) 
void serialEvent() __attribute__((weak));
bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
void serialEvent1() __attribute__((weak));
bool Serial1_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL2)
void serialEvent2() __attribute__((weak));
bool Serial2_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL3)
void serialEvent3() __attribute__((weak));
bool Serial3_available() __attribute__((weak));
#endif

void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL0)
    if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
    if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
#if defined(HAVE_HWSERIAL2)
    if (Serial2_available && serialEvent2 && Serial2_available()) serialEvent2();
#endif
#if defined(HAVE_HWSERIAL3)
    if (Serial3_available && serialEvent3 && Serial3_available()) serialEvent3();
#endif
}

// macro to guard critical sections when needed for large TX buffer sizes
#if (SERIAL_TX_BUFFER_SIZE > 256)
#define TX_BUFFER_ATOMIC ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
#else
#define TX_BUFFER_ATOMIC
#endif

// Class constructors
UartClass::UartClass(
  USART_TypeDef *hwserial_module,
  uint8_t hwserial_rx_pin,
  uint8_t hwserial_tx_pin) :
    _hwserial_module(hwserial_module),
    _hwserial_rx_pin(hwserial_rx_pin),
    _hwserial_tx_pin(hwserial_tx_pin),
    _rx_buffer_head(0), _rx_buffer_tail(0),
    _tx_buffer_head(0), _tx_buffer_tail(0)
{
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void UartClass::_rx_complete_irq(void)
{
  //if (bit_is_clear(*_rxdatah, USART_PERR_bp)) {
  if (USART_GetFlagStatus(this->_hwserial_module, USART_FLAG_PE) == RESET) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = (unsigned char) USART_ReceiveData(this->_hwserial_module);
    rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
      _rx_buffer[_rx_buffer_head] = c;
      _rx_buffer_head = i;
    }
    if (bound != NULL) {
      bound->write(c);
    }
  } else {
    // Parity error, read byte but discard it
    USART_ReceiveData(this->_hwserial_module);
  }
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void UartClass::_tx_data_empty_irq(void)
{
    // Check if tx buffer already empty.
    if (_tx_buffer_head == _tx_buffer_tail) {
        // Buffer empty, so disable "data register empty" interrupt
        //(*_hwserial_module).CTRLA &= (~USART_DREIE_bm);
        return;
    }

    // There must be more data in the output
    // buffer. Send the next byte
    unsigned char c = _tx_buffer[_tx_buffer_tail];
    _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE;

    // clear the TXCIF flag -- "can be cleared by writing a one to its bit
    // location". This makes sure flush() won't return until the bytes
    // actually got written
    //(*_hwserial_module).STATUS = USART_TXCIF_bm;

    USART_SendData(this->_hwserial_module, c);

    if (_tx_buffer_head == _tx_buffer_tail) {
        // Buffer empty, so disable "data register empty" interrupt
        USART_ITConfig(this->_hwserial_module, USART_IT_TXE, DISABLE);
    }
}

bool get_isr_enabled() {
  uint32_t result;
  __asm volatile ( "csrr %0," "0x800" : "=r" (result) );
  return (result == 0x6088);
}

// Private Methods
// To invoke data empty "interrupt" via a call, use this method
void UartClass::_poll_tx_data_empty(void)
{
    if ( (!(get_isr_enabled())) || !(this->_hwserial_module->CTLR1 & USART_CTLR1_TXEIE)) {
        // Interrupts are disabled either globally or for data register empty,
        // so we'll have to poll the "data register empty" flag ourselves.
        // If it is set, pretend an interrupt has happened and call the handler
        //to free up space for us.

        // Invoke interrupt handler only if conditions data register is empty
        if (USART_GetFlagStatus(this->_hwserial_module, USART_FLAG_TXE) == SET) {
            _tx_data_empty_irq();
        }
    }
    // In case interrupts are enabled, the interrupt routine will be invoked by itself
}

void UartClass::activate_uart_clock() {
    switch((uintptr_t)(this->_hwserial_module)) {
        case USART1_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            break;
        case USART2_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            break;
        case USART3_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            break;
        case UART4_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            break;
        case UART5_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
            break;
        case UART6_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART6, ENABLE);
            break;
        case UART7_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
            break;
        case UART8_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
            break;
        default:
            /* we don't know */
            break;
    }    
}

// Public Methods //////////////////////////////////////////////////////////////

void UartClass::begin(unsigned long baud, uint16_t config)
{
    // Make sure no transmissions are ongoing and USART is disabled in case begin() is called by accident
    // without first calling end()
    if(_written) {
        this->end();
    }

    USART_InitTypeDef USART_InitStructure;
    this->activate_uart_clock(); 

    pinmap_initgpio(this->_hwserial_tx_pin, GPIO_Mode_AF_PP);
    pinmap_initgpio(this->_hwserial_rx_pin, GPIO_Mode_IPU);

    uint16_t word_len = config & SERIAL_DATA_MASK;
    uint16_t parity = config & SERIAL_PARITY_MASK;
    uint16_t stop_bits = config & SERIAL_STOP_BIT_MASK;
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = word_len == SERIAL_DATA_8 ? USART_WordLength_8b : USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = stop_bits == SERIAL_STOP_BIT_1 ? USART_StopBits_1 : USART_StopBits_2;
    USART_InitStructure.USART_Parity = 
        parity == SERIAL_PARITY_NONE ? USART_Parity_No :
        parity == SERIAL_PARITY_EVEN ? USART_Parity_Even : USART_Parity_Odd;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(this->_hwserial_module, &USART_InitStructure);
    USART_Cmd(this->_hwserial_module, ENABLE);

    // activate receive interrupt
    USART_ITConfig(this->_hwserial_module, USART_IT_RXNE, ENABLE);

    NVIC_EnableIRQ(USART1_IRQn); // actually enable interrupts

    _written = false;
}

void UartClass::end()
{
    // wait for transmission of outgoing data
    flush();

    // Disable receiver and transmitter as well as the RX complete and
    // data register empty interrupts.
    USART_ITConfig(this->_hwserial_module, USART_IT_RXNE | USART_IT_TXE, DISABLE);
    USART_Cmd(this->_hwserial_module, DISABLE);
    NVIC_DisableIRQ(USART1_IRQn);

    // clear any received data
    _rx_buffer_head = _rx_buffer_tail;

    _written = false;
}

int UartClass::available(void)
{
    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

int UartClass::peek(void)
{
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        return _rx_buffer[_rx_buffer_tail];
    }
}

int UartClass::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

int UartClass::availableForWrite(void)
{
    tx_buffer_index_t head;
    tx_buffer_index_t tail;

    TX_BUFFER_ATOMIC {
        head = _tx_buffer_head;
        tail = _tx_buffer_tail;
    }
    if (head >= tail) return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
    return tail - head - 1;
}

void UartClass::flush()
{
    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXCIF (transmit
    // complete) bit to 1 during initialization
    if (!_written) {
        return;
    }

    // Spin until the data-register-empty-interrupt is disabled and TX complete interrupt flag is raised
    while ( (this->_hwserial_module->CTLR1 & USART_CTLR1_TCIE) || (USART_GetFlagStatus(this->_hwserial_module, USART_FLAG_TC) == RESET)) {

        // If interrupts are globally disabled or the and DR empty interrupt is disabled,
        // poll the "data register empty" interrupt flag to prevent deadlock
        _poll_tx_data_empty();
    }
    // If we get here, nothing is queued anymore (DREIE is disabled) and
    // the hardware finished transmission (TXCIF is set).
}

size_t UartClass::write(uint8_t c)
{
    _written = true;

    // If the buffer and the data register is empty, just write the byte
    // to the data register and be done. This shortcut helps
    // significantly improve the effective data rate at high (>
    // 500kbit/s) bit rates, where interrupt overhead becomes a slowdown.
    if ( (_tx_buffer_head == _tx_buffer_tail) && (USART_GetFlagStatus(this->_hwserial_module, USART_FLAG_TXE) == SET)) {
        USART_SendData(this->_hwserial_module, c);
        //(*_hwserial_module).STATUS = USART_TXCIF_bm;

        // Make sure data register empty interrupt is disabled to avoid
        // that the interrupt handler is called in this situation
        USART_ITConfig(this->_hwserial_module, USART_IT_TXE, DISABLE);
        //(*_hwserial_module).CTRLA &= (~USART_DREIE_bm);

        return 1;
    }

    tx_buffer_index_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;

    //If the output buffer is full, there's nothing for it other than to
    //wait for the interrupt handler to empty it a bit (or emulate interrupts)
    while (i == _tx_buffer_tail) {
        _poll_tx_data_empty();
    }

    _tx_buffer[_tx_buffer_head] = c;
    _tx_buffer_head = i;

    // Enable data "register empty interrupt"
    USART_ITConfig(this->_hwserial_module, USART_IT_TXE, ENABLE);

    return 1;
}

#endif // whole file