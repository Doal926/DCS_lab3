#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void dma0_addresses(char matrix_in[],char matrix_out[]);
extern void dma1_addresses(char matrix_in[],char matrix_out[]);
extern void dma0_start();
extern void dma1_start();
extern void dma0_stop();
extern void dma1_stop();

//LCD stuff
extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_init();
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);
extern void state3_func();
extern __interrupt void PBs_handler(void);
extern void initState1_Timer();
extern unsigned char keypressed;
extern int LineCount,PrevCount;
extern char KB;
extern int Curser;

extern const char F[];
extern const char Line[];
extern const char Finished[];
extern const char Finished2[];

//extern const char song1str[];
//extern const char song2str[];
//extern const char song3str[];
//extern const char recordstr[];
#endif







