/*!	\file	LCD_Touch_Lib.c
 	\author Daniel Casini
 	\date	05/03/2014
	\brief	LCD and Touchscreen library for STM32F4Discovery with SSD2119 (LCD) and STMPE811QTR (Touch)
*/

#include "LCD_Touch_Lib.h"
#include "stm32f4_discovery_lcd.h"
#include "string.h"
#include "stdio.h"

uint8_t activePage;		/// Index of the currently active page
uint8_t numPages;		/// Current number of LCD pages
uint8_t numWidgets;		/// Current number of Widgets
uint8_t numButtons;		/// Current number of Buttons
uint8_t numLabels;		/// Current number of Labels
uint8_t numTextLines;	/// Current number of TextLines

WIDGET_DESC 	*LCD_pages[NUM_PAGES]; 		/// LCD Pages pointer
WIDGET_DESC 	widgetList[NUMWLIST];		/// Widget List variables
LCD_PAGE	 	pageData[NUM_PAGES];		/// LCD Pages variables
BUTTON		 	buttonData[NUM_BUTTONS];	/// Button Data variables
LABEL	 		labelData[NUM_LABELS];		/// Label Data variables
TEXT_LINE		textLineData[NUM_TEXTLINE];	/// Textline Data variables

EVENT_QUEUE_T eventQueue;	///Touch event queue


void initLCD_Touch(){

	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();
	/* Clear the LCD */
	LCD_Clear(White);

	numWidgets = 0;
	numButtons = 0;
	numPages = 0;
	activePage = 0;
	numLabels = 0;
	numTextLines = 0;
}



WIDGET_DESC *newButton(WIDGET *btn, uint16_t x_tl, uint16_t y_tl,  uint16_t width, uint16_t height, uint8_t* icon, HANDLER handler, void *handler_arg, WIDGET *father){

	if(numWidgets > NUMWLIST || numButtons > NUM_BUTTONS)
		return (WIDGET_DESC *)ERR;

	if( (x_tl < 0) || ((x_tl + width) > LCD_WIDTH) ||
		(y_tl < 0) || ((y_tl + height) > LCD_HEIGHT))
			return (WIDGET_DESC *)ERR;

	btn->wtype = BUTTON_TYPE;
	btn->id = numWidgets;

	btn->bounds.tl_corner.x = x_tl;
	btn->bounds.tl_corner.y = y_tl;
	btn->bounds.br_corner.x = x_tl + width - 1;
	btn->bounds.br_corner.y = y_tl + height - 1;
	BUTTON *bData;
	bData = &buttonData[numButtons];
	btn->handler = handler;
	btn->handler_arg = handler_arg;
	bData->icon = icon;
	btn->wdata = (void *)bData;

	widgetList[numWidgets].data = btn;
	widgetList[numWidgets].child = 0;
	widgetList[numWidgets].sibling = 0;

	insertWidget(btn, father);

	numButtons++;

	return &widgetList[numWidgets++];
}



WIDGET_DESC *newPage(WIDGET *page, uint16_t color){

	page->wtype = LCD_PAGE_TYPE;
	page->bounds.tl_corner.x = 0;
	page->bounds.tl_corner.y = 0;
	page->bounds.br_corner.x = LCD_WIDTH;
	page->bounds.br_corner.y = LCD_HEIGHT;
	page->id = numWidgets;
	page->handler = 0; //no handler

	LCD_PAGE *pData;
	pData = &pageData[numPages];
	pData->backgroundObj.background.color = color;
	pData->backgroundObj.background_type = BACKGROUND_COLOR;

	page->wdata = (void *) pData;

	widgetList[numWidgets].data = page;
	widgetList[numWidgets].child = 0;
	widgetList[numWidgets].sibling = 0;

	LCD_pages[numPages] = &widgetList[numWidgets];
	numPages++;

	return &widgetList[numWidgets++];
}

