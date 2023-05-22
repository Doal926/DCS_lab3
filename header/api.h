#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void printSWs2LEDs(void);
extern void printArr2SWs(char Arr[], int size, unsigned int rate);
extern int printMovingLed(int CurrentLed, int numOfMov, unsigned int rate);
extern void PrintPWM(int PWMon, int PWMoff);
extern void lcd_puts(const char * s);
extern void displayLCD(char sine[], char tring[], char pwmm[]);
extern int displayCount(int CountTime, char time[]);
extern void line_swap(unsigned int i,unsigned int j);
extern void scroll();
extern void initState1(char Start1 [], char Start2 []);
extern int Write_and_Disp_Idiom(int counter, int press_count, char letter);
extern char Find_Letter(int press_count, int last_press);
#endif







