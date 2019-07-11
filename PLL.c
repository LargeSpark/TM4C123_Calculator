#include "PLL.h"
	int n;
	int freq;
	double TPC;
	double ms_cycled;
	double us_cycled;
	int ms_cycle;
	int us_cycle;

void SysTick_Init(void){

  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock

}


	void PLL_Init(unsigned long MHz){
	//Calculating variable

	n = (400/MHz) - 1; //calculate clock divider
	TPC = 1.0/(MHz*100000.0); //calculate time per clock cycle
	//calculate cycles for ms
	ms_cycled = 0.001/TPC;
	ms_cycle = ms_cycled;
	//calculate cycles for us
	us_cycled = 0.000001/TPC;
	us_cycle = us_cycled;
	// Using the above formula allows us to programatically set the clock speed
	// by setting a variable for the divider (n). Because the 7 bit SYSDIV2 is n then it will divide by
	// n + 1 
	
	
  // 0) Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USERCC2
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;
  // 4) set the desired system divider
  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
  
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (n<<22);      // configure for 50 MHz clock 
	//*** YOU CAN CHANGE The 50 MHz Frequency above, by changing the Dividor "7" to 
	//*** any desired frequency, i.e (4<<22) makes it 80 MHz etc. ..
	
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
		
}
	
	
	void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
		
}
		void SysTick_Wait_ms(unsigned long delay_ms){
  NVIC_ST_RELOAD_R = delay_ms * ms_cycle;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
	void SysTick_Wait_us(unsigned long delay_us){
  NVIC_ST_RELOAD_R = delay_us * us_cycle;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }	
}
	int number(void){
		return us_cycle;
	}

