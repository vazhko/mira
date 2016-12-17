
#include 	<pic18.h>
#include	"delay.h"
#include	"GenericTypeDefs.h"
#include 	"HardwareProfile.h"
#include 	"macro.h"


/***************************************************************************************/
#define put_ch(n)		putbyte((n));


/***************************************************************************************/
extern void putbyte(unsigned char byte);
extern unsigned char getbyte(void) ;
extern unsigned char timed_getc(void);
extern void putstr(const char *);
extern void crc_mb_init(WORD *);
extern WORD crc_mb(BYTE *, BYTE );
extern BYTE crc_mb_calc(WORD *, BYTE );
extern BYTE	crc_mb_check(BYTE *, BYTE );
extern void crc_send(WORD );
extern void uart_init(void);

extern void crc_mb_init_i(WORD *);
extern WORD crc_mb_i(BYTE *, BYTE );
extern BYTE crc_mb_calc_i(WORD *, BYTE );
extern void crc_send_i(WORD );
extern BYTE	crc_mb_check_i(BYTE *, BYTE );

/***************************************************************************************/
extern BYTE	timeout_error;


