#include	"sub18.h"

/******************************************************************************************/
void read_float_ee(BYTE pEE, double *pFl){
	
	BYTE j, *pi;
	pi = (BYTE*)pFl;

	for(j = 0; j <= 3; j++){
	  *pi++ = EEPROM_READ(pEE++);
	
	}
}
/******************************************************************************************/
//
double pow_i(double x, BYTE y){
	double f = 1.0;
	
	if (y == 0) return f;
	
	do{
	
	  f *= x;
	  y --;
	} while(y);
	
	return f;
}

/******************************************************************************************/
// ‘унк полином начинает считывание EEPROM с последнего коэфф.
double polynom(BYTE adress, const double x, BYTE fit){
double f_temp_1, f_temp_2 = 0;
BYTE i = 0;

   do{
        read_float_ee(adress, &f_temp_1);
        f_temp_2 = f_temp_2 + pow_i(x, i) * f_temp_1;
        adress += 4;
   } while(i ++ < fit);

   return  f_temp_2;

}


/******************************************************************************************/
void replace(char* str, char old, char new){
	do{
		if(*str == old)*str = new;
		}while(*++str);
	}
/*************************************************************************************/
// ”ниверсальна€ печать

void print_a_b(char* buf, double f_data, char a, char b){	
	BYTE i, sign;
	
	// перегруз
	if((f_data > MAX_UNIPRINT)||(f_data < MIN_UNIPRINT)){			
		i = a + b + 1;
		buf[i + 2] = 0;
		for (i; i; i--) buf[i] = '-';
		buf[a+1] = '.';
		buf[0] = ' ';
		return;
	}
	
	if(f_data < 0) {
		sign = 1;	
		MOD_DOUBLE(f_data); 
	}else {		
		sign = 0;
	}
	
	
	
	i = b + 1;
	for (i; i; i--) f_data *= 10.0;
	
	i = a+b+3;
	while(i){
		i --;
		if (i == (a+1)) continue;
		buf[i] = (char)((long int)f_data%10) + '0';	
		f_data /= 10; 
	}
	
	if(buf[0] - '0')goto skip; // если большое положительное
	
	// удаление лишних нулей и добавление знака
	for (i=0; i <= a; i++) {
		if(((buf[i] - '0') == 0)&&(i != a)) buf[i] = ' ';
		else {
			if (sign) buf[i-1] = '-'; 
			goto skip;
		}	
	}
	
	skip:;
	
	buf[a+1] = '.';
	buf[a+b+2] = 0;

}

/***************************************************************************************/
void pwm1_load(WORD load){
	CCPR1L=((BYTE)((load)>>2));
	DC1B1=(bit)((load)>>1); 
	DC1B0=(bit)(load);	
}

/***************************************************************************************/
void pwm2_load(WORD load){
	CCPR2L=((BYTE)((load)>>2));
	DC2B1=(bit)((load)>>1); 
	DC2B0=(bit)(load);	
}

/***************************************************************************************/

