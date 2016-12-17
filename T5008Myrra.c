/******************************************************************************************/


/******************************************************************************************/
#include 	<pic18.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<stdio.h>
#include 	<math.h>
#include 	<osa.h>

__CONFIG(4,DEBUGEN & LVPDIS);	// required ICD2 settings
__CONFIG(6,UNPROTECT);		// enable writing to flash

#include 	"T5008Myrra.h"


/******************************************************************************************/
double	f_curr_T[8], f_curr_R[8];
// температура фонового излучателя и миры
double	f_TR, f_TM; 

char print_buf[21];
char print_buf2[21];

double f_sum_dT, f_load2;
double f_task_abs, f_v_abs, f_task_diff, f_task;

/******************************************************************************************/
/*interrupts*/
//#include	"i_T5008.c"

/******************************************************************************************/
/******************************************************************************************/
void ch_setup(BYTE ch){
	TRISA0 = 0;
	TRISA1 = 0;
	TRISA2 = 0;
	if TESTBIT(ch, 0) SETBIT(LATA, 0); else CLRBIT(LATA, 0);
	if TESTBIT(ch, 1) SETBIT(LATA, 1); else CLRBIT(LATA, 1);
	if TESTBIT(ch, 2) SETBIT(LATA, 2); else CLRBIT(LATA, 2);
}

/******************************************************************************************/
void task_display(){
//static BYTE i_blink;	// для мигалок
BYTE temp;	
	
	

#ifdef LCD_ON
	DelayMs(250);
	DelayMs(250);
	

    lcd_init();
    LCD_CLS();    
    lcd_prog();   
   	DelayMs(1);     
    lcd_init();
    LCD_CLS();    
    lcd_goto(4,1);    
    lcd_puts("TRITON 5008");
    
    lcd_goto(1,2);
    lcd_puts(" Ver. ");
    lcd_puts(__DATE__);  

	
#endif	

	
	for(;;){	
		OS_Bsem_Wait(BS_DISPLAY);

		
#ifdef LCD_ON	
	

		// заполнение массива
		print_a_b( &print_buf2[0], TR1,2,2);
		sprintf(print_buf, "%s@", &print_buf2[0]);
		lcd_goto(0,0);
		lcd_puts_ex(print_buf);
				
		
		print_a_b( &print_buf2[0], TR2,2,2);
		sprintf(print_buf, "%s@", &print_buf2[0]);
		lcd_goto(8,0);
		lcd_puts_ex(print_buf);		
	
		print_a_b( &print_buf2[0], TR3,2,2);
		sprintf(print_buf, "%s@", &print_buf2[0]);
		lcd_goto(0,1);
		lcd_puts_ex(print_buf);
				
		
		print_a_b( &print_buf2[0], TM1,2,2);
		sprintf(print_buf, "%s@", &print_buf2[0]);
		lcd_goto(8,1);
		lcd_puts_ex(print_buf);	
		
		
		print_a_b( &print_buf2[0], f_sum_dT,2,2);
		sprintf(print_buf, "S=%s    ", &print_buf2[0]);
		lcd_goto(0,2);
		lcd_puts_ex(print_buf);
				
		
		print_a_b( &print_buf2[0], f_load2,3,1);
		sprintf(print_buf, "L=%s%%  ", &print_buf2[0]);
		lcd_goto(8,2);
		lcd_puts_ex(print_buf);	
		
		
		print_a_b( &print_buf2[0], f_v_abs,2,2);
		sprintf(print_buf, "V=%s  ", &print_buf2[0]);
		lcd_goto(0,3);
		lcd_puts_ex(print_buf);
				
		
		print_a_b( &print_buf2[0], f_task,3,2);
		sprintf(print_buf, "T%s ", &print_buf2[0]);
		lcd_goto(8,3);
		lcd_puts_ex(print_buf);
	
				
		
	
#endif
					

		
		if((FERR)||(OERR)){		
			if (RCIF); temp=RCSTA, temp=RCSTA;		 		
			ADDEN = 1;
			CREN = 	0;
			CREN = 	1;
		}
	}


}
/******************************************************************************************/
void task_eeprom_to_ram(){


	for(;;){	
		OS_Bsem_Wait(BS_RAM_REFRASH);			
		//EE_TO_RAM(PW_ID, id);
	}

}


