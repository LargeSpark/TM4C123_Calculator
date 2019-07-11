//Include the appropriate header file for this document
#include "Calculator.h"

//Define global variables
int tnaN = 0; 						//tempNumberArray index number. This is used to keep count of how big the array is without complex code.
int naN = 0;							//numberArray index number. This is used to keep count of how big the array is without complex code.
int opN = 0;							//operatorArray index number. This is used to keep count of how big the array is without complex code.
char mainArray[16];				//Where all the raw input is stored one button at a time.
int mainArrayN = 0;				//mainArray index number. This is used to keep count of how big the array is without complex code.
int tempNumberArray[16];	//This is where temporary numbers are stored while converting the main array into the operator array and number array.
int numberArray[16];			//This is where the numbers of the equation are stored.
char operatorArray[16];		//This is where the operator of the equation are stored.

/* This function starts the calculator code loop*/
void startCalc(void){
	mainArrayN = 0; //Set main array index to 0.
	while(1){
		checkButtons(); //Check buttons pushed.
	}
}
/* This function checks which buttons are pressed and carrys out the appropriate function. */
void checkButtons(void){
	char num; //Define button pressed variable as char.
	num = keyPressed(); //Call keyPressed and move to memory.
	/*
	 *If statements to check which button is pushed.
	 *Then carry out appriate function such as print, equals, or clear
	 *Then move to main array
	*/
	if(num == '1'){
		lcdPrintNumber(1);
		toArray(num);
		}
	if(num == '2'){
		lcdPrintNumber(2);
		toArray(num);
		}
		if(num == '3'){
		lcdPrintNumber(3);
			toArray(num);
		}
		if(num == '4'){
		lcdPrintNumber(4);
			toArray(num);
		}
		if(num == '5'){
		lcdPrintNumber(5);
			toArray(num);
		}
		if(num == '6'){
		lcdPrintNumber(6);
			toArray(num);
		}
		if(num == '7'){
		lcdPrintNumber(7);
			toArray(num);
		}
		if(num == '8'){
		lcdPrintNumber(8);
			toArray(num);
		}
		if(num == '9'){
		lcdPrintNumber(9);
			toArray(num);
		}
		if(num == '0'){
		lcdPrintNumber(0);
			toArray(num);
		}
		if(num == '/'){
			lcdPrintSymbol('/');
			toArray(num);
		}
		if(num == '*'){
			lcdPrintSymbol('*');
			toArray(num);
		}
		if(num == '-'){
			lcdPrintSymbol('-');
			toArray(num);
		}
		if(num == '+'){
			lcdPrintSymbol('+');
			toArray(num);
		}
		if(num == '='){
			equals();
		}
		if(num == 'c'){
			clearcalc();
		}
		while(keyPressed() == num){
		}
}
/* This function starts the appropriate code to show the result. */
void equals(void) {
  setArray(); //Convert main array into 2 seperate arrays. One for numbers, another for operators.
  calculation(); //Carry out bodmas calculation.
}
/* This function moves the pressed button into the main array */
void toArray(char button){
	mainArray[mainArrayN] = button; //MainArray current index is equal to the button pressed.
	mainArrayN++; //Increase index number by 1
}
/* This function moves all the operators and numbers out of the main char array into 2 arrays one made for operators and one made for numbers.*/
void setArray(void) {
  tnaN = 0; //Set tempNumberArray index to 0
  naN = 0;	//Set numberArray index to 0
  opN = 0;	//Set operator Array to 0
	//For loop to go through each number of the mainArray and check if its a digit or operator.
  for (int x = 0; x <= mainArrayN; x++) {
    if (isdigit(mainArray[x])) { //If the index of the array is a digit then store in digit in the tempNumberArray 
      tempNumberArray[tnaN] = mainArray[x] - '0'; //Store char number to int 
      tnaN++; //Increase temp number array index by 1
      //printf("array value: %c ,is number: %i, tnan: %i \n", mainArray[x],tempNumberArray[tnaN], tnaN);
    } else { //If the index of the array is a operator then store the operator in the operatorArray and convert the temp number array into the number array as a single number value.
      int numbr = 0; //define int of numbr which will be the number generated for the numberArray
      operatorArray[opN] = mainArray[x]; //Move operator into operator array from mainArray
      for (int y = 0; y < tnaN; y++) {
        numbr = numbr + tempNumberArray[y]; //Add current number to next number in array
				numbr = numbr * 10; //Multiply by 10 for the next number to consturct a whole number.
      }
      numberArray[naN] = numbr/10; //Divide by 10 once for loop end. This ensure number is not too big by a factor of 10
      //printf("number created: %i \n", numbr);
      naN++; 		//Increase numberArray index by 1
      opN++; 		//Increase operatorArray index by 1
      tnaN = 0; //Reset tempNumberArray index to 0
    }
    //final number
    if (x == mainArrayN - 1) { //If on final number then add to number array.
      //printf("final number start\n");
      int numbr = 0; //define int of numbr which will be the number generated for the numberArray
      for (int y = 0; y < tnaN; y++) {
        numbr = numbr + tempNumberArray[y]; //Add current number to next number in array
				numbr = numbr * 10; //Multiply by 10 for the next number to consturct a whole number.
      }
      //printf("nan: %i\n", naN);
      numberArray[naN] = numbr/10; //Divide by 10 once for loop end. This ensure number is not too big by a factor of 10
     // printf("NUMBR = %i\n", numbr);
      naN++; //Increase numberArray index by 1
      tnaN = 0; //Reset tempNumberArray index to 0
     // printf("array1: %i array2: %i\n", numberArray[0], numberArray[1]);
    }
  }

}

