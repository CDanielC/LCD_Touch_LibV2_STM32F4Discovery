/*!	\file	LCD_Lib_Conf.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	LCD_Touch_Lib configuration file
*/

#ifndef LCD_LIB_CONF_H_
#define LCD_LIB_CONF_H_

#include "STMPE811QTR.h"
#include "stm32f4_discovery_lcd.h"

/*! \def	NUM_PAGES
    \brief	Maximum number of LCD Pages
*/
#define	NUM_PAGES	3

/*! \def	NUM_BUTTONS
    \brief	Maximum number of LCD Buttons
*/
#define	NUM_BUTTONS		16 * NUM_PAGES

/*! \def	NUM_LABELS
    \brief	Maximum number of LCD Labels
*/
#define	NUM_LABELS		16 * NUM_PAGES

/*! \def	NUM_TEXTLINE
    \brief	Maximum number of LCD Text Lines
*/
#define	NUM_TEXTLINE	16 * NUM_PAGES

/*! \def	NUMWLIST
    \brief	Maximum number of widgets
*/
#define	NUMWLIST  		NUM_TEXTLINE + NUM_LABELS + NUM_BUTTONS

/*! \def	MAX_EVENTS_ENQUEUED
    \brief	Maximum number of touch events in the queue
*/
#define	MAX_EVENTS_ENQUEUED	10

/*! \def	MAX_LABEL_LEN
    \brief	Maximum length for a label content
*/
#define MAX_LABEL_LEN		20

#endif
