//Within this library the functions and code to utilise the LCD

//Include the appropriate header file for this document
#include "LCD_Driver.h"
#include <ctype.h>

int charcount; //Characters used

/*
This function firstly initialises the ports which the LCD operates on (Ports A & B).
It then calls the System Tick Initialisation function (Systick Init()), 
this as the where the delay function (SysTick_wait_ms) or (SysTick_wait_us).
In order to initialise the screen, a specific set of instructions must be followed (found on the datasheet PG 46),
In basic terms the
- Power on LCD
- Wait >15ms
- DB5 & DB4 Pulsed HIGH
- Wait > 4.1ms
- DB5 & DB4 Pulsed HIGH
- Wait > 100us
- DB5 & DB4 Pulsed HIGH
*After this is complete the relevant data pins are set to transmit specific Data such as how many lines*
RS R/W DB7 DB6 DB5 DB4
0   0   0   0   1   0
0   0   0   0   1   0
0   0   N   F   *   *  - N = Number of lines (1 = 2 lines 0 = 1 line)  - F = Font  - * can either be 0 or 1 it makes no difference
0   0   0   0   0   0 
0   0   1   0   0   0 
0   0   0   0   0   0
0   0   0   0   0   1
0   0   0   0   0   0

- End of initialisation
*/
void init(void){
	//Initialise Ports
	SYSCTL_RCGC2_R = 0x00000001 + 0x00000002; //Enable Port A, B Clock.
	//Initialise Port A
	GPIO_PORTA_AMSEL_R &= 0X00;	// Disable analogue function for Port A
	GPIO_PORTA_PCTL_R &= 0X00;	// Regular GPIO Set for Port A
	GPIO_PORTA_DIR_R |= 0XFF;		// Set Port A as all outputs
	GPIO_PORTA_AFSEL_R &= 0x00;	// Set that Port A has no alternate function
	GPIO_PORTA_DEN_R |= 0xFF;		// Enable all pins as digital on Port A
	GPIO_PORTA_PUR_R &= 0x00;		// Disable pull up resistors on Port A
	//Initialise Port B
	GPIO_PORTB_AMSEL_R &= 0X00;	// Disable analogue function for Port B
	GPIO_PORTB_PCTL_R &= 0X00;	// Regular GPIO Set for Port B
	GPIO_PORTB_DIR_R |= 0XFF;		// Set Port B as all outputs
	GPIO_PORTB_AFSEL_R &= 0x00;	// Set that Port B has no alternate function
	GPIO_PORTB_DEN_R |= 0xFF;		// Enable all pins as digital on Port B
	GPIO_PORTB_PUR_R &= 0x00;		//Disable pull up resistors on Port B
	
	//Initialise SysTick function
	SysTick_Init();
	
	//set PLL Frequency
	PLL_Init(50); 
	
	//Set lcdRS off for instruction.
	lcdRS = 0x00;
	lcdWriteCommand(0x0E);
	
	//This is the initialisation sequency - described in detail above
	lcdWriteCommand(0x0C);//sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble 
	SysTick_Wait_ms(6); //Wait 6ms
	lcdWriteCommand(0x00); //sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	SysTick_Wait_ms(6); //Wait 6ms
	lcdWriteCommand(0x02);//sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	lcdWriteCommand(0x10); //sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	lcdWriteCommand(0x0F); //sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	lcdWriteCommand(0x01); //sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	lcdWriteCommand(0x06); //sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	lcdClear(); //Clears the LCD
	SysTick_Wait_ms(100); //wait 100ms
	
	
	//TEST LCD SCREEN
	lcdPrintString("Welcome to"); //Print string on line 1
	newline(); //Turn to second line
	lcdPrintString("Heaven"); //Print string on line 2
	SysTick_Wait_ms(6000); // wait 6 seconds
	lcdClear(); //Clear LCD screen
}

//Send LCD command
void lcdWriteCommand(int command){
	lcdRS = 0x00; //Sets the register select pin of the LCD to zero - enabling the sending of instructions
	PB = command << 4; //Move command up 4 bits
	ENPulse(); //The pulse function to set pins high then low in quick succession
}

/*
This function is used to Clear the LCD screen
It does this by setting all relevant pins to 0
Waiting 
and then setting them to zero again
Waiting again
and then a final Set to zero
After this is complete, the screen is cleared of all data
The curser moves to home (far left automatically)
*/
void lcdClear(void){
	lcdWriteCommand(0x00); 	//sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	SysTick_Wait_us(500); 	//wait 500us
	lcdWriteCommand(0x00);	//sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	SysTick_Wait_us(500); 	//wait 500us
	lcdWriteCommand(0x00); 	//sets the pins of Port B to desired value with the Hex value, and shifting the bits up to the next nibble
	SysTick_Wait_us(500); 	//wait 500us
	charcount = 0;
}


