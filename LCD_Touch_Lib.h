/*!	\file	LCD_Touch_Lib.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	New LCD and Touchscreen library for STM32F4Discovery with SSD2119 (LCD) and STMPE811QTR (Touch) (v2)
			Implements screen layers, touch management.
			The currently supported widgets are pages, buttons, textlines, labels
*/

#ifndef LCD_TOUCH_LIB_H_
#define LCD_TOUCH_LIB_H_

#include "STMPE811QTR.h"
#include "stm32f4_discovery_lcd.h"

#include "fonts.h"
#include "LCD_Lib_Layers.h"
#include "LCD_Colors.h"


/*! \def	RGB_2_565(r, g, b)
    \brief	RGB 888 to RGB 565 Conversion Macro
*/
#define RGB_2_565(r, g, b)	(((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) 

/*! \fn 	void initLCD_Touch()
    \brief  Initializes the LCD display and the touchscreen
*/
void initLCD_Touch();

/*! \fn 	WIDGET_DESC *newPage(WIDGET *page, uint16_t color);
    \brief  Creates a new page
	\param  page	Page widget
	\param	color	background color
	\return Pointer to the widget descriptor
*/
WIDGET_DESC *newPage(WIDGET *page, uint16_t color);

/*! \fn 	WIDGET_DESC *newButton(WIDGET *btn, uint16_t x_tl, uint16_t y_tl, uint16_t width,  uint16_t height, uint8_t* icon, HANDLER handler, void *handler_arg, WIDGET *father)
    \brief  Creates a new button
	\param  btn			Button to create
	\param	x_tl		Top right corner x coordinate
	\param	y_tl		Top right corner y coordinate
	\param	height		Height
	\param	width		Width
	\param	icon		Icon of the widget, array of characters in BMP565 format: if null, the widget's area is associated to the page background
	\param	handler		Handler function of the widget
	\param	handler_arg	Handler function argument of the widget
	\param	father 		Father widget (if not specified, the father is automatically detected according to the widgets coordinates)
	\return Pointer to the widget descriptor
*/
WIDGET_DESC *newButton(WIDGET *btn, uint16_t x_tl, uint16_t y_tl, uint16_t width,  uint16_t height, uint8_t* icon, HANDLER handler, void *handler_arg, WIDGET *father);

/*! \fn 	WIDGET_DESC *newLabel(WIDGET *label, uint16_t x, uint16_t y, uint16_t textColor, sFONT *font, char *text, WIDGET *father)
    \brief  Creates a new label
	\param  label		Label to create
	\param	x			Top right corner x coordinate
	\param	y			Top right corner y coordinate
	\param	textColor	Textcolor, in 565 format (16 bit integer)
	\param	font		Text font
	\param	text		Text of the label
	\param	father 		Father widget (if not specified, the father is automatically detected according to the widgets coordinates)
	\return Pointer to the widget descriptor
*/
WIDGET_DESC *newLabel(WIDGET *label, uint16_t x, uint16_t y, uint16_t textColor, sFONT *font, char *text, WIDGET *father);

/*! \fn 	WIDGET_DESC *newTextLine(WIDGET *textLine, uint16_t x_tl, uint16_t y_tl,  uint16_t width, uint16_t height, uint16_t textColor, uint16_t borderColor, uint16_t backgroundColor, sFONT *font, char *input_port, uint16_t input_port_size, WIDGET *father)
    \brief  Creates a new textline
	\param  label			Label to create
	\param	x_tl			Top right corner x coordinate
	\param	y_tl			Top right corner y coordinate
	\param	height			Height
	\param	width			Width
	\param	textColor		Textcolor, in 565 format (16 bit integer)
	\param	borderColor		Textcolor, in 565 format (16 bit integer)
	\param	backgroundColor	Textcolor, in 565 format (16 bit integer)
	\param	font			Text font
	\param	input_port		Pointer to an input port string
	\param	input_port_size	Maximum size of the input port string
	\param	father 			Father widget (if not specified, the father is automatically detected according to the widgets coordinates)
	\return Pointer to the widget descriptor
*/
WIDGET_DESC *newTextLine(WIDGET *textLine, uint16_t x_tl, uint16_t y_tl,  uint16_t width, uint16_t height, uint16_t textColor, uint16_t borderColor, uint16_t backgroundColor, sFONT *font, char *input_port, uint16_t input_port_size, WIDGET *father);