/***************************************************************************************/
#define MAX 0.8
double pid(BYTE reset){
	
	static double f_task_diff_prev, f_task_abs_prev, f_v_prev, f_task_inc, f_dT;
	 
	double delta, f_Pid = 0, f_curr;	
	double f_P, f_I, f_D;
	

	
	EE_TO_RAM(PF_T_DIFF, f_task_diff);
	if(f_task_diff != f_task_diff_prev) {
		reset = 1;
		f_task_diff_prev = f_task_diff;
	}
	
	EE_TO_RAM(PF_T_ABS, f_task_abs);	
	if(f_task_abs != f_task_abs_prev) {
		reset = 1;
		f_task_abs_prev = f_task_abs;		
	}
	
	EE_TO_RAM(PF_V_ABS, f_v_abs);
	if(f_v_abs != f_v_prev) {
		reset = 1;
		f_v_prev = f_v_abs;	
		f_task = f_TR; // фиксируем текущую температуру излучателя для расчета скорости
		f_task_inc = f_v_abs/60.0;
		//определяем напрввление скорости
		if((f_task - f_task_abs) > 0.0) f_task_inc *= (-1.0); else f_task_inc = fabs(f_task_inc);
		
	}	
	
	
	if(f_v_abs == 0) {
		// режим разности без скорости
		// при разности ==0 лучше просто выключить ШИМ		
		if(f_task_diff == 0.0) reset = 1;
		// иначе работаем ШИМом
		f_task = f_task_diff;
		f_curr = f_TR - f_TM;
				
	} else {
		// режим абсолютной т-ры с регулированием скорости		

		
		// почти достигли уставки, обнуляем прирост температуры
		if(fabs(f_task - f_task_abs) <= 2.0) {
			f_task_inc = 0;			
		}
		
		// почти достигли уставки, фиксируем задание
		if(f_task_inc == 0) {
			f_task = f_task_abs;
		}
		
		f_task += f_task_inc;
		f_curr = f_TR; 
	}
	
	
	
	if(reset){
		f_dT = 0.0;
		f_sum_dT = 0.0;
		return 0.0;	
	}	
		
	delta = f_task - f_curr;
	
	
	if(delta > 2.0)delta = 2.0;
	if(delta < -2.0)delta = -2.0;	

		
	EE_TO_RAM(PF_P, f_P);
	EE_TO_RAM(PF_I, f_I);
	EE_TO_RAM(PF_D, f_D);	
	
	f_sum_dT += delta * f_I;
	
	if(f_sum_dT >  1.0)	f_sum_dT =  1.0;
	if(f_sum_dT < -1.0)	f_sum_dT = -1.0;
	
		
	f_Pid = f_P*delta + f_sum_dT + f_D*(delta - f_dT);
	f_dT = delta;
	
		
	
	if(f_Pid > MAX) {f_Pid = MAX; };
	if(f_Pid < (-1.0)*MAX) {f_Pid = (-1.0)*MAX; };
	
	if(f_Pid > 1.0) {f_Pid = 1.0; };
	if(f_Pid < (-1.0)) {f_Pid = (-1.0);};	

	
	return f_Pid;
}


/******************************************************************************************/
void pwm_load(double f_load){
	WORD load;
	
	f_load2 = f_load*100.0;
	
	if(f_load > 0.0){
		load = (WORD)(f_load * 1023.0);
		CCP2CON = 0;
		CCP1CON = 0b00001111;
		LATC1 = 0;
		pwm1_load(load);
	} else if(f_load < 0.0)  {
		CCP1CON = 0;
		CCP2CON = 0b00001111;
		f_load *= -1.0;
		load = (WORD)(f_load * 1023.0);
		LATC2 = 0;
		pwm2_load(load);
	} else {	
		CCP1CON = 0;
		CCP2CON = 0;
		LATC2 = 0;
		LATC1 = 0;		
		
	}		
}


/******************************************************************************************/
void task_pwm(void){
	double f_load;
	
	
	PR2 	= 0xff;
	T2CON 	= 0b01111110;
	
	//CCP1CON = 0b00001111;
	//pwm1_load(0);
	//LATC1 = 0;
	pwm_load(0.0);
	LATC1 = 0;
	LATC2 = 0;
	
	TRISC1 = 0;
	TRISC2 = 0;
	
	for(;;){
		// дискрет 1 с
		OS_Delay(1000);
		
		if(OS_Bsem_Check(BS_PWM_RESET)){			
			OS_Bsem_Reset(BS_PWM_RESET);			
			LATC1 = 0;
			pwm1_load(0);
			pid(1);
			
		} else	if (OS_Bsem_Check(BS_PWM)) {
			f_load = pid(0);
			
			// опред. полярность
			pwm_load(f_load);
			
			
		} else {
			// сброс
			pwm_load(0);
			pid(1);
		}
		

		
	}

}


