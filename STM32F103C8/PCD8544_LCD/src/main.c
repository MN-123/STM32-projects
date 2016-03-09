/**
 * @author  SirVolta
 * @ide     GNU ARM Eclipse
 * @brief   PCD8544 SPI LCD scrolling text
 */

#include <stdio.h>
#include <stdlib.h>
#include "stm32f10_pcd8544.h"
#include "diag/Trace.h"
#include "stm32f10x_conf.h"

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

 //Initialize LCD with 0x38 software contrast
 PCD8544_Init (0x38);
 //PCD8544_Invert(PCD8544_Invert_Yes);

 PCD8544_GotoXY (0, PCD8544_HEIGHT - 8);
 PCD8544_Puts ("PCD8544 LCD", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
 PCD8544_Refresh ();
 for (uint32_t i = 0; i < 0xF00000; i++);

 char buf[25];
 uint32_t i = 0;
 while (1)
  {
   GPIO_ToggleBits(LEDPORT, LEDPIN);
   for (uint32_t i = 0; i < 0x600000; i++); // waste time

   sprintf (buf, "%lu", i++);
   PDC8544ShiftFrameBuffer (8);
   //PCD8544HorizontalLine(PCD8544_HEIGHT - 10)
   PCD8544_GotoXY (0, PCD8544_HEIGHT - 8);
   PCD8544_Puts (buf, PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
   PCD8544_Refresh ();
  }
}
