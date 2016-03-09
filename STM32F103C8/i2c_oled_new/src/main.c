/**
 * @author  SirVolta
 * @ide     GNU ARM Eclipse
 * @brief   128x64 SSD1306 I2C LCD using DMA, including shiftframebuffer
 */

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f10x_conf.h"
#include "tm_stm32f10_ssd1306.h"
#include "tm_stm32f10_fonts.h"

int
main ()
{
 trace_puts (PROJNAME);

 GPIO_InitTypeDef GPIO_InitStruct;

 //Led
 RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
 GPIO_InitStruct.GPIO_Pin = LEDPIN | GPIO_Pin_5;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
 GPIO_Init (LEDPORT, &GPIO_InitStruct);
 GPIO_SetBits (LEDPORT, LEDPIN); //led is active low
 
 // waste time to give oled a chance to power up
 for (uint32_t i = 0; i < 0x100000; i++);
 // initialize the OLED Display
 trace_puts (TM_SSD1306_Init () ? "OLED ready" : "OLED error");

 //Print text on bottom of screen so it can be shifted up
 TM_SSD1306_GotoXY (0, SSD1306_HEIGHT - 19);
 TM_SSD1306_Puts ("I2C OLED", &font_medium_11x18, SSD1306_COLOR_WHITE);
 TM_SSD1306_UpdateScreen ();

 char buf[10];
 uint16_t i = 0;
 while (1)
  {
   GPIO_ToggleBits(LEDPORT, LEDPIN);
   for (uint32_t i = 0; i < 0x600000; i++); // waste time

   sprintf (buf, "Count: %u", i++);
   SSD1306ShiftFrameBuffer (11);
   TM_SSD1306_GotoXY (0, SSD1306_HEIGHT - 11);
   TM_SSD1306_Puts (buf, &font_small_7x10, SSD1306_COLOR_WHITE);
   TM_SSD1306_UpdateScreen ();
  }
}