/*
A function used to Set pins high, and then low within quick succession.
This is used throughout the coding of the LCD,
as the same data must be sent several times in series after a specified wait time,
this is how the LCD recieves instructions.
*/
void ENPulse(void){
	lcdEN = 0x04; //EN Pin High
	SysTick_Wait_ms(1); //Wait 1 ms
	lcdEN = 0x00; //EN Pin Low
	SysTick_Wait_ms(1); //Wait 1 ms
}


/*
A function to move the curser to a desired position utilising an X and Y co-ordinate system

With hex1 as the upper line (upper nibble) (Y co-ordinate)
and hex2 as the lower line (lower nibble)  (X co-ordinate)

As there are only two lines, the Y co-ordinate only has two possible values
*/
void lcdGoTo(int y, int x){
	int hex1; //Upper Nibble 
	int hex2; //Lower Nibble
	
	if(y == 0){ 
		hex1 = 0;
		hex2 = x;
	}
	if(y == 1){
		hex1 = 0x04;
		hex2 = x;
	}
	lcdRS = 0x00;
	PB = (hex1) << 4;
	ENPulse(); //The pulse function to set pins high then low in quick succession
	PB = (hex2) << 4;
	ENPulse(); //The pulse function to set pins high then low in quick succession
	SysTick_Wait_ms(4);
	PB = 0x00;
	ENPulse(); //The pulse function to set pins high then low in quick succession
}


/*
A function encapsulating the case machine which prints NUMBER characters to the screen
The hex values are obtained from the table on the LCD data sheet (PG 17 & 18)
This involves a unique co-ordinate system with each column and row ranging from 0000 - 1111 (or 0x00 - 0x0F)
Again data is pulsed into the LCD screen utilising the ENPulse function
*/
void lcdPrintNumber(int number){
	charcount++;
		lcdRS = 0x08;
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		switch (number){
		
			case 0: // case 0 = number 0 select
				PB = (0x00) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 1: // case 1 = number 1 select
				PB = (0x01) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 2: // case 2 = number 2 select
				PB = (0x02) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 3: // case 3 = number 3 select
				PB = (0x03) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 4: // case 4 = number 4 select
				PB = (0x04) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 5: // case 5 = number 5 select
				PB = (0x05) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 6: // case 6 = number 6 select
				PB = (0x06) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 7: // case 7 = number 7 select
				PB = (0x07) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 8: // case 8 = number 8 select
				PB = (0x08) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
			
			case 9: // case 9 = number 9 select
				PB = (0x09) << 4;
				ENPulse(); //The pulse function to set pins high then low in quick succession
			break;
		}
}

/*
A function encapsulating the If statements which prints SYMBOL characters to the screen
The hex values are obtained from the table on the LCD data sheet (PG 17 & 18)
This involves a unique co-ordinate system with each column and row ranging from 0000 - 1111 (or 0x00 - 0x0F)
Again data is pulsed into the LCD screen utilising the ENPulse function
*/

