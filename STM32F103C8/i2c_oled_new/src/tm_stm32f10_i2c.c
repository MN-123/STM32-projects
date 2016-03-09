/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f10_i2c.h"
#include "assert.h"
#include "diag/Trace.h"

/* Private variables */
static uint32_t TM_I2C_Timeout;
static uint32_t TM_I2C_INT_Clocks[3] =
 { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

void
TM_I2C_Init (I2C_TypeDef* I2Cx, uint32_t clockSpeed, uint8_t alternate)
{
 I2C_InitTypeDef I2C_InitStruct;
 GPIO_InitTypeDef GPIO_InitStruct;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 //both I2C devices uses PORTB
 RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
 RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);

 if (I2Cx == I2C1)
  {
   /* Enable clock */
   RCC_APB1PeriphClockCmd (RCC_APB1ENR_I2C1EN, ENABLE);

   /* Enable pins */
   if (alternate)
    {
     //SCL, SDA
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
     GPIO_PinRemapConfig (GPIO_Remap_I2C1, ENABLE);
    }
   else
    {
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    }
   GPIO_Init (GPIOB, &GPIO_InitStruct);

   /* Check clock, set the lowest clock your devices support on the same I2C but */
   if (clockSpeed < TM_I2C_INT_Clocks[0])
    {
     TM_I2C_INT_Clocks[0] = clockSpeed;
    }

   /* Set values */
   I2C_InitStruct.I2C_ClockSpeed = TM_I2C_INT_Clocks[0];
   I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C1_ACKNOWLEDGED_ADDRESS;
   I2C_InitStruct.I2C_Mode = TM_I2C1_MODE;
   I2C_InitStruct.I2C_OwnAddress1 = TM_I2C1_OWN_ADDRESS;
   I2C_InitStruct.I2C_Ack = TM_I2C1_ACK;
   I2C_InitStruct.I2C_DutyCycle = TM_I2C1_DUTY_CYCLE;
  }

 else if (I2Cx == I2C2)
  {
   /* Enable clock */
   RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

   /* Enable pins, note that I2C2 has no alternate pins on this device */
   //SCL, SDA
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_Init (GPIOB, &GPIO_InitStruct);

   /* Check clock, set the lowest clock your devices support on the same I2C but */
   if (clockSpeed < TM_I2C_INT_Clocks[1])
    {
     TM_I2C_INT_Clocks[1] = clockSpeed;
    }

   /* Set values */
   I2C_InitStruct.I2C_ClockSpeed = TM_I2C_INT_Clocks[1];
   I2C_InitStruct.I2C_AcknowledgedAddress = TM_I2C2_ACKNOWLEDGED_ADDRESS;
   I2C_InitStruct.I2C_Mode = TM_I2C2_MODE;
   I2C_InitStruct.I2C_OwnAddress1 = TM_I2C2_OWN_ADDRESS;
   I2C_InitStruct.I2C_Ack = TM_I2C2_ACK;
   I2C_InitStruct.I2C_DutyCycle = TM_I2C2_DUTY_CYCLE;
  }

 /* Disable I2C first */
 I2C_Cmd (I2Cx, DISABLE);
 I2C_DeInit (I2Cx);

 /* Initialize I2C */
 I2C_Init (I2Cx, &I2C_InitStruct);

 /* Enable I2C */
 I2Cx->CR1 |= I2C_CR1_PE;
}

