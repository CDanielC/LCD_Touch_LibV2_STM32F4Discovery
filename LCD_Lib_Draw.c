
#include "LCD_Lib_Draw.h"

void drawSingleWidget(WIDGET_DESC* widget){
	switch(widget->data->wtype){
		case LCD_PAGE_TYPE:	drawLCDPage(widget);
							break;
		case BUTTON_TYPE:	drawButton(widget);
							break;
		case LABEL_TYPE:	drawLabel(widget);
							break;
		case TEXTLINE_TYPE:	drawTextLine(widget);
							break;
	}
}

void drawLabel(WIDGET_DESC* widget){
	LABEL *lData;

	lData = (LABEL *) widget->data->wdata;
	POINT coord = widget->data->bounds.tl_corner;

	LCD_SetFont(lData->font);
	LCD_SetTextColor(lData->text_color);

	LCD_DisplayStringXY(coord.x, coord.y, lData->text, TRANSPARENT);
}

void drawLCDPage(WIDGET_DESC* widget){
	LCD_PAGE *pData;
	pData = (LCD_PAGE *) widget->data->wdata;
	if(pData->backgroundObj.background_type == BACKGROUND_IMAGE)
		LCD_DrawPicture(0, 0, LCD_WIDTH, LCD_HEIGHT, pData->backgroundObj.background.image);
	else
		LCD_Clear(pData->backgroundObj.background.color);
}

void drawButton(WIDGET_DESC* widget){

	uint16_t width, height;

	TOUCH_AREA btn_area = widget->data->bounds;
	BUTTON *bData = (BUTTON *)widget->data->wdata;

	width = btn_area.br_corner.x - btn_area.tl_corner.x + 1;
	height = btn_area.br_corner.y - btn_area.tl_corner.y + 1;

	if(bData->icon != 0)
			LCD_DrawPicture(btn_area.tl_corner.x, btn_area.tl_corner.y, width, height, bData->icon);
}

uint8_t drawTextLine(WIDGET_DESC* widget){

	uint16_t width, height;

	TOUCH_AREA textline_area = widget->data->bounds;
	TEXT_LINE *tData = (TEXT_LINE *)widget->data->wdata;

	width = textline_area.br_corner.x - textline_area.tl_corner.x + 1;
	height = textline_area.br_corner.y - textline_area.tl_corner.y + 1;

	if((textline_area.tl_corner.x + (tData->font->Width) * POINTS_TO_PIXEL * strlen(tData->textline)) > textline_area.br_corner.x)
		return -1;
	LCD_DrawFilledRect(	textline_area.tl_corner.x, textline_area.tl_corner.y,
						textline_area.tl_corner.x + width,
						textline_area.tl_corner.y + height,
						tData->border_color, tData->background_color);
	LCD_SetFont(tData->font);
	LCD_SetTextColor(tData->text_color);
	if(strlen(tData->textline) > 0)
		LCD_DisplayStringXY(textline_area.tl_corner.x + tData->font->Width,
							textline_area.tl_corner.y + tData->font->Height,
							tData->textline, TRANSPARENT);
	return 1;
}


