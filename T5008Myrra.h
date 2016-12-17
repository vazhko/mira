

/******************************************************************************************/

#define LCD_ON
#define ADC_ON


/******************************************************************************************/
// для настройки
// править установить номер тритона
#define RS232_ADR		0x55

#define SCALE			1.0
#define ZERO			0.0


/******************************************************************************************/
#include 	"HardwareProfile.h"
#include	"GenericTypeDefs.h"
#include 	"delay.h"
#include 	"macro.h"




#ifdef LCD_ON
	#include 	"lcd.h"
#endif

#ifdef ADC_ON
	#include 	"ad7705.h"	
#endif




#include 	"eeprom.h"
#include 	"usart.h"



#include 	"sub18.h"
#include 	"slide_filter.h"



#define ERR_VAL	10000.0

#define TR1	f_curr_T[4]
#define TR2	f_curr_T[5]
#define TR3	f_curr_T[6]

#define TM1	f_curr_T[0]