/*! \fn		void pollingTouch(POINT *pnt)
    \brief  Waits for a pression on the touchscreen and return the point in which is pressed (blocking)
	\param  area	Area of the screen
	\param	point	Point to check
*/
void pollingTouch(POINT *pnt);

/*! \fn		void getTouch(POINT *pnt)
    \brief  Waits for a pression on the touchscreen and return the point in which is pressed (not blocking)
	\param  area	Area of the screen
	\param	point	Point to check
*/
void getTouch(POINT *pnt);

/*! \fn		void enqueueTouchEvents(POINT detected)
    \brief  Enqueue a touch event, after having been detected the pressed widget
	\param  detected	point returned by the touchscreen
*/
void enqueueTouchEvents(POINT detected);

/*! \fn		void dispatchTouchEvents()
    \brief  Dispatches a touch event taken from the queue, if any
*/
void dispatchTouchEvents();

/*! \fn		void drawWidget(WIDGET *widget)
    \brief  Draws the given widget and all the widgets in its sub-tree
	\param  widget	widget object
*/
void drawWidget(WIDGET *widget);

/*! \fn		void drawCurrentPage()
    \brief  Draws the current page
*/
void drawCurrentPage();

/*! \fn		uint8_t setPage(uint8_t pageIndex)
    \brief  Sets the page with index equal to pageIndex as activePage
	\return -1 if the page does not exists, 1 otherwise
*/
uint8_t setPage(uint8_t pageIndex);

/*! \fn		void changePage(uint8_t page_index)
    \brief  Changes and draw the page with index page_index
    \return -1 if the given page does not exists
*/
uint8_t changePage(uint8_t page_index);

/*! \fn		uint8_t textLine_setText(WIDGET *textLine, char newText[])
    \brief  Set the text of a given textline
	\param  textLine	textline widget
	\param	newText		text to be copied
	\return -1 if the given text is too long for the textline, 1 otherwise
*/
uint8_t textLine_setText(WIDGET *textLine, char newText[]);

/*! \fn		uint8_t textLine_setAndDisplayText(WIDGET *textLine, char newText[])
    \brief  Set the text of a given textline and updates the screen
	\param  textLine	textline widget
	\param	newText		text to be copied
	\return -1 if the given text is too long for the textline, 1 otherwise
*/
uint8_t textLine_setAndDisplayText(WIDGET *textLine, char newText[]);

/*! \fn		uint8_t textLine_addText(WIDGET *textLine, char newText[])
    \brief  Concatenates the given text with the one already contained in the given textline
	\param  textLine	textline widget
	\param	newText		text to be concatenated
	\return -1 if the text becomes to long for the textline, 1 otherwise
*/
uint8_t textLine_addText(WIDGET *textLine, char newText[]);

/*! \fn		uint8_t textLine_addAndDisplayText(WIDGET *textLine, char newText[])
    \brief  Concatenates the given text with the one already contained in the given textline and updates the screen
	\param  textLine	textline widget
	\param	newText		text to be concatenated
	\return -1 if the text becomes to long for the textline, 1 otherwise
*/
uint8_t textLine_addAndDisplayText(WIDGET *textLine, char newText[]);

/*! \fn		void textLine_eraseText(WIDGET *textLine)
    \brief  Erases the textline widget text
	\param  textLine	textline widget
*/
void textLine_eraseText(WIDGET *textLine);

/*! \fn		void textLine_eraseAndDisplayText(WIDGET *textLine)
    \brief  Erases the textline widget text and updates the screen
	\param  textLine	textline widget
*/
void textLine_eraseAndDisplayText(WIDGET *textLine);

/*! \fn		uint8_t textLine_removeLastChar(WIDGET *textLine)
    \brief  Remove the last character in the textline
	\param  textLine	textline widget
	\return -1 if there is no character to remove, 1 otherwise
*/
uint8_t textLine_removeLastChar(WIDGET *textLine);

/*! \fn		uint8_t textLine_removeLastCharAndDisplayText(WIDGET *textLine)
    \brief  Remove the last character in the textline and updates the screen
	\param  textLine	textline widget
	\return -1 if there is no character to remove, 1 otherwise
*/
uint8_t textLine_removeLastCharAndDisplayText(WIDGET *textLine);



#endif /* LCD_TOUCH_LIB_H_ */
