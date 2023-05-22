#include  "../header/halGPIO.h"     // private library - HAL layer

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
const char F[] = "Write your idiom";
const char Line[] = "max 32, have: ";
const char Finished[] = "Finished writing 32";
const char Finished2[] = "Moving to sleep mode  ";
unsigned char keypressed = 0x00;
char KB;
int Curser = 0;
int LineCount,PrevCount;
unsigned long int REdge;
void sysConfig(void){ 
	GPIOconfig();
//	TIMERconfig();
        lcd_init();
	DMAConfig();
}
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
//--------------------------------------------------------------------
//              Clear 8-bit LEDs array
//--------------------------------------------------------------------
void clrLEDs(void){
    LEDsArrPort = 0x00;
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  PBsArrIntPend &= 0x00;
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}

//*********************************************************************
//            Port2 Interrupt Service Rotine
//************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void KeyPadWrite(void){
    DMA0CTL &= ~DMAEN;
//    TBCCR0 = 0;
    KB = 'K';
    keypressed = 0x01;
    while (KB == 'K') {   
    KeyPadPortOut |=  0x0E;
    KeyPadPortOut &=  0xFE;
    //if((KeyPadPortIn & 0x10) == 0) KB = 'D'; //unused
    //else if ((KeyPadPortIn & 0x20) == 0) KB = 'E'; // unused
    if ((KeyPadPortIn & 0x40) == 0) KB = '0';
   // else if ((KeyPadPortIn & 0x80) == 0) KB = 'F'; // unused
    
    KeyPadPortOut |= 0x0D;
    KeyPadPortOut &= 0xFD;
    if((KeyPadPortIn & 0x10) == 0) KB = 'C';
    else if ((KeyPadPortIn & 0x20) == 0) KB = '9';
    else if ((KeyPadPortIn & 0x40) == 0) KB = '8';
    else if ((KeyPadPortIn & 0x80) == 0) KB = '7'; 
    
    KeyPadPortOut |= 0x0B;
    KeyPadPortOut &= 0xFB;
    if((KeyPadPortIn & 0x10) == 0) KB = 'B';
    else if ((KeyPadPortIn & 0x20) == 0) KB = '6';
    else if ((KeyPadPortIn & 0x40) == 0) KB = '5';
    else if ((KeyPadPortIn & 0x80) == 0) KB = '4';
    
    KeyPadPortOut |= 0x07;
    KeyPadPortOut &= 0xF7;
    if((KeyPadPortIn & 0x10) == 0) KB = 'A';
    else if ((KeyPadPortIn & 0x20) == 0) KB = '3';
    else if ((KeyPadPortIn & 0x40) == 0) KB = '2';
    else if ((KeyPadPortIn & 0x80) == 0) KB = '1';
    }
    delay(debounceVal*50);
    KeyPadPortOut &= ~0x0F;
    KeyPadIntPend &= ~0x02;
    LPM0_EXIT;
 }

//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs1_handler(void){
   
	delay(120*debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
        PrevCount = LineCount;
        clrLEDs();
        keypressed = 0x00;
        DMA0CTL &= ~DMAEN;
//        TBCCR0 = 0;
        first = 1;
	if(PBsArrIntPend & PB0){
	    state = state1;
        LineCount = 0;
        KeyPadIntEn    =  0x02;  // enable will set on on state 1 and state2
        KeyPadIntPend  =  0x00;  // enable will set on on state 1 and state2
        PBsArrIntPend &= ~PB0;
//        TBCCTL1 |= OUTMOD_4;
        Curser = 0;
    } else if(PBsArrIntPend & PB1){
        KeyPadIntEn    =  0x02;  // enable will set on on state 1 and state2
        KeyPadIntPend  =  0x00;  // enable will set on on state 1 and state2
        lcd_clear();
        state = state2;
        PBsArrIntPend &= ~PB1;
        Curser = 0;
    } else if(PBsArrIntPend & PB2){
        state = state3;
        lcd_clear();
        Curser =(Curser+1)%2;
        PBsArrIntPend &= ~PB2;
	}  
	

        
//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}

//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){
  
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){
        
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
          
	LCD_RS(0);   
}

