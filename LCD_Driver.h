//Define as header file
#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H
//Includes
#include "PLL.h"
#include <stdio.h>
#include <string.h>

//Pre-Processor Directives
//Defines
#define SYSCTL_RCGC2_R (*((volatile unsigned long*)0x400FE108))
//PORT A
#define GPIO_PORTA_DATA_R (*((volatile unsigned long*)0x400043FC))
#define GPIO_PORTA_DIR_R (*((volatile unsigned long*)0x40004400))
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long*)0x40004420))
#define GPIO_PORTA_PUR_R (*((volatile unsigned long*)0x40004510))
#define GPIO_PORTA_DEN_R (*((volatile unsigned long*)0x4000451C))
#define GPIO_PORTA_LOCK_R (*((volatile unsigned long*)0x40004520))
#define GPIO_PORTA_CR_R (*((volatile unsigned long*)0x40004524))
#define GPIO_PORTA_AMSEL_R (*((volatile unsigned long*)0x40004528))
#define GPIO_PORTA_PCTL_R (*((volatile unsigned long*)0x4000452C))
//PORT B
#define GPIO_PORTB_DATA_R (*((volatile unsigned long*)0x400053FC))
#define GPIO_PORTB_DIR_R (*((volatile unsigned long*)0x40005400))
#define GPIO_PORTB_AFSEL_R (*((volatile unsigned long*)0x40005420))
#define GPIO_PORTB_PUR_R (*((volatile unsigned long*)0x40005510))
#define GPIO_PORTB_DEN_R (*((volatile unsigned long*)0x4000551C))
#define GPIO_PORTB_LOCK_R (*((volatile unsigned long*)0x40005520))
#define GPIO_PORTB_CR_R (*((volatile unsigned long*)0x40005524))
#define GPIO_PORTB_AMSEL_R (*((volatile unsigned long*)0x40005528))
#define GPIO_PORTB_PCTL_R (*((volatile unsigned long*)0x4000552C))

//Pin Defines
#define DB4 (*((volatile unsigned long *)0x40005040)) //PB4
#define DB5 (*((volatile unsigned long *)0x40005080)) //PB5
#define DB6 (*((volatile unsigned long *)0x40005100)) //PB6
#define DB7 (*((volatile unsigned long *)0x40005200)) //PB7
#define PB  (*((volatile unsigned long *)0x400053C0)) //Port
#define lcdRS (*((volatile unsigned long *)0x40004020))//PA3 // RS
#define lcdEN (*((volatile unsigned long *)0x40004010))//PA2 // EN

//Functions
/** init
*
* This function initialises the ports for the lcd and sends initalisation data to the LCD. 
*/
void init(void);
/** ENPulse
*
* This function pulses the EN port on the LCD screen to enable the lcd to recieve a nibble of data.
*/
void ENPulse(void); 
/** lcdPrintNumber
*
* This function prints a number on the LCD screen
* number - This is the number you wish to print on the lcd screen.
*/
void lcdPrintNumber(int number);
/** lcdPrintSymbol
*
* This function prints a symbol on the lcd screen
* symbol - This is the symbol you wish to print on the lcd screen.
*/
void lcdPrintSymbol(char symbol);
/** lcdPrintLetter
*
* This function prints a letter on the lcd screen
* letter - This is the letter you wish to print on the lcd screen.
*/
void lcdPrintLetter(char letter);
/** lcdPrintString
*
* This function prints a string on the lcd screen
* str - This is the string of characters you wish to print on the lcd screen.
*/
void lcdPrintString(char *str);
/** lcdClear
*
* This function clears the lcd screen and set cursor to home.
*/
void lcdClear(void);
/** lcdGoTo
*
* This function moves the cursor to the co-ordinates set.
* x - This is the x coordinate where the cursor will move to.
* y - This is the y coordinate where the cursor will move to.
*/
void lcdGoTo(int y, int x);
/** newline
*
* This function moves the cursor onto a new line. This is an alternative to the lcdGoTo.
*/
void newline(void);
/** lcdWriteCommand
*
* This function sends a command to the lcd screen
*/
void lcdWriteCommand(int command);
#endif
