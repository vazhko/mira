#include "HardwareProfile.h"

extern unsigned char delayus_variable;

#if (PIC_CLK == 4000000)
	#define dly125n please remove; for 32Mhz+ only
	#define dly250n please remove; for 16Mhz+ only
	#define dly500n please remove; for 8Mhz+ only
	#define dly1u asm("nop")
	#define dly2u dly1u;dly1u
#elif (PIC_CLK == 8000000)
	#define dly125n please remove; for 32Mhz+ only
	#define dly250n please remove; for 16Mhz+ only
	#define dly500n asm("nop")
	#define dly1u dly500n;dly500n
	#define dly2u dly1u;dly1u
#elif (PIC_CLK == 16000000)
	#define dly125n please remove; for 32Mhz+ only
	#define dly250n asm("nop")
	#define dly500n dly250n;dly250n
	#define dly1u dly500n;dly500n
	#define dly2u dly1u;dly1u
#elif (PIC_CLK == 20000000)
	#define dly200n asm("nop")
	#define dly400n dly250n;dly250n
	#define dly2u dly400n;dly400n;dly400n;dly400n;dly400n
#elif (PIC_CLK == 40000000)
	#define dly125n asm("nop")
	#define dly250n dly125n;dly125n
	#define dly500n dly250n;dly250n
	#define dly1u dly500n;dly500n
	#define dly2u dly1u;dly1u
#else
	#error delay.h - please define pic_clk correctly
#endif

//
//delay routine

#if PIC_CLK == 4000000
	#define DelayDivisor 4
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 4") //on PIC18F, it is asm("goto $ - 4") and on PIC16F core it is asm("goto $ - 2")
#elif PIC_CLK == 8000000
	#define DelayDivisor 2
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 4")
#elif PIC_CLK == 16000000
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop")
	#define Jumpback asm("goto $ - 4")
#elif PIC_CLK == 20000000
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop"); asm("nop")
	#define Jumpback asm("goto $ - 6")
#elif PIC_CLK == 40000000
	#define DelayDivisor 1
	#define WaitFor1Us asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop")
	#define Jumpback asm("goto $ - 12")
#else
	#error delay.h - please define PIC_CLK correctly
#endif


#define DelayUs(x) { \
			delayus_variable=(unsigned char)(x/DelayDivisor); \
			asm("movlb (_delayus_variable) >> 8"); \
			WaitFor1Us; } \
			asm("decfsz (_delayus_variable)&0ffh,f"); \
			Jumpback;
	




#define LOOP_CYCLES_CHAR	9							//how many cycles per loop, optimizations on
#define timeout_char_us(x)	(long)(((x)/LOOP_CYCLES_CHAR)*(PIC_CLK/1000000/4))

#define LOOP_CYCLES_INT		16							//how many cycles per loop, optimizations on
#define timeout_int_us(x)	(long)(((x)/LOOP_CYCLES_INT)*(PIC_CLK/1000000/4))



//if lo byte is zero, faster initialization by 1 instrucion
#define timeout_int_lobyte_zero_us(x)	(long)(((x)/LOOP_CYCLES_INT)*(PIC_CLK/4.0)&0xFF00)

//function prototypes
void DelayBigUs(unsigned int cnt);
void DelayMs(unsigned char cnt);
void DelayMs_ISR(unsigned char cnt);
void DelayBigMs(unsigned int cnt);
void DelayS(unsigned char cnt);
void DelayS_ISR(unsigned char cnt);