void calculation(void) {
  int z; //A value used for the for loops
  int divcount = 0; //How many divisions are in the equation
  int multicount = 0; //How many multiplications are in the equation
  int addcount = 0; //How many additions are in the equation
  int subcount = 0; //How many subtractions are in the equation
  //count the operator arrays values.
  for (int i = 0; i < opN; i++) { 
    if (operatorArray[i] == '+') { //If '+' found then
      addcount++; //increase addcount by 1
    }
    if (operatorArray[i] == '-') { //If '-' found then
      subcount++; //increase subcount by 1
    }
    if (operatorArray[i] == '/') { //If '/' found then
      divcount++; //increase divcount by 1
    }
    if (operatorArray[i] == '*') { //If '*' found then
      multicount++; //increase multicount by 1
    }
  }
  //printf("count as, + = %i , - = %i , / = %i, * = %i \n", addcount, subcount, divcount, multicount);
  //end count
	
	//If there are divisons then find and calculate divisions until there are none left.
	if (divcount != 0){
  while (divcount != 0) {
    for (z = 0; z < opN; z++) {
      if (operatorArray[z] == '/') {
        divi(z); 		//Calculate division
        divcount--; //Decrease divison count by 1
      }
    }
  }
}
  //If there are multiplication then find and calculate multiplications until there are none left.
  if (multicount != 0) {
    while (multicount != 0) {
      for (z = 0; z < opN; z++) {
        if (operatorArray[z] == '*') {
          mult(z);			//Calculate multiplication
          multicount--;	//Decrease multiplication count by 1
        }
      }
    }
  }
	//If there are additions then find and calculate additions until there are none left.
  if (addcount != 0) {
    //Addition
    while (addcount != 0) {
      for (z = 0; z < opN; z++) {
        if (operatorArray[z] == '+') {
          addi(z);		//Calculate additions
          addcount--;	//Decrease additions count by 1
        }
      }
    }
  }
  //If there are subtractions then find and calculate subtractions until there are none left.
  if (subcount != 0) {
    while (subcount != 0) {
      for (z = 0; z < opN; z++) {
        if (operatorArray[z] == '-') {
          subt(z);		//Calculate subtractions
          subcount--;	//Decrease subtractions count by 1
        }
      }
    }
  }
//  int result = numberArray[0];

  //printf("result: %i\n", result);
  printAnswer(); //Print result to an LCD screen.
}

/* This function carries out the division and then outputs the result into the numberarray and shifts down the numbers.*/
void divi(int z) {
  int newnum; //Define new number
  //printf("calc operator is /\n");
  //printf("number array[z] = %i , number array[z+1] = %i\n", numberArray[z],numberArray[z + 1]);
  newnum = numberArray[z] / numberArray[z + 1]; //New number equals the number array index / the number array index plus 1.
  //printf("+newnum = %i \n", newnum);
  numberArray[z] = newnum; //place result in array
  //shift back array by 1
  for (int divshift = z + 1; divshift <= naN; divshift++) {
    numberArray[divshift] = numberArray[divshift + 1];
    operatorArray[divshift] = operatorArray[divshift + 1];
  }
  opN--; //Reduce operator index by 1
}

