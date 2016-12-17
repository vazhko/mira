#include	"usart.h"

BYTE		timeout_error;


/******************************************************************************************/
//#pragma inline uart_init
void uart_init(void){
	BRGH		=	0;					
	SPBRG 		= 	UART_DIVIDER;
	TXEN 		= 	1;
	CREN 		= 	1;
	SPEN		=	1;
}
	
/******************************************************************************************/
//#pragma inline putbyte
void putbyte(BYTE byte){
	TXREG = byte;
	while(!TRMT)	continue;
}
	
/******************************************************************************************/	
BYTE getbyte(void) {
/* set when register is not empty */
	while(!RCIF){		
		NOP();
		continue;
	}
	return RCREG;	
}

/******************************************************************************************/
BYTE timed_getc(void) {
	WORD		timeout;
	
	timeout_error=0;
	timeout=0;
	
	do NOP();	
	while(!RCIF&&(++timeout < 10000));   // 1/2 second
	//delay_us(10);
	if(RCIF){
		return(getbyte());
	} else {
		timeout_error=1;
		return(0);
	}	
}

/******************************************************************************************/
void putstr(const char *s){
	while(*s) putbyte(*s++);
}

/******************************************************************************************/
//#pragma inline crc_mb_init
void	crc_mb_init(WORD *code){
	*code = 0xffff;
}

/******************************************************************************************/
//#pragma inline crc_mb_init
void	crc_mb_init_i(WORD *code){
	*code = 0xffff;
}

/******************************************************************************************/
//#pragma inline crc_mb_calc
BYTE crc_mb_calc_i(WORD *code, BYTE next){
	BYTE flag;
	BYTE i;	
	*code ^= next;
	
	for(i = 0; i < 8; i ++){
		flag = *code & 1;
		*code = *code >> 1;
		if (flag)  *code ^= 0xA001;
	}
	return next;	
}

/******************************************************************************************/
//#pragma inline crc_mb_calc
BYTE crc_mb_calc(WORD *code, BYTE next){
	BYTE flag;
	BYTE i;	
	*code ^= next;
	
	for(i = 0; i < 8; i ++){
		flag = *code & 1;
		*code = *code >> 1;
		if (flag)  *code ^= 0xA001;
	}
	return next;	
}
/******************************************************************************************/
WORD crc_mb_i(BYTE *buffer, BYTE cnt){
	WORD crc;
	BYTE i;
	
	crc_mb_init(&crc);
	for(i = 0; i < cnt; i ++){
		crc_mb_calc(&crc, *buffer++);
	}
	
	*buffer++ = (BYTE)(crc >> 8);
	*buffer 	= (BYTE) crc;
	return 	crc;
}

/******************************************************************************************/
WORD crc_mb(BYTE *buffer, BYTE cnt){
	WORD crc;
	BYTE i;
	
	crc_mb_init(&crc);
	for(i = 0; i < cnt; i ++){
		crc_mb_calc(&crc, *buffer++);
	}
	
	*buffer++ = (BYTE)(crc >> 8);
	*buffer 	= (BYTE) crc;
	return 	crc;
}

/******************************************************************************************/
// Проверка CRC  в count - количество байт в buffer не включая CRC 
BYTE	crc_mb_check(BYTE *buffer, BYTE cnt){
	
	WORD crc;
	BYTE i;
	
	crc_mb_init(&crc);
	
	for(i = 0; i < cnt; i ++){
		crc_mb_calc(&crc, *buffer++);
	}
	
	if (((BYTE)(crc >> 8) == *buffer++)&&((BYTE)crc == *buffer)) return 1; else return 0;
	
}


/******************************************************************************************/
// Проверка CRC  в count - количество байт в buffer не включая CRC 
BYTE	crc_mb_check_i(BYTE *buffer, BYTE cnt){

	WORD _cs;
	BYTE i;
	
	crc_mb_init_i(&_cs);	

	for(i = 0; i < cnt; i ++){
		crc_mb_calc_i(&_cs, *buffer++);
	}	
	
	if (((BYTE)(_cs >> 8) == *buffer++)&&((BYTE)_cs == *buffer++)) {
		NOP();
		return 1; 
	}	else {
		NOP();
		return 0;
	}
	
}
/******************************************************************************************/
//#pragma inline crc_send
void crc_send(WORD crc){
	putbyte((BYTE)(crc >> 8));	
	putbyte((BYTE)crc);
	
}

/******************************************************************************************/
void crc_send_i(WORD crc){
	putbyte((BYTE)(crc >> 8));	
	putbyte((BYTE)crc);	
}

