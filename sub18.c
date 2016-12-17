#ifndef UCHAR
#define UCHAR	unsigned char
#endif

#ifndef UINT
#define UINT	unsigned int
#endif

////////////////////////////////////////////////////////////////////////////////////////////
#define byte_0(x)		(UCHAR)(*(((UCHAR *)&x)+0))
#define byte_1(x)		(UCHAR)(*(((UCHAR *)&x)+1))
#define byte_2(x)		(UCHAR)(*(((UCHAR *)&x)+2))
#define byte_3(x)		(UCHAR)(*(((UCHAR *)&x)+3))
#define MOD_DOUBLE(data)	byte_3(data) = byte_3(data)&0x7f; 
////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////EEPROM///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************************/
char eeprom_reads(UCHAR adr){
char data;
	data = EEPROM_READ(adr);
	return data;
	}
	
/******************************************************************************************/	
void eeprom_writes(UCHAR adr, char data){
	EEPROM_WRITE(adr,data);
	}
	
/******************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////
void write_float_ee(UCHAR pEE, double *pFl){
UCHAR i, *pi;
CLRWDT();
pi = (UCHAR*)pFl;
for(i = 0; i <= 3; i++){
  EEPROM_WRITE(pEE++, *pi++);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
//#if  read_float_ee
void read_float_ee(UCHAR pEE, double *pFl){
UCHAR j, *pi;
CLRWDT();
pi = (UCHAR*)pFl;
CLRWDT();
for(j = 0; j <= 3; j++){
  *pi++ = EEPROM_READ(pEE++);
	CLRWDT();
	}
}

/******************************************************************************************/
//
double pow_i(double x, UCHAR y){
double f = 1.0;

if (y == 0) return f;

do{
	CLRWDT();
  f *= x;
	CLRWDT();
  y --;
	} while(y);

return f;
}

/******************************************************************************************/
// ‘унк полином начинает считывание EEPROM с последнего коэфф.
double polynom(UCHAR adress, const double x, UCHAR fit){
double f_temp_1, f_temp_2 = 0;
UCHAR i = 0;

   do{
        read_float_ee(adress, &f_temp_1);
        f_temp_2 = f_temp_2 + pow_i(x, i) * f_temp_1;
				CLRWDT();
        adress += 4;
				CLRWDT();
        } while(i ++ < fit);

   return  f_temp_2;

}


/******************************************************************************************/
#ifndef print_2_2
void print_2_2(char* buf, double f_data){
UCHAR i, sign;

if((f_data > 999.99)||(f_data < -99.99)){
	buf[0] = ' ';
	buf[1] = '-';
	buf[2] = '-';
	buf[3] = '.';
	buf[4] = '-';
	buf[5] = '-';
	buf[6] = 0;
	return;
}


if(f_data < 0) {
	sign = 1;	
	MOD_DOUBLE(f_data); 
}else {		
	sign = 0;
}
f_data *= 100;

i = 6;
while(i){
	i --;
	if (i == 3) continue;
	buf[i] = (UCHAR)((UINT)(float)f_data % 10 + '0');	
	f_data /= 10;
  CLRWDT();
	}

if(buf[0] - '0') goto skip;
if (sign) buf[0] = '-'; else buf[0] = ' ';

skip:;

buf[3] = '.';
buf[6] = 0;
}
#endif


/*************************************************************************************/
// ”ниверсальна€ печать

void print_a_b(char* buf, double f_data, char a, char b){	
UCHAR i, sign;

// перегруз
if((f_data > 999.99)||(f_data < -199.99)){			
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
	CLRWDT();  
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
void pwm1_load(UINT load){
	CCPR1L=((UCHAR)((load)>>2));
	DC1B1=(bit)((load)>>1); 
	DC1B0=(bit)(load);	
}

