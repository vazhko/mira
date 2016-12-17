#include 	<pic18.h>


	

/******************************************************************************************/
		
void write___ee(unsigned int adr, unsigned char *var, unsigned char byte){	
while(byte--){
	EEPROM_WRITE(adr+byte, (unsigned char)(*(((unsigned char *)var)+byte)));
	};		
}

/******************************************************************************************/

void read___ee(unsigned int adr, unsigned char *var, unsigned char byte){
while(byte--){	
	(unsigned char)(*(((unsigned char *)var)+byte))=EEPROM_READ(adr+byte);
	};		
}

/******************************************************************************************/