void lcdPrintSymbol(char symbol){
		lcdRS = 0x08;
	if(symbol == '+'){ //selects the mathematical symbol for plus '+'
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0B) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(symbol == '-'){ //selects the mathematical symbol for minus '-'
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0D) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(symbol == '/'){ //selects the mathematical symbol for divide '/'
		PB = (0x0F) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0D) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(symbol == '*'){ //selects the mathematical symbol for multiple 'x or *'
		PB = (0x07) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x08) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(symbol == '='){ //selects the mathematical symbol for equals '='
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0D) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
}
/*
A function encapsulating the If statements which prints LETTER characters to the screen
The hex values are obtained from the table on the LCD data sheet (PG 17 & 18)
This involves a unique co-ordinate system with each column and row ranging from 0000 - 1111 (or 0x00 - 0x0F)
Again data is pulsed into the LCD screen utilising the ENPulse function
*/
void lcdPrintLetter(char letter){
	charcount++;
	lcdRS = 0x08;
	if(letter == 'A'){ //Selects Letter capital A
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'B'){ //Selects Letter capital  B
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'C'){ //Selects Letter capital C
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	} 
	if(letter == 'D'){ //Selects Letter capital D
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'E'){ //Selects Letter capital E
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'F'){ //Selects Letter capital F
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x06) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'G'){ //Selects Letter capital G
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x07) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'H'){ //Selects Letter capital H
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x08) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'I'){ //Selects Letter capital I
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x09) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'J'){ //Selects Letter capital J
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0A) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'K'){ //Selects Letter capital K
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0B) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'L'){ //Selects Letter capital L
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0C) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'M'){ //Selects Letter capital M
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0D) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'N'){ //Selects Letter capital N
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0E) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'O'){ //Selects Letter capital O
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0F) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'P'){ //Selects Letter capital P
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x00) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'Q'){ //Selects Letter capital Q
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'R'){ //Selects Letter capital R
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'S'){ //Selects Letter capital S
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'T'){ //Selects Letter capital T
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'U'){ //Selects Letter capital U
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'V'){ //Selects Letter capital V
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x06) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'W'){ //Selects Letter capital W
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x07) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'X'){ //Selects Letter capital X
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x08) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'Y'){ //Selects Letter capital Y
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x09) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'Z'){ //Selects Letter capital Z
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0A) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'a'){ //Selects Letter lowercase a
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'b'){ //Selects Letter lowercase b
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'c'){ //Selects Letter lowercase c
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'd'){ //Selects Letter lowercase d
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'e'){ //Selects Letter lowercase e
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'f'){ //Selects Letter lowercase f
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x06) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'g'){ //Selects Letter lowercase g
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x07) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'h'){ //Selects Letter lowercase h
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x08) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'i'){ //Selects Letter lowercase i
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x09) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'j'){ //Selects Letter lowercase j
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0A) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'k'){ //Selects Letter lowercase k
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0B) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'l'){ //Selects Letter lowercase l
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0C) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'm'){ //Selects Letter lowercase m
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0D) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'n'){ //Selects Letter lowercase n
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0E) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'o'){ //Selects Letter lowercase o
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0F) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'p'){ //Selects Letter lowercase p 
		PB = (0x05) << 4; 
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x00) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'q'){ //Selects Letter lowercase q
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'r'){ //Selects Letter lowercase r
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x02) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 's'){ //Selects Letter lowercase s
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x03) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 't'){ //Selects Letter lowercase t
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x04) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'u'){ //Selects Letter lowercase u
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'v'){ //Selects Letter lowercase v
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x06) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'w'){ //Selects Letter lowercase w
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x07) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'x'){ //Selects Letter lowercase x
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x08) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'y'){ //Selects Letter lowercase y
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x09) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == 'z'){ //Selects Letter lowercase z
		PB = (0x05) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x0A) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == ' '){ //Selects Letter lowercase 'space'
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
		PB = (0x01) << 4;
		ENPulse(); //The pulse function to set pins high then low in quick succession
	}
	if(letter == '0'){ //Selects Letter 0 for the print string function
		lcdPrintNumber(0);
	}
	if(letter == '1'){ //Selects Letter 1 for the print string function
		lcdPrintNumber(1);
	}
	if(letter == '2'){ //Selects Letter 2 for the print string function
		lcdPrintNumber(2);
	}
	if(letter == '3'){ //Selects Letter 3 for the print string function
		lcdPrintNumber(3);
	}
	if(letter == '4'){ //Selects Letter 4 for the print string function
		lcdPrintNumber(4);
	}
	if(letter == '5'){ //Selects Letter 5 for the print string function
		lcdPrintNumber(5);
	}
	if(letter == '6'){ //Selects Letter 6 for the print string function
		lcdPrintNumber(6);
	}
	if(letter == '7'){ //Selects Letter 7 for the print string function
		lcdPrintNumber(7);
	}
	if(letter == '8'){ //Selects Letter 8 for the print string function
		lcdPrintNumber(8);
	}
	if(letter == '9'){ //Selects Letter 9 for the print string function
	lcdPrintNumber(9);
	}
}
/*
This function allows for the printing of strings of characters to the screen
str is this string
Utilising a for loop which cycles through each bit and then combines them via the case machines above for 
PrintNumber/Symbol/Letter
*/
void lcdPrintString(char *str){
	int x;
	int stringLength = strlen(str);
	
	//for loop cycling through each desired symbol and concatenating them
	for(x = 0; x < stringLength; x++){
	lcdPrintLetter(str[x]);
}
}


/*
This function moves the curser to the next line, alternating between line 1 and 2
These are predefined values which therefore avoid the additional wait times within the goTo function, 
making the calculator more effecient.
*/
void newline(void){
	int moveby = 41 - charcount;
	for(int x; x < moveby; x++){
		lcdPrintLetter(' ');
	}
	charcount = 0;
}

