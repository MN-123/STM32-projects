/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/05/library-61-ssd1306-oled-i2c-lcd-for-stm32f4xx
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library for 128x64 SSD1306 I2C LCD
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_SSD1306_H
#define TM_SSD1306_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup TM_SSD1306
 * @brief    Library for 128x64 SSD1306 I2C LCD - http://stm32f4-discovery.com/2015/05/library-61-ssd1306-oled-i2c-lcd-for-stm32f4xx
 * @{
 *
 * This SSD1306 LCD uses I2C for communication
 *
 * Library features functions for drawing lines, rectangles and circles.
 *
 * It also allows you to draw texts and characters using appropriate functions provided in library.
 *
 * \par Select custom I2C settings
 *
 * Use defines.h file for custom settings
 *
@verbatim
//Select custom I2C
#define SSD1306_I2C              I2C1

//Select custom I2C address 
#define SSD1306_I2C_ADDR         0x78

//Select custom width and height if your LCD differs in size
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F10xx
 - TM I2C
 - TM FONTS
 - string.h
 - stdlib.h
@endverbatim
 */

#include "stm32f10x.h"
#include "tm_stm32f10_i2c.h"
#include "tm_stm32f10_fonts.h"

#include <stdlib.h>
#include <string.h>

/**
 * @defgroup TM_SSD1306_Macros
 * @brief    Library defines
 * @{
 */


/**
 * @}
 */
 
/**
 * @defgroup TM_SSD1306_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  SSD1306 color enumeration
 */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

/**
 * @}
 */

/**
 * @defgroup TM_SSD1306_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
uint8_t TM_SSD1306_Init(void);

/**
 * @brief  Initializes DMA controller for SSD1306
 * @param  None
 */
void TM_SSD1306_initDMA(void);


/** 
 * @brief  Updates buffer from internal RAM to LCD
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
 * @param  None
 * @retval None
 */
void TM_SSD1306_UpdateScreen(void);

/**
 * @brief  Toggles pixels invertion inside internal RAM
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  None
 * @retval None
 */
void TM_SSD1306_ToggleInvert(void);

/** 
 * @brief  Fills entire LCD with desired color
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_Fill(SSD1306_COLOR_t Color);

/**
 * @brief  Draws pixel at desired location
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color); 

/**
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @retval None
 */
void TM_SSD1306_GotoXY(uint16_t x, uint16_t y);

/**
 * @brief  Puts character to internal RAM
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref TM_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char TM_SSD1306_Putc(char ch, TM_FontDef_t* Font, SSD1306_COLOR_t color);

/**
 * @brief  Puts string to internal RAM
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref TM_FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char TM_SSD1306_Puts(char* str, TM_FontDef_t* Font, SSD1306_COLOR_t color);

/**
 * @brief  Draws line on LCD
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);

/**
 * @brief  Draws rectangle on LCD
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);

/**
 * @brief  Draws filled rectangle on LCD
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);

/**
 * @brief  Draws triangle on LCD
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);

/**
 * @brief  Draws circle to STM buffer
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

/**
 * @brief  Draws filled circle to STM buffer
 * @note   @ref TM_SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void TM_SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

/**
 * @brief  Shifts the contents of the frame buffer up the specified
 * number of pixels
 * @note This implementation is very inefficient and needs a rewrite.
 * @param[in]  height
 * The number of pixels to shift the frame buffer up, leaving
 * a blank space at the bottom of the frame buffer x pixels high
 */
void SSD1306ShiftFrameBuffer (uint8_t height);


/**
 * @brief  Enables OLED and chargepump
 */
void SSD1306_ON(void);

/**
 * @brief  Puts display in low-power standby mode
 */
void SSD1306_OFF(void);


/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
