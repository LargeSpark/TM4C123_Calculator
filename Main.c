//Includes
#include "LCD_Driver.h"
#include "Keypad_Driver.h"
#include "Calculator.h"

// This is the first function called when the TM4C123 turns on.
int main(void){ 
	init(); //Initialise LCD
	padInit(); //Intialise Pad
	startCalc(); //Start Calculator code
}
