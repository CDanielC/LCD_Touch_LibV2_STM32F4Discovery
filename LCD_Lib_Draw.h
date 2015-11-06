/*!	\file	LCD_Lib_Draw.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	LCD_Lib_Touch widget draw functions
*/

#ifndef LCD_LIB_DRAW_H_
#define LCD_LIB_DRAW_H_

#include "LCD_Lib_Types.h"

/*! \fn		void drawSingleWidget(WIDGET_DESC* widget)
    \brief  calls a specific function to draw a widget, according to the widget type
*/
void drawSingleWidget(WIDGET_DESC* widget);

/*! \fn		drawLCDPage(WIDGET_DESC* widget)
    \brief  draws an LCD page
*/
void drawLCDPage(WIDGET_DESC* widget);

/*! \fn		void drawLabel(WIDGET_DESC* widget)
    \brief  draws a label widget
*/
void drawLabel(WIDGET_DESC* widget);

/*! \fn		void drawButton(WIDGET_DESC* widget)
    \brief  draws a button widget
*/
void drawButton(WIDGET_DESC* widget);

/*! \fn		uint8_t drawTextLine(WIDGET_DESC* widget)
    \brief  draw a textline widget
    \return -1 if the the text is too long to be contained in the textbox, 1 otherwise
*/
uint8_t drawTextLine(WIDGET_DESC* widget);

#endif
