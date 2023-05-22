//#include <msp430.h>

#include  "../header/api.h"         // private library - API layer
#include  "../header/app.h"         // private library - APP layer
enum FSMstate state;
enum SYSmode lpm_mode;
unsigned char first;
//char Start1[16] = "Please insert   ";
//char Start2[16] = "your idiom:     ";
//char LCD_arr1[16] ="                ";
//char LCD_arr2[16] ="                ";
//int press_count = 0;
//char letter;
//int counter;
//int last_press;

//char idiom_recorder[RecordSize];
void main(void){
  state = state0;       // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  while(1){
    switch(state){
      case state0:
        enterLPM(lpm_mode);
        break;


//      case state1:
//        if (first){
//            initState1(Start1, Start2);
//            first = 0;
//        }
//        if (keypressed){
//          letter = Find_Letter(press_count, last_press);
//          press_count += 1;
//          counter = Write_and_Disp_Idiom(counter,press_count,letter,LCD_arr1,LCD_arr2);
//        }
//        enterLPM(lpm_mode); // wait for TimerA0 interupts or KeyPad interupts
//        if (counter == 32){
//          counter = 0;
//        }
//        break;

      case state2:
        line_swap(0, 2);
        scroll();
        enterLPM(lpm_mode); // wait for TimerA0 interupts
        break;

     // case state3:
      //   enterLPM(lpm_mode);  // All happens backdoors
      //      break;
    }
  }
}





