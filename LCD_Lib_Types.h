/*!	\file	LCD_Lib_Types.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	Types for LCD_Touch_Lib
*/

#ifndef LCD_LIB_TYPES_H_
#define LCD_LIB_TYPES_H_

#include "LCD_Lib_HWParam.h"
#include "LCD_Lib_Conf.h"

/*! \def	POINTS_TO_PIXEL
    \brief	Converts points to pixels (for fonts)
*/
#define POINTS_TO_PIXEL	1.3

typedef void (*HANDLER)(void *arg); /*!< Object handler type */

/*! 	\struct
		\brief 	Represents a point
 */
typedef struct point{
    uint16_t x; 	/*!< X component */
    uint16_t y; 	/*!< Y component */
} POINT;

/*! 	\struct
		\brief 	Represents a point
 */
typedef struct _touch_event{
    uint16_t x; 	/*!< X component */
    uint16_t y; 	/*!< Y component */
} TOUCH_EVENT;


/*! 	\struct
		\brief 	Represents a rectangular area in the screen, through its top right / bottom left corner coordinates
 */
typedef struct touch_area {
	POINT tl_corner; 	/*!< Top left corner coordinates of the button area */
	POINT br_corner; 	/*!< Bottom right corner coordinates of the button area */
} TOUCH_AREA;

/*! 	\struct
		\brief 	Circular array for touch events
 */
typedef struct _event_queue{
	uint8_t widgetID[MAX_EVENTS_ENQUEUED];
	uint8_t counter;
	uint8_t read_index;
	uint8_t write_index;
} EVENT_QUEUE_T;

/*! 	\struct
		\brief 	Widget types
 */
typedef enum _widgetType{
	LCD_PAGE_TYPE,
	BUTTON_TYPE,
	LABEL_TYPE,
	TEXTLINE_TYPE,
	NUM_WIDGET_TYPE
} WIDGET_TYPE;

/*! 	\union
		\brief 	Represents the type that a background can assume
 */
typedef union _background
{
	uint16_t color;
	uint8_t* image;
} BACKGROUND;

/*! 	\struct
		\brief 	Represents the background types
 */
typedef enum _backgroundType{
	BACKGROUND_COLOR,
	BACKGROUND_IMAGE,
	NUM_BACKGROUND_TYPES
}BACKGROUND_TYPE;

/*! 	\struct
		\brief 	Page background type
 */
typedef struct lcd_page_background {
	BACKGROUND 		background;
	BACKGROUND_TYPE	background_type;
} PAGE_BACKGROUND;

/*! 	\struct
		\brief 	Type for a basic widget, the features of each particular type can be linked with the field wdata
 */
typedef struct _widget {
	WIDGET_TYPE wtype;			/*!< Widget Type */
	TOUCH_AREA 	bounds;			/*!< Widget bounds (top left and bottom right corner points) */
	uint8_t 	id;				/*!< Global widget ID */
	void 		*wdata;			/*!< Specific Widget Data */
	HANDLER 	handler; 		/*!< Handler function of the widget, if any */
	void		*handler_arg;	/*!< Handler function argument, if any */
} WIDGET;

/*! 	\struct
		\brief 	Widget descriptor type, used to maintain the layered structure of each page
 */
typedef struct _wdesc {
	WIDGET 			*data;
	struct _wdesc 	*child;
	struct _wdesc 	*sibling;
} WIDGET_DESC;

/*! 	\struct
		\brief 	Widget Data for LCD_PAGE_TYPE
 */
typedef struct lcd_page {
	PAGE_BACKGROUND	backgroundObj;
	BACKGROUND_TYPE	background_type;
} LCD_PAGE;

/*! 	\struct
		\brief 	Specific Button widget Data
 */
typedef struct touch_button {
	uint8_t* icon; 			/*!< Null if the icon is included in the background */
} BUTTON;

/*! 	\struct
		\brief 	Specific TextLine widget Data
 */
typedef struct _textline{
	uint16_t background_color;
	uint16_t border_color;
	uint16_t text_color;
	uint16_t textline_len;
	sFONT	 *font;
	char	 *textline;
} TEXT_LINE;

/*! 	\struct
		\brief 	Specific Label widget Data
 */
typedef struct _label{
	uint16_t text_color;
	sFONT	 *font;
	char	 text[MAX_LABEL_LEN];
} LABEL;

#endif
