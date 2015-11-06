/*!	\file	LCD_Lib_Layers.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	Layered structure for LCD GUI abstractions implemented using a tree
*/
#ifndef LCD_LIB_LAYERS_H_
#define LCD_LIB_LAYERS_H_

#include "LCD_Lib_Draw.h"

/*! \fn		uint8_t contains(TOUCH_AREA *area, POINT *point)
    \brief  Checks whether a point is contained in a touch area
	\param  area	Area of the screen
	\param	point	Point to check
	\return 1 if point is contained in area, 0 otherwise
*/
uint8_t contains(TOUCH_AREA *area, POINT *point);

/*! \fn		void drawWidgets(WIDGET_DESC *desc)
    \brief  Draws the given widget and all the widgets in its sub-tree
	\param  desc	widget descriptor
*/
void drawWidgets(WIDGET_DESC *desc);

/*! \fn		void drawWidgetSiblings(WIDGET_DESC* widget)
    \brief  Draws the given widget and all the siblings widgets
	\param	widget	widget descriptor
*/
void drawWidgetSiblings(WIDGET_DESC* widget);

/*! \fn		void insertWidget(WIDGET *widget, WIDGET *father)
    \brief  Inserts a widget in the tree: if father is specified, widget becomes a child of father,
    		otherwise the father widget is searched with the findContainerWidget() method
    \param	widget		widget to insert in the layer tree
    \param	father		father widget
*/
void insertWidget(WIDGET *widget, WIDGET *father);

/*! \fn		void insertChild(WIDGET_DESC *father, WIDGET_DESC *newChild)
    \brief  Inserts the widget newChild as child of father
	\param	father		father widget
	\param	newChild	newChild widget
*/
void insertChild(WIDGET_DESC *father, WIDGET_DESC *newChild);

/*! \fn		WIDGET_DESC *findPressedWidget(POINT *detected)
    \brief  Finds the pressed widget from a point, i.e. finds the deepest widget in the widget layer tree that
    		has an handler
	\param	detected	detected point in the touchscreen
*/
WIDGET_DESC *findPressedWidget(POINT *detected);

/*! \fn		POINT getWidgetCenter(WIDGET *widget)
	\param	widget	widget object
	\return widget bounding box center
*/
POINT getWidgetCenter(WIDGET *widget);

/*! \fn		WIDGET_DESC *findContainerWidget(WIDGET *widget)
    \brief  Returns the 'father' widget of the given widget, i.e., a widget with a bounding box that entirely contains
    		the one of the given widget
*/
WIDGET_DESC *findContainerWidget(WIDGET *widget);

#endif /* LCD_LIB_LAYERS_H_ */
