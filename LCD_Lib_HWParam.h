/*!	\file	LCD_Lib_HWParam.h
 	\author Daniel Casini
 	\date	17/10/2015
	\brief	HW parameters for the LCD_Touch_Lib
*/

#ifndef LCD_LIB_HWPARAM_H_
#define LCD_LIB_HWPARAM_H_

/*! \def	TOUCH_AD_VALUE_MAX
    \brief	Analog/Digital converter max value
*/
#define 	TOUCH_AD_VALUE_MAX    	(4000)

/*! \def	TOUCH_AD_VALUE_MIN
    \brief	Analog/Digital converter min value
*/
#define 	TOUCH_AD_VALUE_MIN    	(100)

/*! \def	LCD_HEIGHT
    \brief	Screen height
*/
#define		LCD_HEIGHT 				LCD_PIXEL_HEIGHT

/*! \def	LCD_WIDTH
    \brief	Screen width
*/
#define 	LCD_WIDTH 				LCD_PIXEL_WIDTH

/*! \def	X_SCALE
    \brief	Parameter for the conversion between the AD value to the LCD value (result of previous calibration)
*/
#define 	X_SCALE					-0.092348285

/*! \def	Y_SCALE
    \brief	Parameter for the conversion between the AD value to the LCD value (result of previous calibration)
*/
#define		Y_SCALE					0.0662910193

/*! \def	X_OFFSET
    \brief	Parameter for the conversion between the AD value to the LCD value (result of previous calibration)
*/
#define		X_OFFSET				-353

/*! \def	Y_OFFSET
    \brief	Parameter for the conversion between the AD value to the LCD value (result of previous calibration)
*/

#define		Y_OFFSET				24

/*! \def	ERR
    \brief	Error constant
*/
#define 	ERR						255

#endif
