#include "lcd.h"


/***************************************************************************************/
#define IOFF						uGIE=(BYTE)GIE,GIE=0
#define ION							if(uGIE)GIE=1
/***************************************************************************************/

static WORD 	timeout;

/***************************************************************************************/
char const c_lcd_prog[] = {	//0 - gradus
	0b00000110,
	0b00001001,
	0b00001001,
	0b00000110,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	
	0b10000000,
	0b10000000,
	0b00000001,
	0b00000011,
	0b00000101,
	0b10001001,										
	0b11111111,
	0b10000000, 
	
	0b10000000,
	0b10000100,
	0b11111110,
	0b01111111,
	0b11111110,
	0b10000100,										
	0b10000000,
	0b10000000,
	0
};

/***************************************************************************************/
void _lcd_strobe(){
	LCD_EN = 1;
	CLRWDT();
	CLRWDT();
	CLRWDT();
	CLRWDT();
	CLRWDT();
	CLRWDT();
	LCD_EN =0;
}

/***************************************************************************************/
void _lcd_ready(void){
  BYTE TEMP;//int1
  WORD b_lcd_err = 0;
  CLRWDT();
  LCD_DATA =(0);
  LCD_RS = 0; LCD_EN = 0; LCD_RW = 1;
  LCD_TRIS_DATA=(0xff);
  timeout = 0;
  //Check busy flag
  do{
    LCD_EN = 1;
	 	CLRWDT();
	 	CLRWDT();
    TEMP = LCD_PORT_DATA&0x80;
    LCD_EN = 0;
    if(++timeout > 2000)  b_lcd_err = 1;
    }while((TEMP)&&(!b_lcd_err));
  
  LCD_TRIS_DATA=(0x00);
  LCD_RS = 0; LCD_EN = 0; LCD_RW = 0;
}


/***************************************************************************************/
void lcd_send_cmd(char data){
BYTE fl = (BYTE)GIE;GIE =0;

	_lcd_ready();
	LCD_TRIS_DATA=(0);
	LCD_RS = 0;	LCD_EN = 0;	LCD_RW = 0;
	LCD_DATA=(data);
	_lcd_strobe();
	
	GIE = (bit)fl;
}

/***************************************************************************************/
char lcd_read_byte() {
	char res;
	BYTE fl = (BYTE)GIE;GIE =0;
	
	LCD_DATA = 0x00;
	LCD_TRIS_DATA=(0xFF);
	LCD_RW = 1;
	
	CLRWDT();	
	CLRWDT();
	LCD_EN = 1;
	CLRWDT();
	CLRWDT();
	CLRWDT();
	res = LCD_PORT_DATA;
	CLRWDT();
	LCD_EN = 0;
	CLRWDT();
	
	GIE = (bit)fl;
	return res;
}

/***************************************************************************************/
void lcd_init(void){
	BYTE i;
	DelayMs(100);
	
	LCD_TRIS_DATA=(0);
	LCD_DATA=(INI);
	LCD_TRIS_RW = 0;
	LCD_TRIS_RS = 0;
	LCD_TRIS_EN = 0;
	LCD_RS = 0;	LCD_EN = 0;	LCD_RW = 0;
	
	for(i = 0; i<= 3; i++){
		DelayMs(100);
		_lcd_strobe();
	}
	
	lcd_send_cmd(0b00001110);
	//lcd_send_cmd(cls);
	lcd_send_cmd(cur_sh_on);
	LCD_CUR_OFF();
	//enable_interrupts(global);
}

/***************************************************************************************/
void lcd_goto( BYTE x, BYTE y) {       //todo: отладить!!!
BYTE fl = (BYTE)GIE;GIE =0;

   switch(y){
      case 0:
         lcd_send_cmd(0x80 + x);
         break;
      case 1:
         lcd_send_cmd(0xC0 + x);
         break;
      case 2:
         lcd_send_cmd(0x90 + x + LCD20);
         break;
      case 3:
         lcd_send_cmd(0xD0 + x + LCD20);
         break;
   }
	GIE = (bit)fl;
}

/***************************************************************************************/
void lcd_putc(char c) {
	BYTE fl = (BYTE)GIE;GIE =0;
	CLRWDT();
	
	LCD_RS = 0;
	LCD_TRIS_DATA=(0);
	_lcd_ready();
	LCD_RS = 1;	LCD_EN = 0;	LCD_RW = 0;
	LCD_DATA=(c);
	_lcd_strobe();
	LCD_DATA=(0);
	LCD_RS = 0;
	
	GIE = (bit)fl;
	CLRWDT();
}

/***************************************************************************************/
void lcd_puts(const char *s){
	while(*s) lcd_putc(*s++);
}

/***************************************************************************************/
// печать 0 или 1
void lcd_puts_ex(const char *s){
	while(*s) {
	  
	  	switch(*s){
		  	// gradus
		  	case '@':
		  		lcd_putc(1);		  		
		  	break;
		  	
		  	default:
		  	lcd_putc(*s);
		  	break;
		}
		s++;
		
	}  
	
}


/***************************************************************************************/
void lcd_prog(void){
	BYTE i_char, i = 0;
	
	i_char = 0b01001000;

	do{
		lcd_send_cmd(i_char ++);
		CLRWDT();
		lcd_putc(c_lcd_prog[i++]);
		CLRWDT();
	}while (c_lcd_prog[i]) ;

}
