#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD + WDTPW;		// Stop WDT
    // Timer B0 config
 
    // Timer B1 config
    TimerB1Buzz_DIR |= 0x04;          // Set P2.2 to output direction from timer to buzzer
    TimerB1Buzz_SEL |= 0x04;          // Set P2.2 to output direction from timer to buzzer
    
  // KeyPad
  KeyPadPortOut = 0x00;
  KeyPadPortSel = 0x00;
  KeyPadPortDir = 0x0f;
  
  KeyPadIntSel &= ~0x02;
  KeyPadIntDir &= ~0x02;
  KeyPadIntEn   =  0x00;  // enable will set on on state 1 and state2
  KeyPadIntEdge |= 0x02; // pull-down mode
  KeyPadIntPend &= ~0xFF;
  // PushButtons Setup
  PBsArrPortSel &= ~0x0f;
  PBsArrPortDir &= ~0x0f;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0c;         // pull-down mode
  PBsArrIntEn = PBEnalebottums;
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts 
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
   TBCTL = TBSSEL_2 + MC_1 + TBCLR;        // SMCLK, cont, clear TAR
   TACTL = MC_1 + TASSEL_1;    // configure for 345 ms
   TACCR0 = 10220;
   TACCR2 = 20;
   TBCCR1 = 100;
   TBCCR0 = 0;
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
  /*
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + SREF_0 + MSC; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_3 + ADC10SSEL_3 + SHS_0 + CONSEQ_2 + ADC10DIV_7;                  // input A3
  ADC10AE0 |= 0x08;                         // PA.3 ADC option select
  ADC10CTL0 |= ENC + ADC10SC; 
	//write here ADC congiguration code
  */
//------------------------------------------------------------------------------------- 
//            DMA configuration 
//-------------------------------------------------------------------------------------
}              
void DMAConfig(void){
//  DMACTL0 = DMA0TSEL_0;
//  DMA0CTL = DMADT_0+DMAEN;                    //channel 0 = single transfer
//  DMA1CTL = DMADT_1+DMAEN;                    //channel 1 = block transfer
 // DMA0DA = (void (*) ()) &TBCCR0;
    DMA0CTL |= DMADT_2 | DMASRCINCR_3 | DMADSTINCR_3 | DMAEN; // Enable DMA0, block transfer, no address increment
    DMA0CTL |= DMASRCBYTE | DMADSTBYTE;
    DMA0CTL |= DMA0TSEL_0;
}
           
             

 
             
             
            
  