//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){
  
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_Sel &= ~CONTROL_LEG;
        LCD_DATA_SEL &= ~OUTPUT_DATA; 
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	
	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots 
	
	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm(" nop"); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm(" nop");
  asm(" nop");
  LCD_EN(0);
}

//******************************************************************
// DMA1 matrix block addressing
//******************************************************************
void dma1_addresses(char matrix_in[],char matrix_out[]){
    DMA1SA = (void(*)()) &matrix_in;
    DMA1DA = (void(*)()) &matrix_out;
    DMA1SZ = 32;
}
//******************************************************************
// DMA0 matrix line addressing
//******************************************************************
void dma0_addresses(char matrix_in[],char matrix_out[]){
    DMA0SA = (void(*)()) matrix_in;
    DMA0DA = (void(*)()) matrix_out;
    DMA0SZ = 32;
}

//************* *****************************************************
// DMA0 start
//******************************************************************
void dma0_start(){
    DMA0CTL |= DMAREQ + DMAEN;
}
//******************************************************************
// DMA1 start
//******************************************************************
void dma1_start(){
    DMA1CTL |= DMAREQ + DMAEN;
}
//******************************************************************
// DMA0 stop
//******************************************************************
void dma0_stop(){
    DMA0CTL &= ~DMAREQ;
}
//******************************************************************
// DMA1 stop
//******************************************************************
void dma1_stop(){
    DMA1CTL &= ~DMAREQ;
}
///////////////
void initState1_Timer(){
    TBCTL = TBSSEL_1 + MC_2 + ID_0;            // Clock Source: ACLK , contmode
    TBCCR0 = 0x200;                            // count to 2^12
    enable_interrupts();
}

////////////////
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector = TIMER1_B_VECTOR
//__interrupt void TIMER1_B_ISR (void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(TIMER1_B_VECTOR))) TIMER1_B_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//  switch(__even_in_range(TBIV,0x0F))
//  {
//      // case  TA1IV_TACCR2:                 // Vector  2:  TACCR1 CCIFG
//      //               // Rising Edge was captured
//      //       if (!Count)
//      //       {
//      //         //  TA1CTL |= TACLR;        // SMCLK, cont, clear TAR
//      //           REdge1 = TA1CCR2;
//      //           Count++;
//      //       }
//      //       else
//      //       {
//      //           REdge = TA1CCR2;
//      //           if (REdge<REdge1)
//      //             REdge = CLOCK_FREQ+REdge-REdge1;
//      //           else
//      //             REdge = REdge-REdge1;
//      //           Count=0x0;
//      //           LPM0_EXIT; // must be called from ISR only
//      //       }
//      //   break;
//
//      // case  TA1IV_NONE: break;              // Vector  0:  No interrupt
//      case TBIV_TBCCR1:              // Vector  4:  TACCR2 CCIFG
//        press_count = 0;
//        break;
//      // case TA1IV_TAIFG: break;              // Vector 10:  TAIFG
//      // default:  break;
//  }
//


/////// STATE 3 /////////
void state3_func(){

const char shift_val[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

//  TBCTL &= 0x0000;
  P9DIR |= 0x0FF;                            // P9 output
  DMACTL0 = DMA0TSEL_8;                      // DMA channel 0 transfer select 8:  Timer_B (TBCCR0.IFG)
  DMA0CTL |= DMASRCBYTE | DMADSTBYTE;
  DMA0SA = (int)shift_val;                   // Source block address
  DMA0DA = (int)&P9OUT;                      // Destination single address
  DMA0SZ = 0x8;                              // Block size
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMAEN;  // Rpt, inc src
  DMA0CTL|= DMASBDB;                         // DMA transfer: source word to destination byte
  TBCTL = TBSSEL_1 + MC_2 + ID_0;            // Clock Source: ACLK , contmode
  TBCCR0 = 0x200;                            // count to 2^12
  enable_interrupts();
  enterLPM(lpm_mode);                        // Enter LPM0
  DMACTL0 = 0;
  DMA0CTL = 0;

}
//*********************************************************************
//            DMA ISR
//*********************************************************************
#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR (void){
    LPM0_EXIT;
    _BIC_SR(DMAIV);
}