/* This function carries out the multiplication and then outputs the result into the numberarray and shifts down the numbers.*/
void mult(int z) {
  int newnum; //Define new number
  //printf("calc operator is *\n");
  //printf("number array[z] = %i , number array[z+1] = %i\n", numberArray[z],numberArray[z + 1]);
  newnum = numberArray[z] * numberArray[z + 1]; //New number equals the number array index * the number array index plus 1.
 // printf("+newnum = %i \n", newnum);
  numberArray[z] = newnum; //place result in array
  //shift back array by 1
  for (int divshift = z + 1; divshift <= naN; divshift++) {
    numberArray[divshift] = numberArray[divshift + 1];
    operatorArray[divshift] = operatorArray[divshift + 1];
  }
  opN--; //Reduce operator index by 1
}

/* This function carries out the addition and then outputs the result into the numberarray and shifts down the numbers.*/
void addi(int z) {
  int newnum; //Define new number
  //printf("calc operator is +\n");
  //printf("number array[z] = %i , number array[z+1] = %i\n", numberArray[z], numberArray[z + 1]);
  newnum = numberArray[z] + numberArray[z + 1]; //New number equals the number array index + the number array index plus 1.
  //printf("+newnum = %i \n", newnum);
  numberArray[z] = newnum; //place result in array
  //shift back array by 1
  for (int divshift = z + 1; divshift <= naN; divshift++) {
    numberArray[divshift] = numberArray[divshift + 1];
    operatorArray[divshift] = operatorArray[divshift + 1];
  }
  opN--; //Reduce operator index by 1
}

/* This function carries out the subtraction and then outputs the result into the numberarray and shifts down the numbers.*/
void subt(int z) {
  int newnum; //Define new number
 // printf("calc operator is -\n");
 // printf("number array[z] = %i , number array[z+1] = %i\n", numberArray[z],  numberArray[z + 1]);
  newnum = numberArray[z] - numberArray[z + 1]; //New number equals the number array index - the number array index plus 1.
//  printf("+newnum = %i \n", newnum);
numberArray[z] = newnum; //place result in array
  //shift back array by 1
  for (int divshift = z + 1; divshift <= naN; divshift++) {
    numberArray[divshift] = numberArray[divshift + 1];
    operatorArray[divshift] = operatorArray[divshift + 1];
  }
  opN--; //Reduce operator index by 1
}

/* This function print the answer to the LCD screen*/
void printAnswer(void) {
	int revarray[16];		//Define reverse array
	int revarrayN = 0;	//Define reverse array index
	int negative = 0; //Define negative check. If 1 - Negative number, If 0 - Positive number
	
	
	newline(); //Move cursor to new line
	lcdPrintSymbol('='); //Print Equals symbol
 // printf("answer = %i \n", numberArray[0]);
	lcdPrintString(" "); //Print space
	
	
	//Check if number is negative
	if(numberArray[0] < 0){
		numberArray[0] = abs(numberArray[0]);
		negative = 1;
		lcdPrintSymbol('-');
	}
	
	
  int value1 = numberArray[0]; //define value to split
  while (value1 > 0) { //This loops returns the value in seperate digits in reverse and outputs it to an array
    int digit = value1 % 10; 
	//	lcdPrintNumber(digit);
		revarray[revarrayN] = digit; //Output digit to array
		revarrayN++; //Increase array index by 1
    //printf("%i \n", digit);
    value1 /= 10;
  }
	while (revarrayN != 0){ //This loop plays the array in reverse and prints the number to the lcd.
		lcdPrintNumber(revarray[revarrayN -1]); //Print number to lcd
		if(revarray[revarrayN -1] == 0){
		}
	revarrayN--; //Decrease reverse array index by 1
	}
	if(numberArray[0] == 0){ //if answer is zero then print zero.
		lcdPrintNumber(0);
	}
	while (keyPressed() != 'c'){ //If clear pressed exit loop
		
	}
	clearcalc(); //Clear calculator
}

void clearcalc(void){
	lcdClear(); //Clear lcd and return to home
	//Set default values
	mainArrayN = 0; 
	tnaN = 0;
	naN = 0;
	opN = 0;
	//Reset arrays to zero.
	for (int i=0; i <16; ++i){
	tempNumberArray[i] = 0;
	numberArray[i] = 0;
	operatorArray[i] = 0;
	mainArray[i] = 0;
	}
}