WIDGET_DESC *newTextLine(WIDGET *textLine, uint16_t x_tl, uint16_t y_tl,  uint16_t width, uint16_t height, uint16_t textColor, uint16_t borderColor, uint16_t backgroundColor, sFONT *font, char *input_port, uint16_t input_port_size, WIDGET *father){

		if(numWidgets > NUMWLIST || numTextLines > NUM_TEXTLINE)
			return (WIDGET_DESC *)ERR;

		if( (x_tl < 0) || ((x_tl + width) > LCD_WIDTH) ||
			(y_tl < 0) || ((y_tl + height) > LCD_HEIGHT))
				return (WIDGET_DESC *)ERR;

		textLine->wtype = TEXTLINE_TYPE;
		textLine->id = numWidgets;

		textLine->bounds.tl_corner.x = x_tl;
		textLine->bounds.tl_corner.y = y_tl;
		textLine->bounds.br_corner.x = x_tl + width - 1;
		textLine->bounds.br_corner.y = y_tl + height - 1;

		TEXT_LINE *tData;
		tData = &textLineData[numTextLines];
		tData->background_color = backgroundColor;
		tData->border_color = borderColor;
		tData->text_color = textColor;
		tData->font = font;
		tData->textline_len = input_port_size;
		bzero((void*)input_port, sizeof(input_port));
		tData->textline = input_port;
		textLine->wdata = (void *)tData;

		widgetList[numWidgets].data = textLine;
		widgetList[numWidgets].child = 0;
		widgetList[numWidgets].sibling = 0;

		insertWidget(textLine, father);

		numTextLines++;

		return &widgetList[numWidgets++];
}



WIDGET_DESC *newLabel(WIDGET *label, uint16_t x, uint16_t y, uint16_t textColor, sFONT *font, char *text, WIDGET *father){

	if(numWidgets > NUMWLIST || numLabels > NUM_LABELS)
		return (WIDGET_DESC *)ERR;

	label->wtype = LABEL_TYPE;
	label->bounds.tl_corner.x = x;
	label->bounds.tl_corner.y = y;
	label->id = numWidgets;
	label->handler = 0; //no handler

	LABEL *lData;
	lData = &labelData[numLabels];
	lData->text_color = textColor;
	lData->font = font;
	strcpy(lData->text, text);

	label->wdata = (void *)lData;

	widgetList[label->id].data = label;
	widgetList[label->id].child = 0;
	widgetList[label->id].sibling = 0;

	/*if(pData->backgroundObj.background_type == COLOR)
		LCD_Clear(pData->backgroundObj.background.color);
	else //draw image
		LCD_DrawPicture(0, 0, LCD_LIB_SCREEN_W, LCD_LIB_SCREEN_H, pData->backgroundObj.background.image);*/

	//LCD_SetTextColor(pData->textColor);
	insertWidget(label, father);
	numLabels++;

	return &widgetList[numWidgets++];
}




void enqueueTouchEvents(POINT detected){
		WIDGET *widget;
		WIDGET_DESC *desc;

		// else desc->child == 0, so the father object is the target
		/*while(desc->child != 0 && contains(&desc->data->bounds, &detected))
			desc = desc->child; REMOVE */
		desc = findPressedWidget(&detected);
		if(desc != 0){
			widget = desc->data;
			if(eventQueue.counter != MAX_EVENTS_ENQUEUED){
				eventQueue.widgetID[eventQueue.write_index] = widget->id;
				eventQueue.write_index = (eventQueue.write_index + 1) % MAX_EVENTS_ENQUEUED;
				eventQueue.counter++;
			}
		}
}

void dispatchTouchEvents(){
	uint8_t id;
	if(eventQueue.counter != 0){
		id = eventQueue.widgetID[eventQueue.read_index];
		eventQueue.read_index = (eventQueue.read_index + 1) % MAX_EVENTS_ENQUEUED;
		eventQueue.counter--;
		if(widgetList[id].data->handler != 0)
			(widgetList[id].data->handler)(widgetList[id].data->handler_arg);
	}
}

void pollingTouch(POINT *pnt){

	  TS_STATE *pstate = 0;

		do {
		  pstate = IOE_TS_GetState();
		  delay(10);
		} while(!pstate->TouchDetected);
		pnt->x = pstate->X;
		pnt->y = pstate->Y;

		/* If AD result is abnormal, then LCD show nothing */
		if ((pnt->x > TOUCH_AD_VALUE_MAX)
			|| (pnt->x < TOUCH_AD_VALUE_MIN)
			|| (pnt->y > TOUCH_AD_VALUE_MAX)
			|| (pnt->y < TOUCH_AD_VALUE_MIN)) {
		  return;
		}
		/*
		/*Calculate coordinates*/
		pnt->x = ((int)(pnt->x * X_SCALE - X_OFFSET));
		pnt->y = ((int)(pnt->y * Y_SCALE - Y_OFFSET));

		if (pnt->x >= LCD_WIDTH) {
		  pnt->x = LCD_WIDTH - 1;
		}
		if (pnt->y >= LCD_HEIGHT) {
		  pnt->y = LCD_HEIGHT - 1;
		}
		return;
}