/******************************************************************************************/
void task_mesure(){
	
	static char ___gie;
	static double f_res; 
	static double f_ref, f_zero, f_k;
	static BYTE i_ch = 0, k, i;
	static BYTE b_begin = 1;
	static unsigned int	ch_en;	
	
	ch_setup(0);
	
	
#ifdef ADC_ON		
	adc_init();
	adc_get_bipolar(ADC_GAIN_32 | ADC_BIPOLAR | ADC_SELF | ADC_BUF_ON);
	adc_get_bipolar(ADC_GAIN_32 | ADC_BIPOLAR | ADC_SELF | ADC_BUF_ON);
#endif	

	for(;;){		
		
		OS_Bsem_Wait (BS_ADC_START);	
		

		ch_setup(i_ch);		
		
		OS_Bsem_Set(BS_DISPLAY);
		
		OS_Bsem_Set(BS_ADC_START);
		
		OS_Delay(250);		
		
				
#ifdef ADC_ON

		f_res = adc_get_bipolar(ADC_GAIN_32 | ADC_BIPOLAR | ADC_BUF_ON);
		
		
		EE_TO_RAM(PF_REF, f_ref);
		EE_TO_RAM(PF_Z, f_zero);
		
		f_res *= f_ref;
		f_res += f_zero;
		
		
		EE_TO_RAM((PF_Z0 + i_ch * 4), f_zero);
		EE_TO_RAM((PF_K0 + i_ch * 4), f_k);		
				
		f_res *= f_k;
		f_res += f_zero;
#else
		f_res = ((double)i_ch)*1.11;
#endif		

		filter(&f_res, i_ch, b_begin); // фильтруем f_res

	
		___gie = OS_DI(); 
		f_curr_R[i_ch] = f_res; 
		OS_RI(___gie);
		f_res = polynom(PF_POLYNOM1, (f_res*10.0), 2);
		___gie = OS_DI(); 
		f_curr_T[i_ch] = f_res; 
		OS_RI(___gie);	
				
				

		
		EE_TO_RAM(PW_CH_EN, ch_en);		
		do{
			i_ch ++;
			i_ch &= 7;
			if(i_ch == 0) b_begin = 0;
		} while ((TESTBIT((ch_en), i_ch)) == 0);			

		
		
		k = 0;
		f_TR = 0.0;
		for(i = 0; i < 3; i ++){
			if(TESTBIT(ch_en, i + 4)){
				f_TR += f_curr_T[i + 4];
				k ++;
			}	
		}
			
		if(k == 0){
			f_TR = 0.0;
		} else {	
			f_TR /= (double)k;	
		}
		
		f_TM = TM1;	
		

		//debug		
		///f_TR = TR2;
		///TM1 = 0.0;
		
	}		

}

/******************************************************************************************/
void main(){


	LATA		=	0;
	ADCON0	=		0;
	ADCON1 	= 	0x06;
	
	
	LATB		=		0;
	LATC		=		0;
	CLRWDT();
	LATA	=	0;
	PORTA  	= 	0;
	ADCON0	=	0;
	ADCON1 	= 	0x06;
	//RA0-RA2	ADR
	//RA5 - in 	RDY
	TRISA 	=  	0b11100000;
	
	//LCD
	TRISB 	=  	0;
	RBPU 	= 	0;
	
	//C0 - RST
	//C3 - out CLK
	//C4 - in DATAIN
	//C5 - out DATAOUT
	//C6 - TX
	//C7 - RX
	TRISC 	=  	0b10010110;
	
	
	//RD3 - OFF
	//RD5 - SDA
	//RD6 - SCL
	//RD7 - TX/RX
	TRISE	=	0xe8;
	TRISD	=	0x00;

	////////////////////SPI/////////////
	SSPSTAT	=	0;
	SSPCON1	=	0x31;
	//////////////Timer T0//////////////
	T0CON 	= 	0b10000110;
	//////////////Extern////////////////




	// set rs232
	uart_init();	
	
	
	PEIE    	= 	1;
	TXEN 		= 	1;
	RCIE 		= 	1;
	RCIP		=	1;
	IPEN		=	1;
	
	TMR0IP = 1;
	TMR0IE = 1;
	TMR0IF = 1;
	

	
	OS_Init();
	
	OS_Task_Create(0, task_eeprom_to_ram);	
	OS_Task_Create(0, task_display);
	OS_Task_Create(0, task_mesure);			
	OS_Task_Create(0, task_pwm);	
	
	OS_Bsem_Set(BS_RAM_REFRASH);
	OS_Bsem_Set(BS_ADC_START);
	OS_Bsem_Reset(BS_DISPLAY);
	OS_Bsem_Reset(BS_PWM);
	///OS_Bsem_Set(BS_PWM);

	
	
	OS_EI();
	OS_Run(); 

}
