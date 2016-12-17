#include <pic18.h>
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "macro.h"
#include "delay.h"


/*
#define _DATA					PORTB
#define LCD_TRIS_DATA    		TRISB
#define LCD_DATA         		LATB
//#define LCD_TRIS_UPR			TRISE
#define LCD_TRIS_RW				TRISE0
#define LCD_TRIS_RS 			TRISE1
#define LCD_TRIS_EN 			TRISE2
//#define LCD_UPR					PORTE
#define rw						LATE0
#define rs						LATE1
#define en						LATE2
*/

/////////////////////////////////////////////////////
#define INI                0b00111000
#define format             0b00111100
#define dsp_on             0b00001100
#define dsp_off            0b00001000
//#define LCD_CLS()              1
#define cur_sh_on          0b00000110
#define cur_sh_off         0b00000100
#define cur_return         0b10000000
//
#define lcd_read_busy      0b11111001
#define lcd_read           0b11111001
#define lcd_write          0b11111010
////////macroses
#define LCD_CLS()          		lcd_send_cmd(1)
#define LCD_CUR_BL_ON()     	lcd_send_cmd(0b00001111)
#define LCD_CUR_BL_OFF()    	lcd_send_cmd(0b00001110)
#define LCD_CUR_ON()     		lcd_send_cmd(0b00001110)
#define LCD_CUR_OFF()    		lcd_send_cmd(0b00001100)


// 16 или 20 символов в строке
#ifndef LCD20SYMB
#define LCD20 0
#else
#define LCD20 4
#endif


//#define lcd_cur_sh_on    lcd_send_cmd(0b00000000)
//#define lcd_cur_sh_off   lcd_send_cmd(0b00001100)
/////////////////////////////////////////////////////
#define   A_    	0x41
#define   B_    	0xA0
#define   V_    	0x42
#define   G_    	0xA1
#define   D_    	0xE0
#define   E_   	0x45
#define   YO_    0xA2
#define   ZH_    0xA3
#define   Z_    	0xA4
#define   I_    	0xA5
#define   YI_    0xA6
#define   K_    	0x4B
#define   L_    	0xA7
#define   M_    	0x4D
#define   N_    	0x48
#define   O_    	0x4F
#define   P_    	0xA8
#define   R_    	0x50
#define   S_    	0x43
#define   T_    	0x54
#define   U_    	0xA9
#define   F_    	0xAA
#define   H_    	0x58
#define   TC_    0xE1
#define   CH_    0xAB
#define   SH_    0xAC
#define   SCH_   0xE2
#define   MZ_    0x62
#define   TZ_    0xAD
#define   II_    0xAE
#define   EE_    0xAF
#define   YU_    0xB0
#define   YA_    0xB1


#define   a_    	0x61
#define   b_    	0xB2
#define   v_    	0xB3
#define   g_    	0xB4
#define   d_    	0xE3
#define   e_    	0x65
#define   yo_    	0xB5
#define   zh_    	0xB6
#define   z_    	0xB7
#define   i_    	0xB8
#define   yi_    	0xB9
#define   k_    	0xBA
#define   l_    	0xBB
#define   m_    	0xBC
#define   n_    	0xBD
#define   o_    	0x6F
#define   p_    	0xBE
#define   r_    	0x70
#define   s_    	0x63
#define   t_    	0xBF
#define   u_    	0x79
#define   f_    	0xE4
#define   h_    	0x78
#define   ts_   	0xE5
#define   ch_    	0xC0
#define   sh_    	0xC1
#define   sch_    0xE6
#define   tz_    	0xC2
#define   ii_    	0xC3
#define   mz_    	0xC4
#define   ee_    	0xC5
#define   yu_    	0xC6
#define   ya_     0xC7

void lcd_init(void);
void lcd_goto( BYTE x, BYTE y);
void lcd_putc(char c);
void lcd_puts(const char *s);
void lcd_puts_ex(const char *s);
void lcd_send_cmd(char data);
void lcd_prog(void);
