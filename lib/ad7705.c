#include	"ad7705.h"

///////////////////////////////////////////////////////////////////////////
static BYTE _databuf, __gie;
///////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
#pragma inline spi_read
char spi_read (char DATA) {
    CLRWDT();
    __gie = (BYTE)GIE;
    GIE = 0;
    SSPBUF = DATA;      
    do NOP();    while (!BF);
    _databuf = SSPBUF;
    GIE = (bit)__gie;
    return _databuf;
}

///////////////////////////////////////////////////////////////////////////
/*adc_init(ADC_24576 | ADC_250)*/
void adc_init(){

	ADC_DRDY_TRIS = 1;	

#if RS232_ADR <= 3
	ADC_CS = 0;
	ADC_CS_TRIS = 0;
#endif	
	
	// аппаратный сброс
	DelayMs(100);
	LC0 = 1;
  	DelayMs(100);
	LC0 = 0;
	DelayMs(100);
	LC0 = 1;
	DelayMs(100);
	DelayMs(250);
	
	// программный сброс
	spi_read (0xff);
	spi_read (0xff);
	spi_read (0xff);
	spi_read (0xff);
	
  	spi_read (0x20);
  	spi_read (ADC_24576 | ADC_50);	
	//spi_read (0x10);
  	//spi_read (1);
			
}

///////////////////////////////////////////////////////////////////////////
WORD adc_one_cycle(BYTE gain){
	WORD hi_low;

	CLRWDT();
		
	spi_read (0x10);	
	spi_read (gain);
		
	do{ 
		NOP();			
	}while((ADC_DRDY)/*&&(wait)*/);
		
	spi_read (0x38);  
	BYTE_1(hi_low) = spi_read (0);  
	BYTE_0(hi_low) = spi_read (0);
	//spi_read (0x10);  
	//spi_read (1);
	
	CLRWDT();
	
return hi_low;
}

////////////Опрос и преобразование результатов АЦП///////////////////////
// sample:
// f_data = adc_get(ADC_GAIN_16 | ADC_UNIPOLAR | ADC_SELF);
double adc_get(BYTE gain){
	WORD result;


	result = (adc_one_cycle(gain));
	
	gain = (1 << ((gain & 0b00111000) >> 3));
	return (double)result / gain / 0xffff;
}

////////////Опрос и преобразование результатов АЦП///////////////////////
double adc_get_bipolar(BYTE gain){
	WORD result;


	result = (adc_one_cycle(gain));
	
	gain = (1 << ((gain & 0b00111000) >> 3));
	return ((double)result - 0x8000) *2.0 / gain / 0xffff;
}
