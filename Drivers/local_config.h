/**
* \file
* \version 1.0
* \author bazhen.paseka
* \date 2205
* \mail bazhen.info(at)gmail.com
*************************************************************************************
* \copyright	Bazhen Paseka
* \copyright	Brovary, Kyiv region
* \copyright	Ukraine
*
*
*************************************************************************************
*
* \brief
*
*/

#ifndef TM1638_LED8KEYS_LOCAL_CONFIG_H_
#define TM1638_LED8KEYS_LOCAL_CONFIG_H_

	/*
	 * 		TM1638 LED & KEYS:
	 * 	VCC		-> 3v3	power
	 * 	GND		-> GND	power
	 * 	STB		-> PB9	Output
	 * 	CLK		-> PB8	Output
	 * 	DIO		-> PB7	Output + Input
	 *
	 * 		stm32f103c8t6:
	 * 	SWDIO	-> PA13
	 * 	SWCLK	-> PA14
	 * 	debug 	-> PA2, 9600
	 * 	LED		-> PB2
	 * 	BTN		-> PA0
	 * 	8MHz	-> PD0, PD1
	 */

/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/

/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/
	#define 	SOFT_VERSION	200
	//#define 	DEBUG_LVL 1
	//#define 	DEBUG_LVL 2
	#define 	DEBUG_LVL 3
	#define		UART_DEBUG		huart2
/*
**************************************************************************
*								   DATA TYPES
**************************************************************************
*/

/*
**************************************************************************
*									 MACRO'S
**************************************************************************
*/
#endif /* TM1638_LED8KEYS_LOCAL_CONFIG_H_ */
