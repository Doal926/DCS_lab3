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

#define   Song1Size      55
#define   Song2Size      48
#define   Song3Size      41
#define   RecordSize     33


// Times for Each note
#define  Note0   501 //1047
#define  Note1   473 //1109
#define  Note2   446 //1175
#define  Note3   421 //1245
#define  Note4   397 //1319
#define  Note5   375 //1397
#define  Note6   354 //1480
#define  Note7   334 //1568
#define  Note8   316 //1661
#define  Note9   298 //1760
#define  Note10  281 //1865
#define  Note11  265 //1976
#define  Note12  250 //2093

#define Song1Line1 Note2,Note7,Note7,Note7,Note2,Note4,Note4,Note2,Note11,Note11,Note9,Note9,Note7,Note2,Note7,Note7,Note7,Note2
#define Song1Line2 Note4,Note4,Note2,Note11,Note11,Note9,Note9,Note7,Note2,Note7,Note7,Note7,Note2,Note7,Note7,Note7,Note7,Note7,Note7,Note2
#define Song1Line3 Note7,Note7,Note7,Note2,Note7,Note7,Note7,Note2,Note4,Note4,Note2,Note11,Note11,Note9,Note9,Note7

#define Song2Line1 Note0,Note5,Note9,Note0,Note5,Note9,Note9,Note7,Note7,Note7,Note7,Note9,Note10,Note10,Note10,Note12,Note10,Note9,Note7
#define Song2Line2 Note9,Note9,Note9,Note9,Note5,Note7,Note9,Note10,Note10,Note10,Note12,Note10,Note10,Note9,Note5,Note9,Note12,Note12
#define Song2Line3 Note0,Note4,Note7,Note10,Note9,Note7,Note5,Note5,Note5,Note5

#define Song3Line1 Note4,Note7,Note4,Note4,Note7,Note4,Note12,Note11,Note9,Note7,Note5,Note2,Note5,Note9,Note7
#define Song3Line2 Note7,Note5,Note4,Note2,Note0,Note4,Note7,Note4,Note4,Note7,Note4,Note12,Note11,Note9,Note7
#define Song3Line3 Note5,Note2,Note5,Note9,Note7,Note7,Note5,Note4,Note2,Note0

#define Song1 {Song1Line1,Song1Line2,Song1Line3,0}
#define Song2 {Song2Line1,Song2Line2,Song2Line3,0}
#define Song3 {Song3Line1,Song3Line2,Song3Line3,0}

// ADC abstraction
#define ADCArrPortDir     P1DIR
#define ADCArrPortSel     P1SEL

// TimerB1 abstraction
#define TimerB1Buzz_DIR      P2DIR
#define TimerB1Buzz_SEL      P2SEL


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

#define PBEnalebottums     0x07   // Change to enable pb3

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
#define lcd_clear()		lcd_cmd(0x01)
#define lcd_putchar(x)		lcd_data(x)
#define lcd_goto(x)		lcd_cmd(0x80+(x))
#define lcd_cursor_right()	lcd_cmd(0x14)
#define lcd_cursor_left()	lcd_cmd(0x10)
#define lcd_display_shift()	lcd_cmd(0x1C)
#define lcd_home()		lcd_cmd(0x02)
#define cursor_off              lcd_cmd(0x0C)
#define cursor_on               lcd_cmd(0x0F) 
#define lcd_function_set        lcd_cmd(0x3C) // 8bit,two lines,5x10 dots 
#define lcd_new_line            lcd_cmd(0xC0)                                  

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
extern void DMAConfig(void);

#endif



