/*
 * LCD_Lib_Layers.c
 *
 *  Created on: 17/ott/2015
 *      Author: Daniel
 */
#include "LCD_Lib_Types.h"
#include "LCD_Lib_Layers.h"

extern WIDGET_DESC 	*LCD_pages[NUM_PAGES];
extern WIDGET_DESC 	widgetList[NUMWLIST];
extern uint8_t activePage;

void drawWidgets(WIDGET_DESC *desc){
	if(desc){
		drawSingleWidget(desc);

		while(desc->child){

			desc = desc->child;
			drawWidgetSiblings(desc);
		}
	}
}

void drawWidgetSiblings(WIDGET_DESC* widget){
	WIDGET_DESC *desc;
	desc = widget;
	do{
		drawSingleWidget(desc);
		drawWidgets(desc->child);
		desc = desc->sibling;
	}while(desc);
}



void insertChild(WIDGET_DESC *father, WIDGET_DESC *newChild){
	WIDGET_DESC *temp;
	if(father->child == 0){
		father->child = newChild;
	}
	else{
		temp = father->child;
		while(temp->sibling != 0)
			temp = temp->sibling;
		temp->sibling = newChild;
	}
}

WIDGET_DESC *findContainerWidgetFirstChild(WIDGET_DESC *desc, WIDGET_DESC **father, POINT *detected){

	while (desc->child != 0){

		*father = desc;

		desc = desc->child;

		if(!contains(&desc->data->bounds, detected))
			return desc;

	}
	return desc;
}

WIDGET_DESC *findContainerWidgetSibling(WIDGET_DESC *desc, POINT *detected){
	WIDGET_DESC *father;
	while(desc->sibling != 0){
		if(contains(&desc->sibling->data->bounds, detected)){
			desc = desc->sibling;
			return desc;
		}
		desc = desc->sibling;
	}
	return 0;
}

WIDGET_DESC *findPressedWidgetFirstChild(WIDGET_DESC *desc, WIDGET_DESC **father, POINT *detected){

	while (desc->child != 0){

		if(desc->data->handler != 0)
			*father = desc;

		desc = desc->child;

		if(!contains(&desc->data->bounds, detected))
			return desc;

	}
	return desc;
}

WIDGET_DESC *findPressedWidgetSibling(WIDGET_DESC *desc, POINT *detected){
	WIDGET_DESC *father;
	while(desc->sibling != 0){
		if(contains(&desc->sibling->data->bounds, detected) && desc->sibling->data->handler != 0){
			desc = desc->sibling;
			return desc;
		}
		desc = desc->sibling;
	}
	return 0;
}

WIDGET_DESC *findPressedWidget(POINT *detected){
	WIDGET_DESC *desc, *descChild, *descSib, *father;
	desc = LCD_pages[activePage];
	father = 0;

	if(desc == 0) //does not exist any page
		return 0;

	do{

		//finds the right 'father' node
		descChild = findPressedWidgetFirstChild(desc, &father, detected);

		//the first child of 'father' is the right node, otherwise search among the siblings
		if(descChild->data->handler != 0 && contains(&descChild->data->bounds, detected))
			return descChild;

		//finds the right sibling node
		descSib = findPressedWidgetSibling(descChild, detected);

		if(descSib == 0)
			return father;

		desc = descSib;

	}while(desc != 0);

	return 0;
}

POINT getWidgetCenter(WIDGET *widget){
	if(widget->wtype == LABEL_TYPE)
		return widget->bounds.tl_corner;
	else{
		POINT center;
		center.x = (widget->bounds.tl_corner.x + widget->bounds.br_corner.x) / 2;
		center.y = (widget->bounds.tl_corner.y + widget->bounds.br_corner.y) / 2;
		return center;
	}
}

WIDGET_DESC *findContainerWidget(WIDGET *widget){

	POINT widgetPoint = getWidgetCenter(widget);

	WIDGET_DESC *desc, *descChild, *descSib, *father;
	desc = LCD_pages[activePage];
	father = desc; //because if not childs have been found, the page object is the father

	if(desc == 0) //does not exist any page
		return 0;

	do{

		//finds the right 'father' node
		descChild = findContainerWidgetFirstChild(desc, &father, &widgetPoint);

		//the first child of 'father' is the right node, otherwise search among the siblings
		if(descChild->data->handler != 0 && contains(&descChild->data->bounds, &widgetPoint))
			return descChild;

		//finds the right sibling node
		descSib = findContainerWidgetSibling(descChild, &widgetPoint);

		if(descSib == 0)
			return father;

		desc = descSib;

	}while(desc != 0);

	return 0;
}

void insertWidget(WIDGET *widget, WIDGET *father){
	WIDGET_DESC *father_desc;
	if(father)
		insertChild(&widgetList[father->id], &widgetList[widget->id]);
	else if((father_desc = findContainerWidget(widget)))
		insertChild(father_desc, &widgetList[widget->id]);
}

uint8_t contains(TOUCH_AREA *area, POINT *point){
	if	((point->x >= area->tl_corner.x) &&
		 (point->x <= area->br_corner.x) &&
		 (point->y >= area->tl_corner.y) &&
		 (point->y <= area->br_corner.y))
		return 1;
	else
		return 0;
}

