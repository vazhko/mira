
#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H


	#define CLOCK_FREQ	40000000
	#define GetSystemClock() CLOCK_FREQ   
	#define GetInstructionClock() CLOCK_FREQ
	#define PIC_CLK		20000000    
    
/******************************************************************************************/
	//LCD8    
	#define LCD_RS			LE1
	#define LCD_EN			LE2
	#define TRIS_LCD_EN2	TRISD6
	#define LCD_RW			LE0 
	#define LCD_DATA		LATB
	#define LCD_PORT_DATA	PORTB 
	#define LCD_TRIS_RS		TRISE1
	#define LCD_TRIS_EN		TRISE0
	#define LCD_TRIS_RW		TRISE2 
	#define LCD_TRIS_DATA	TRISB
	
	//#define LCD20SYMB
    
    
/******************************************************************************************/
	// программный фильтр
	#define	FLTR_DATA_COUNT		8		// количество каналов
	#define	FLTR_ERR_COUNT		1		// количество одиночных помех
	#define	FLTR_ERR_VALUE		2.0		// величина отклонения, классифицируемое, как "одиночка"
	
/******************************************************************************************/

	//ADC	
	#define ADC_DRDY  			RA5
	#define ADC_DRDY_TRIS 		TRISA5
	#define	ADC_CS				LATD3
	#define	ADC_CS_TRIS			TRISD3
	
	
	
	#define FOSC				10000000
	#define BAUD				19200
	#define UART_DIVIDER 			((int)(FOSC/(64UL * BAUD) -1)) /*SPBRG = 	UART_DIVIDER*/
	
	#define RS232_ADR			0x55	
	
	
	#define	PWM1				LATC2
	#define	PWM2				LATC1
	
	#define	PWM1_POL			LATE0
	#define	PWM2_POL			LATE1
	
	#define	OUT40HZ				LATD4


#endif  //HARDWARE_PROFILE_H