void drawWidget(WIDGET *widget){
	WIDGET_DESC *desc = &widgetList[widget->id];
	drawWidgets(desc);
}

void getTouch(POINT *pnt){

	  TS_STATE *pstate = 0;

		pstate = IOE_TS_GetState();
		pnt->x = pstate->X;
		pnt->y = pstate->Y;

		/* If AD result is abnormal, then LCD show nothing */
		if ((pnt->x > TOUCH_AD_VALUE_MAX)
			|| (pnt->x < TOUCH_AD_VALUE_MIN)
			|| (pnt->y > TOUCH_AD_VALUE_MAX)
			|| (pnt->y < TOUCH_AD_VALUE_MIN)) {
		  return;
		}
		/*
		/*Calculate coordinates*/
		pnt->x = ((int)(pnt->x * X_SCALE - X_OFFSET));
		pnt->y = ((int)(pnt->y * Y_SCALE - Y_OFFSET));

		if (pnt->x >= LCD_WIDTH) {
		  pnt->x = LCD_WIDTH - 1;
		}
		if (pnt->y >= LCD_HEIGHT) {
		  pnt->y = LCD_HEIGHT - 1;
		}
		return;
}

uint8_t textLine_setText(WIDGET *textLine, char newText[]){
	if(textLine->wtype != TEXTLINE_TYPE)
		return -1;
	TEXT_LINE *textData = (TEXT_LINE *)textLine->wdata;
	if(strlen(newText) > textData->textline_len)
		return -1;
	strcpy(textData->textline, newText);
	return 1;
}

uint8_t textLine_setAndDisplayText(WIDGET *textLine, char newText[]){
	if(textLine_setText(textLine, newText) == -1)
		return -1;
	else{
		WIDGET_DESC *text_desc = &widgetList[textLine->id];
		drawWidgets(text_desc);
		return 1;
	}
}

uint8_t textLine_addText(WIDGET *textLine, char newText[]){
	if(textLine->wtype != TEXTLINE_TYPE)
		return -1;
	TEXT_LINE *textData = (TEXT_LINE *)textLine->wdata;
	if((strlen(newText) + strlen(textData->textline)) > textData->textline_len)
		return -1;
	sprintf(textData->textline, "%s%s", textData->textline, newText);
	return 1;
}

uint8_t textLine_removeLastChar(WIDGET *textLine){
	if(textLine->wtype != TEXTLINE_TYPE)
		return -1;
	TEXT_LINE *textData = (TEXT_LINE *)textLine->wdata;
	if(strlen(textData->textline) == 0)
		return -1;
	uint8_t index = strlen(textData->textline) - 1;
	textData->textline[index] = '\0';
	return 1;
}

uint8_t textLine_removeLastCharAndDisplayText(WIDGET *textLine){
	if(textLine_removeLastChar(textLine) == -1)
		return -1;
	else{
		WIDGET_DESC *text_desc = &widgetList[textLine->id];
		drawWidgets(text_desc);
		return 1;
	}
}

uint8_t textLine_addAndDisplayText(WIDGET *textLine, char newText[]){
	if(textLine_addText(textLine, newText) == -1)
		return -1;
	else{
		WIDGET_DESC *text_desc = &widgetList[textLine->id];
		drawWidgets(text_desc);
		return 1;
	}
}

void textLine_eraseText(WIDGET *textLine){
	TEXT_LINE *textData = (TEXT_LINE *)textLine->wdata;
	bzero(textData->textline, textData->textline_len);
}

void textLine_eraseAndDisplayText(WIDGET *textLine){
	textLine_eraseText(textLine);
	WIDGET_DESC *text_desc = &widgetList[textLine->id];
	drawWidgets(text_desc);
}

void drawCurrentPage(){
	drawWidgets(LCD_pages[activePage]);
}

uint8_t changePage(uint8_t page_index){

	if(page_index >= numPages)
		return -1;

	//changes the active page
	activePage = page_index;
	//draws the page
	drawCurrentPage();

}

uint8_t setPage(uint8_t page_index){

	if(page_index >= numPages)
		return -1;

	//changes the active page
	activePage = page_index;

}
