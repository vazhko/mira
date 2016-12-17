#include 	<pic18.h>
#include 	<osa.h>

#include 	"HardwareProfile.h"
#include	"GenericTypeDefs.h"
#include 	"delay.h"
#include 	"macro.h"
#include 	"eeprom.h"
#include 	"usart.h"


BYTE   RxTxBuff[17];           // RX/TX буффер
WORD    iCs;                    // CRC
BYTE   cData, cAdr, *lp_p;
//BYTE   cPrevSec;

extern double	f_curr_T[8], f_curr_R[8];


/***************************************************************************************/
// Отсылка измеренных данных (0x55 0x45 без CS)
void send_measured_data(double *fdata){
    BYTE i;    
    CLRWDT();
    crc_mb_init(&iCs);
    put_ch(crc_mb_calc(&iCs, RS232_ADR));
    put_ch(crc_mb_calc(&iCs, RxTxBuff[1]));

    lp_p = (BYTE*)fdata/*&f_curr_T[0]*/;
	for (i = 0; i < 32; i ++) {        
        CLRWDT();
        put_ch(crc_mb_calc(&iCs,*lp_p ++));
    }

    crc_send(iCs);  
}


/***************************************************************************************/
static void interrupt HI_ISR(void){

// INT_RS232
if((RCIF)&&(RCIE)){
       //Принимаем
        // адрес
        RxTxBuff[0] = timed_getc();
        
        // сброс от bootloader
		if (RxTxBuff[0] == 0xEA){			 
			 DelayMs_ISR(250);
			 RxTxBuff[0]   =   RCREG;
			 if (RxTxBuff[0] == 0xEA) {
				///lcd_clear();
				///lcd_gotoxy(1,2);
				///lcd_puts(" BOOTLOADING...  ");
				RESET(); 
			}else goto error;
		}
		
        if ((RxTxBuff[0] != RS232_ADR))  goto error;
        if ((FERR)||(OERR))  goto error;
        if (timeout_error) goto error;
        // комманда
        RxTxBuff[1] = timed_getc();
        if ((FERR)||(OERR))  goto error;
        if (timeout_error) goto error;

        // обрабатываем комманду
        switch (RxTxBuff[1]) {
        default:
            //RxTxBuff[1] += 0x80;
        break;
        

        case 0x10: // запись в EEPROM (preset multiple registers)
            // Принимаем заголовок
            // RxTxBuff[6] - количество байт данных
            for (cAdr = 2; cAdr < 7; cAdr ++) {
                RxTxBuff[cAdr] = timed_getc();          
                if ((FERR)||(OERR))  goto error;
                if (timeout_error) goto error;
                CLRWDT();
            }
            // Принимаем данные
            for (cAdr = 7; cAdr < RxTxBuff[6]+9; cAdr ++) {
                RxTxBuff[cAdr] = timed_getc();          
                if ((FERR)||(OERR))  goto error;
                if (timeout_error) goto error;
                CLRWDT();
            }
            // Проверка КС и запись, если ОК
            if (crc_mb_check(&RxTxBuff[0], RxTxBuff[6] + 7)) {
                // todo: запись в еепром
                cData = 7;
                for (cAdr = RxTxBuff[3]; cAdr < RxTxBuff[6]+RxTxBuff[3]; cAdr ++) {
                    EEPROM_WRITE(cAdr, (RxTxBuff[cData]));
                    CLRWDT();
                    cData ++;
                }
                // Ответ
                crc_mb_init(&iCs);
                for (cAdr = 0; cAdr < 6; cAdr ++) {
                    put_ch(crc_mb_calc(&iCs, RxTxBuff[cAdr]));
                    CLRWDT();                                               
                }
                // передача КС
                crc_send(iCs);
                // Обновляем конфигурацию
                OS_Bsem_Set(BS_RAM_REFRASH); 
            } else {
                // todo: передача кода ошибки в КС
            }       
        break;

        
        case 0x03: // чтение из EEPROM (read holding registers)
            for (cAdr = 2; cAdr < 8; cAdr ++) {
                RxTxBuff[cAdr] = timed_getc();          
                if ((FERR)||(OERR))  goto error;
                if (timeout_error) goto error;
                CLRWDT();
            }
            CLRWDT();   
            if (crc_mb_check(&RxTxBuff[0], 6)) { // если КС совпадает, то формируем передачу
                crc_mb_init(&iCs);                          
                put_ch(crc_mb_calc(&iCs, RS232_ADR));
                put_ch(crc_mb_calc(&iCs, 0x03));
                RxTxBuff[5]*=2; // преобраз количество регистров в байты
                put_ch(crc_mb_calc(&iCs, RxTxBuff[5]));                         
                // Передача данных с извлечением
                for (cAdr = RxTxBuff[3]; cAdr < (RxTxBuff[5]+RxTxBuff[3]); cAdr ++) {
                    cData = EEPROM_READ(cAdr);                              
                    put_ch(crc_mb_calc(&iCs, cData));
                }   
            }
            // передача КС
            crc_send(iCs);
            break;

            
        case 0x45: // чтение измеренных данных (из пользовательских комманд)
            send_measured_data(&f_curr_T[0]);
        break;
        
        case 0x46: // чтение измеренных данных (из пользовательских комманд)
            send_measured_data(&f_curr_R[0]);
        break;
        	
        case 0x41: // запуск ШИМ
                put_ch(crc_mb_calc(&iCs, RS232_ADR));
    			put_ch(crc_mb_calc(&iCs, 0x41));
    			
    			OS_Bsem_Set(BS_PWM);    			
    			OS_Bsem_Set(BS_PWM_RESET);
        break;
        
        case 0x42: // стоп ШИМ
                put_ch(crc_mb_calc(&iCs, RS232_ADR));
    			put_ch(crc_mb_calc(&iCs, 0x42));
    			
    			OS_Bsem_Reset(BS_PWM);     			
    			OS_Bsem_Set(BS_PWM_RESET);   			
        break;
            
        }//END_SWITCH
        
error:;         
        RCIF = 0;
        CREN = 0;       
        CLRWDT();
        CREN = 1;			
		
	}//END_INT_RS232




if((TMR0IF)&&(TMR0IE)){	// 1ms
	WRITETIMER0(65517);
	TMR0IF = 0;
	OS_Timer();	
	OS_CLRWDT();
}//END_TMR0IF
    
	
}
