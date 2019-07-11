//Define as header file
#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

//Includes
#include "PLL.h"

//Pre-Processor Directives
//Defines
#define SYSCTL_RCGC2_R (*((volatile unsigned long*)0x400FE108))
//PORT A Pre-Processor Directives
#define GPIO_PORTA_DATA_R (*((volatile unsigned long*)0x400043FC))
#define GPIO_PORTA_DIR_R (*((volatile unsigned long*)0x40004400))
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long*)0x40004420))
#define GPIO_PORTA_PUR_R (*((volatile unsigned long*)0x40004510))
#define GPIO_PORTA_DEN_R (*((volatile unsigned long*)0x4000451C))
#define GPIO_PORTA_LOCK_R (*((volatile unsigned long*)0x40004520))
#define GPIO_PORTA_CR_R (*((volatile unsigned long*)0x40004524))
#define GPIO_PORTA_AMSEL_R (*((volatile unsigned long*)0x40004528))
#define GPIO_PORTA_PCTL_R (*((volatile unsigned long*)0x4000452C))
//Port D Pre-Processor Directives
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
//Port E Pre-Processor Directives
#define GPIO_PORTE_DATA_R (*((volatile unsigned long*)0x400243FC))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long*)0x40024400))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long*)0x40024420))
#define GPIO_PORTE_PUR_R (*((volatile unsigned long*)0x40024510))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long*)0x4002451C))
#define GPIO_PORTE_LOCK_R (*((volatile unsigned long*)0x40024520))
#define GPIO_PORTE_CR_R (*((volatile unsigned long*)0x40024524))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long*)0x40024528))
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long*)0x4002452C))
#define GPIO_PORTE_IS_R (*((volatile unsigned long *)0x40024404))
//Define port D
#define PD  (*((volatile unsigned long *)0x400073C0))


//Keypad Pin Defines
//Column Outputs
#define COL1 (*((volatile unsigned long *)0x40004040)) //PA4 
#define COL2 (*((volatile unsigned long *)0x40007008)) //PD1
#define COL3 (*((volatile unsigned long *)0x40007010)) //PD2
#define COL4 (*((volatile unsigned long *)0x40007020)) //PD3
//Row Inputs
#define ROW1 (*((volatile unsigned long *)0x40024004))//PE0
#define ROW2 (*((volatile unsigned long *)0x40024008)) //PE1
#define ROW3 (*((volatile unsigned long *)0x40024010)) //PE2
#define ROW4 (*((volatile unsigned long *)0x40024020)) //PE3
	
//FUNCTIONS
/** padInit
*
* This function initialises the ports for the keypad.
*/
void padInit(void);
/** keyPressed
*
* This function turns a char of which key is pressed on the pad.
*/
char keyPressed(void);
/** kPinOnOff
*
* This function turns on a single column of the keypad.
* Col - Column pin to turn on
*/
void kPinOnOff(int COL);
#endif
