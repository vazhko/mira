#include 	<pic18.h>
#include	"delay.h"
#include	"GenericTypeDefs.h"
#include 	"HardwareProfile.h"
#include 	"macro.h"



//Operation modes
#define 	ADC_NORMAL 			0x00
#define 	ADC_SELF 			0x40
#define 	ADC_ZERO_SCALE 		0x80
#define 	ADC_FULL_SCALE 		0xc0

//Gain settings
#define 		ADC_GAIN_1 		0x00
#define 		ADC_GAIN_2 		0x08
#define 		ADC_GAIN_4 		0x10
#define 		ADC_GAIN_8 		0x18
#define 		ADC_GAIN_16 	0x20
#define 		ADC_GAIN_32 	0x28
#define 		ADC_GAIN_64 	0x30
#define 		ADC_GAIN_128 	0x38

//Polar operations
#define ADC_BIPOLAR 			0x00
#define ADC_UNIPOLAR 			0x04

#define ADC_BUF_ON 				0x02
#define ADC_BUF_OFF 			0x00

//update rates
#define ADC_50 					0x04
#define ADC_60 					0x05
#define ADC_250 				0x06
#define ADC_500 				0x07

//update quartz
#define ADC_24576 				0x04
#define ADC_49152 				0x0C







char spi_read (char DATA);
void adc_init();
WORD adc_one_cycle(BYTE gain);
double adc_get(BYTE gain);
double adc_get_bipolar(BYTE gain);

