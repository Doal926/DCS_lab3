#ifndef _bsp_H_
#define _bsp_H_

//#include  <msp430g2553.h>          // MSP430x2xx
#include  <msp430xG46x.h>  // MSP430x4xx

#define   MINUTES_VAL       60
#define   COUNT_VALUE       60
#define   debounceVal      250
#define   SMCLK_FREQ       0x100000
#define   CLOCK_FREQ       0x10000
//#define   M
//#define   N

#define   RecordSize     32

// ADC abstraction
#define ADCArrPortDir     P1DIR
#define ADCArrPortSel     P1SEL

// TimerB1 abstraction
#define TimerB1_DIR      P2DIR
#define TimerB1_SEL      P2SEL


//KeyPAd
#define KeyPadPortIn	   P10IN 
#define KeyPadPortOut	   P10OUT
#define KeyPadPortSel      P10SEL 
#define KeyPadPortDir      P10DIR 
 
#define KeyPadIntPort      P2IN
#define KeyPadIntSel       P2SEL 
#define KeyPadIntDir       P2DIR
#define KeyPadIntEn        P2IE
#define KeyPadIntEdge      P2IES
#define KeyPadIntPend      P2IFG
// PushButtons abstraction
#define PBsArrPort	       P1IN
#define PBsArrIntPend	   P1IFG
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08

#define PBEnablebuttons    0x07   // Change to enable pb3
#define LEDsArrPort        P9OUT
// PWM
#define PWMConSEL         P2SEL
#define PWMConDir         P2DIR
#define PWMConPort        P2OUT

// #define CHECKBUSY	1  // using this define, only if we want to read from LCD

#ifdef CHECKBUSY
	#define	LCD_WAIT lcd_check_busy()
#else
	#define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)	(!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)	(!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction 

#define LCD_RS(a)	(!a ? (P2OUT&=~0X40) : (P2OUT|=0X40)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)	(!a ? (P2DIR&=~0X40) : (P2DIR|=0X40)) // P2.6 pin direction  

#define LCD_RW(a)	(!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)	(!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_Sel P2SEL 
#define CONTROL_LEG 0xE0
#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset
   
#define LCD_DATA_WRITE	P1OUT
#define LCD_DATA_DIR	P1DIR
#define LCD_DATA_READ	P1IN
#define LCD_DATA_SEL	P1SEL
/*---------------------------------------------------------
  END CONFIG
-----------------------------------------------------------*/
#define FOURBIT_MODE	0x0
#define EIGHTBIT_MODE	0x1
#define LCD_MODE        FOURBIT_MODE
   
#define OUTPUT_PIN      1	
#define INPUT_PIN       0	
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00	

#define LCD_STROBE_READ(value)	LCD_EN(1), \
				asm("nop"), asm("nop"), \
				value=LCD_DATA_READ, \
				LCD_EN(0) 

#define	lcd_cursor(x)		lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()	    	lcd_cmd(0x01)
#define lcd_putchar(x)		lcd_data(x)
#define lcd_goto(x)		    lcd_cmd(0x80+(x))
#define lcd_cursor_right()	lcd_cmd(0x14)
#define lcd_cursor_left()	lcd_cmd(0x10)
#define lcd_display_shift()	lcd_cmd(0x1C)
#define lcd_home()		    lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

extern void GPIOconfig(void);
//extern void TIMERconfig(void);
//extern void ADCconfig(void);
extern void DMAConfig(void);

#endif



