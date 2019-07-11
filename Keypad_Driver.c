//Within this library the functions and code to utilise the keypad

//Include the appropriate header file for this document
#include "Keypad_Driver.h"

/*This is the matrix for the keypad.
This allows the columns and rows to be read as a char of the number of symbol they associate with.
Rows and columns start from zero
Example if column = 1 and row = 1 then the read off would be 5. */
char keypadMatrix[4][4] = {{'1','4','7','c'},
													{'2','5','8','0'},
													{'3','6','9','='},
													{'+','-','*','/'}};

/* This function initalises Port D (Output) and Port E(Input) which the pad operates on */
void padInit(void){
	//	Initialise Port D
	SYSCTL_RCGC2_R |= 0x0000008;     // D Clock Enable
  GPIO_PORTD_CR_R = 0xFF;          // Allow changes to Port D      
  GPIO_PORTD_AMSEL_R = ~0x00;      // Disable analogue function for Port D
  GPIO_PORTD_PCTL_R = 0x00000000;  // Regular GPIO set
  GPIO_PORTD_DIR_R = 0xFF;         // Set Port D as all outputs
  GPIO_PORTD_AFSEL_R = 0x00;       // Set that Port D has no alternate function       
  GPIO_PORTD_DEN_R = 0xFF;         // Enable all pins as digital on Port D

	//Initialise Port E
  SYSCTL_RCGC2_R |= 0x0000010;     // E Clock Enable
  GPIO_PORTE_CR_R = 0xFF;          // Allow changes to Port E    
  GPIO_PORTE_AMSEL_R = ~0x00;      // Disable analogue functions for Port E
  GPIO_PORTE_PCTL_R = 0x00000000;  // Regular GPIO set
  GPIO_PORTE_DIR_R = 0x00;         // Set Port E as all inputs
  GPIO_PORTE_AFSEL_R = 0x00;       // Set that Port E has no alternate function     
  GPIO_PORTE_DEN_R = 0x03F;        // Enable pins as digital for pins PE0-5
}

/* This function checks if a button has been pressed on the keypad and finds the value of that button by inputing the column and row into the keypadMatrix above */
char keyPressed(void){
	char key; //Define return value key as a char
	int column = 10; //Define column interger as value 10. Value 10 as default as this is far out of the scope of the 4x4 matrix.
	int row = 10; //Define row interger as value 10. Value 10 as default as this is far out of the scope of the 4x4 matrix.
	kPinOnOff(0); //Turn off output pins
	SysTick_Wait_ms(50); //Wait 50ms
	/*Start Scan
	This is made of a for loop with a nested for loop inside of it. 
	The first for loop sets the output of 1 pin at a time and waits 100ms.
	The second for loop then scans the outputs one at a time on port E to see if there is any outputs
	If there is an output then set intergers row and column to those values and set x = 4 and y = 5 to break out of loops*/
	for(int y = 1;y<5;y++){ //For loop 1
		kPinOnOff(y); //Set value of column between 1 and 4
		SysTick_Wait_ms(100); //Delay 100ms
		for(int x = 0; x<4; x++){ //For loop 2
			if(GPIO_PORTE_DATA_R&(0x01 << x) && x !=100){ //Read one Port E pins one at a time and if there is a value set values.
				column = y-1; //If there is an output set column as the output for y - 1. This is -1 due to the original for loop is made to work with the function kPinOnOff
				row = x; //If there is an output set row to the x value from the for loop
				x=100; // Break out of second loop
				y=100; // Break out of first loop
			}
		}
	}
	key = keypadMatrix[column][row]; //Find keypad value on the keypad matrix.
	kPinOnOff(0); //Turn off all outputs to ensure it does not interfere with LCD.
	return key; //Return the value of key pressed.
}
/* 
This function was created to turn on and off pins one at a time for the keypad.
It was created because the Pin PE0 did not want to turn off so it was changed to PA3 while the rest of the pins where still on PD1-3.
This function allows me to turn on one pin at a time.
When COL = 0 all pins are off.
When COL = 1 PA3 is on
When COL = 2 PD1 is on
When COL = 3 PD2 is on
When COL = 4 PD3 is on
*/
void kPinOnOff(int COL){
	PD = 0x00; //Port D all set off
	if(COL == 0){// All pins off
	COL1 = 0x00; // 0xFF - ON 
	COL2 = 0x00; // 0x02 - ON
	COL3 = 0x00; // 0x04 - ON 
  COL4 = 0x00; // 0x08 - ON
	}
	if(COL == 1){//Set ONLY PA3 on
	COL1 = 0xFF; // 0xFF - ON 
	COL2 = 0x00; // 0x02 - ON
	COL3 = 0x00; // 0x04 - ON 
  COL4 = 0x00; // 0x08 - ON
	}
	if(COL == 2){//Set ONLY PE1 on
	COL1 = 0x00; // 0xFF - ON 
	COL2 = 0x02; // 0x02 - ON
	COL3 = 0x00; // 0x04 - ON 
  COL4 = 0x00; // 0x08 - ON
	}
	if(COL == 3){//Set ONLY PE2 on
	COL1 = 0x00; // 0xFF - ON 
	COL2 = 0x00; // 0x02 - ON
	COL3 = 0x04; // 0x04 - ON 
  COL4 = 0x00; // 0x08 - ON
	}
	if(COL == 4){//Set ONLY PE3 on
	COL1 = 0x00; // 0xFF - ON 
	COL2 = 0x00; // 0x02 - ON
	COL3 = 0x00; // 0x04 - ON 
  COL4 = 0x08; // 0x08 - ON
	}
}
