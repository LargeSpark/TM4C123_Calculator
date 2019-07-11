//Define as header file
#ifndef Calculator_H
#define Calculator_H

//Includes
#include "Keypad_Driver.h"
#include "LCD_Driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//Functions
/** startCalc
*
* This function starts the calculator.
*/
void startCalc(void);
/** checkButtons
*
* This checks which buttons are pushed on the keypad and then carries out the appropriate function.
*/
void checkButtons(void);
/** toArray
*
* This function moves all button pressed into a char array for storage.
*/
void toArray(char button);
/** equals
*
* This function starts the code for when the equals button is pressed. First by calling toArray and then calling the equation.
*/
void equals(void);
/** divi
*
* This function divides all divisons in the equation from the arrays
*/
void divi(int z);
/** mult
*
* This function multiplys all multiplications in the equation from the arrays
*/
void mult(int z);
/** addi
*
* This function adds all additions in the equation from the arrays
*/
void addi(int z);
/** subt
*
* This function subtracts all subtractions in the equation from the arrays
*/
void subt(int z);
/** calculation
*
* This function runs each function in BODMAS order. The functions run are divi, mult, addi, subt.
*/
void calculation(void);
/** setArray
*
* This function moves all the operators and numbers out of the main char array into 2 arrays one made for operators and one made for numbers.
*/
void setArray(void);
/** printAnswer
*
* This function prints the answer to the LCD screen.
*/
void printAnswer(void);
/** clearcalc
*
* This function prints the answer to the LCD screen.
*/
void clearcalc(void);
#endif
