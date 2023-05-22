#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer


char data_matrix_in[10][32] = {
                         "An apple a day keeps the doctor ",
                         "Climb on the bandwagon          ",
                         "Dot the i's and cross the t's   ",
                         "He who pays the piper calls the ",
                         "The pen is mightier than the    ",
                         "The pot calling the kettle black",
                         "Shed crocodile tears            ",
                         "Close but no cigar              ",
                         "Cut from the same cloth         ",
                         "Strike while the iron's hot     "
};

char data_matrix_out[10][32]; // = {
//                          "An apple a day keeps the doctor ",
//                          "Climb on the bandwagon          ",
//                          "Dot the i's and cross the t's   ",
//                          "He who pays the piper calls the ",
//                          "The pen is mightier than the    ",
//                          "The pot calling the kettle black",
//                          "Shed crocodile tears            ",
//                          "Close but no cigar              ",
//                          "Cut from the same cloth         ",
//                          "Strike while the iron's hot     "
//};
char key0[2] = "0T";
char key1[2] = "1G";
char key2[2] = "2H";
char key3[2] = "3I";
char key4[2] = "4K";
char key5[2] = "5L";
char key6[2] = "6M";
char key7[2] = "7O";
char key8[2] = "8P";
char key9[2] = "9Q";
char key_a[2] = "CJ";
char key_b[2] = "DN";
char key_c[2] = "ER";
char key_d[3] = "FYZ";
char key_star[3] = "BWX";
char key_hashtag[2] = "AS";
char LCD_Arr1[16] ="                ";
char LCD_Arr2[16] ="                ";
//--------------------------------------------------------------------
//            PWM 75% implant
//--------------------------------------------------------------------       
void PrintPWM(int PWMon, int PWMoff){
    PWMConPort |= 0x80;   // Port on 
    delay(PWMon);
    PWMConPort &= ~0x80;   // Port off
    delay(PWMoff);
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){
  while(*s){
    lcd_data(*s++);
  }
}
//******************************************************************
// Init states
//******************************************************************
void initState1(char Start1 [], char Start2 []){
    // Initializtion of timer and LCD message
    initState1_Timer();
    lcd_clear();
    lcd_home();
    lcd_puts(Start1);
    lcd_new_line;
    lcd_puts(Start2);
}

void line_swap(unsigned int i,unsigned int j){
    DMAConfig();
//    dma0_addresses(data_matrix_in[i],data_matrix_out[j]);
//    dma0_start();
//    dma0_addresses(data_matrix_in[j],data_matrix_out[i]);
//    dma0_start();
    unsigned int line;
    for (line = 0; line < 10; line++){
        if (line==i){
            dma0_addresses(data_matrix_in[i],data_matrix_out[j]);
            dma0_start();
            enterLPM(lpm_mode);
//            dma0_stop();
        }
        else if (line==j){
            dma0_addresses(data_matrix_in[j],data_matrix_out[i]);
            dma0_start();
            enterLPM(lpm_mode);
//            dma0_stop();
        }
        else{
            dma0_addresses(data_matrix_in[line],data_matrix_out[line]);
            dma0_start();
            enterLPM(lpm_mode);
//            dma0_stop();
        }

    }
}

////////////////////// REMOVE //////////////////////////

void scroll(){
    int idiom_num = 0;
    char line_1[16];
    char line_2[16];
    int i;
    while(state == 2){

        //wait for keystroke
        enterLPM(lpm_mode);

        //when we wake up:
        if(KB == '3') idiom_num ++;
        if(KB == '1') idiom_num--;
        KB = 0;
        idiom_num = idiom_num % 10;
        for (i=0; i<16; i++){
            line_1[i] = data_matrix_out[idiom_num][i];
            line_2[i] = data_matrix_out[idiom_num][i+16];
        }

        lcd_clear();
        lcd_home();
        lcd_puts(line_1);
        lcd_new_line;
        lcd_puts(line_2);
    }

}
///////////////////////////////////////////////////////////
//
int Write_and_Disp_Idiom(int counter, int press_count, char letter){
  // Add the last letter to the idiom and display to the LCD
  if (press_count > 1){
    counter -= 1;
  }
  if (counter<16){
    LCD_Arr1[counter] = letter;
    lcd_clear();
    lcd_home();
    lcd_puts(LCD_Arr1);
    lcd_new_line;
    lcd_puts(LCD_Arr2);
  }
  else{
    LCD_Arr2[counter-16] = letter;
    lcd_clear();
    lcd_home();
    lcd_puts(LCD_Arr1);
    lcd_new_line;
    lcd_puts(LCD_Arr2);

  }

  counter += 1;
  return counter;
}
////////////////////////////////////////////////////////////////
char Find_Letter(int press_count, int last_press){
  // Select the letter based on the press_count and last_press
  char letter;

  if (KB != last_press){
    press_count = 0;
  }
  last_press = KB;
  if (KB =='0') {
    letter = key0[press_count % 2];
  }
  else if (KB=='1') {
    letter = key1[press_count % 2];
  }
  else if (KB=='2') {
    letter = key2[press_count % 2];
  }
  else if (KB=='3') {
    letter = key3[press_count % 2];
  }
  else if (KB=='4') {
    letter = key4[press_count % 2];
  }
  else if (KB=='5') {
    letter = key5[press_count % 2];
  }
  else if (KB=='6') {
    letter = key6[press_count % 2];
  }
  else if (KB=='7') {
    letter = key7[press_count % 2];
  }
  else if (KB=='8') {
    letter = key8[press_count % 2];
  }
  else if (KB=='9') {
    letter = key9[press_count % 2];
  }
  else if (KB=='A') {
    letter = key_a[press_count % 2];
  }
  else if (KB=='B') {
    letter = key_b[press_count % 2];
  }
  else if (KB=='C') {
    letter = key_c[press_count % 2];
  }
  else if (KB=='D') {
    letter = key_d[press_count % 3];
  }
  else if (KB=='E') {
    letter = key_star[press_count % 3];
  }
  else {
    letter = key_hashtag[press_count % 2];
  }
  return letter;
}