uint8_t
TM_I2C_Read (I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg)
{
 uint8_t received_data;
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 TM_I2C_WriteData (I2Cx, reg);
 TM_I2C_Stop (I2Cx);
 TM_I2C_Start (I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
 received_data = TM_I2C_ReadNack (I2Cx);
 return received_data;
}

void
TM_I2C_ReadMulti (I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg,
                  uint8_t* data, uint16_t count)
{
 uint8_t i;
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
 TM_I2C_WriteData (I2Cx, reg);
 //TM_I2C_Stop(I2Cx);
 TM_I2C_Start (I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
 for (i = 0; i < count; i++)
  {
   if (i == (count - 1))
    {
     /* Last byte */
     data[i] = TM_I2C_ReadNack (I2Cx);
    }
   else
    {
     data[i] = TM_I2C_ReadAck (I2Cx);
    }
  }
}

uint8_t
TM_I2C_ReadNoRegister (I2C_TypeDef* I2Cx, uint8_t address)
{
 uint8_t data;
 TM_I2C_Start (I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
 /* Also stop condition happens */
 data = TM_I2C_ReadNack (I2Cx);
 return data;
}

void
TM_I2C_ReadMultiNoRegister (I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data,
                            uint16_t count)
{
 uint8_t i;
 TM_I2C_Start (I2Cx, address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
 for (i = 0; i < count; i++)
  {
   if (i == (count - 1))
    {
     /* Last byte */
     data[i] = TM_I2C_ReadNack (I2Cx);
    }
   else
    {
     data[i] = TM_I2C_ReadAck (I2Cx);
    }
  }
}

void
TM_I2C_Write (I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data)
{
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 TM_I2C_WriteData (I2Cx, reg);
 TM_I2C_WriteData (I2Cx, data);
 TM_I2C_Stop (I2Cx);
}

void
TM_I2C_WriteMulti (I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg,
                   uint8_t* data, uint16_t count)
{
 uint16_t i;
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 TM_I2C_WriteData (I2Cx, reg);
 for (i = 0; i < count; i++)
  {
   TM_I2C_WriteData (I2Cx, data[i]);
  }
 TM_I2C_Stop (I2Cx);
}

int16_t
TM_I2C_WriteMultiDMA (I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg,
                      uint16_t len)
{
 int16_t ok = 0;
 // If DMA is already enabled, wait for it to complete first.
 // Interrupt will disable this after transmission is complete.
 TM_I2C_Timeout = 10000000;
 while ((SSD1306_DMA->CCR & DMA_CCR1_EN) && TM_I2C_Timeout)
  {
   if (--TM_I2C_Timeout == 0)
    {
     return -1;
    }
  }
 //Set amount of bytes to transfer, then enable DMA
 DMA_Cmd (SSD1306_DMA, DISABLE); //should already be disabled at this point
 DMA_SetCurrDataCounter (SSD1306_DMA, len);
 DMA_Cmd (SSD1306_DMA, ENABLE);
 //Send I2C start
 ok = TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 //Send register to write to
 TM_I2C_WriteData (I2Cx, reg);
 //Start DMA transmission, interrupt will handle transmit complete.
 I2C_DMACmd (I2Cx, ENABLE);
 return ok;
}

void
TM_I2C_WriteNoRegister (I2C_TypeDef* I2Cx, uint8_t address, uint8_t data)
{
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 TM_I2C_WriteData (I2Cx, data);
 TM_I2C_Stop (I2Cx);
}

void
TM_I2C_WriteMultiNoRegister (I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data,
                             uint16_t count)
{
 uint8_t i;
 TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 for (i = 0; i < count; i++)
  {
   TM_I2C_WriteData (I2Cx, data[i]);
  }
 TM_I2C_Stop (I2Cx);
}

int16_t
TM_I2C_WriteMultiNoRegDMA (I2C_TypeDef* I2Cx, uint8_t address, uint16_t len)
{
 int16_t ok = 0;
 // If DMA is already enabled, wait for it to complete first.
 // Interrupt will disable this after transmission is complete.
 TM_I2C_Timeout = 10000000;
 while ((SSD1306_DMA->CCR & DMA_CCR1_EN) && TM_I2C_Timeout)
  {
   if (--TM_I2C_Timeout == 0)
    {
     return -1;
    }
  }
 //Set amount of bytes to transfer, then enable DMA
 DMA_Cmd (SSD1306_DMA, DISABLE); //should already be disabled at this point
 DMA_SetCurrDataCounter (SSD1306_DMA, len);
 DMA_Cmd (SSD1306_DMA, ENABLE);
 //Send I2C start
 ok = TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
 //Start DMA transmission, interrupt will handle transmit complete.
 I2C_DMACmd (I2Cx, ENABLE);
 return ok;
}

/* Private functions */
int16_t
TM_I2C_Start (I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction,
              uint8_t ack)
{
 /* Generate I2C start pulse */
 I2Cx->CR1 |= I2C_CR1_START;

 /* Wait till I2C is busy */
 TM_I2C_Timeout = TM_I2C_TIMEOUT;
 while (!(I2Cx->SR1 & I2C_SR1_SB))
  {
   if (--TM_I2C_Timeout == 0x00)
    {
     return 1;
    }
  }

 /* Enable ack if we select it */
 if (ack)
  {
   I2Cx->CR1 |= I2C_CR1_ACK;
  }

 /* Send write/read bit */
 if (direction == I2C_TRANSMITTER_MODE)
  {
   /* Send address with zero last bit */
   I2Cx->DR = address & ~I2C_OAR1_ADD0;

   /* Wait till finished */
   TM_I2C_Timeout = TM_I2C_TIMEOUT;
   while (!(I2Cx->SR1 & I2C_SR1_ADDR))
    {
     if (--TM_I2C_Timeout == 0x00)
      {
       return 1;
      }
    }
  }
 if (direction == I2C_RECEIVER_MODE)
  {
   /* Send address with 1 last bit */
   I2Cx->DR = address | I2C_OAR1_ADD0;

   /* Wait till finished */
   TM_I2C_Timeout = TM_I2C_TIMEOUT;
   while (!I2C_CheckEvent (I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
     if (--TM_I2C_Timeout == 0x00)
      {
       return 1;
      }
    }
  }

 /* Read status register to clear ADDR flag */
 I2Cx->SR2;

 /* Return 0, everything ok */
 return 0;
}

void
TM_I2C_WriteData (I2C_TypeDef* I2Cx, uint8_t data)
{
 /* Wait till I2C is not busy anymore */
 TM_I2C_Timeout = TM_I2C_TIMEOUT;
 while (!(I2Cx->SR1 & I2C_SR1_TXE) && TM_I2C_Timeout)
  {
   TM_I2C_Timeout--;
  }

 /* Send I2C data */
 I2Cx->DR = data;
}

uint8_t
TM_I2C_ReadAck (I2C_TypeDef* I2Cx)
{
 uint8_t data;

 /* Enable ACK */
 I2Cx->CR1 |= I2C_CR1_ACK;

 /* Wait till not received */
 TM_I2C_Timeout = TM_I2C_TIMEOUT;
 while (!I2C_CheckEvent (I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
   if (--TM_I2C_Timeout == 0x00)
    {
     return 1;
    }
  }

 /* Read data */
 data = I2Cx->DR;

 /* Return data */
 return data;
}

uint8_t
TM_I2C_ReadNack (I2C_TypeDef* I2Cx)
{
 uint8_t data;

 /* Disable ACK */
 I2Cx->CR1 &= ~I2C_CR1_ACK;

 /* Generate stop */
 I2Cx->CR1 |= I2C_CR1_STOP;

 /* Wait till received */
 TM_I2C_Timeout = TM_I2C_TIMEOUT;
 while (!I2C_CheckEvent (I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
   if (--TM_I2C_Timeout == 0x00)
    {
     return 1;
    }
  }

 /* Read data */
 data = I2Cx->DR;

 /* Return data */
 return data;
}

uint8_t
TM_I2C_Stop (I2C_TypeDef* I2Cx)
{
 /* Wait till transmitter not empty */
 TM_I2C_Timeout = TM_I2C_TIMEOUT;
 while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF))))
  {
   if (--TM_I2C_Timeout == 0x00)
    {
     return 1;
    }
  }

 /* Generate stop */
 I2Cx->CR1 |= I2C_CR1_STOP;

 /* Return 0, everything ok */
 return 0;
}

uint8_t
TM_I2C_IsDeviceConnected (I2C_TypeDef* I2Cx, uint8_t address)
{
 uint8_t connected = 0;
 /* Try to start, function will return 0 in case device will send ACK */
 if (!TM_I2C_Start (I2Cx, address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE))
  {
   connected = 1;
  }

 /* STOP I2C */
 TM_I2C_Stop (I2Cx);

 /* Return status */
 return connected;
}

