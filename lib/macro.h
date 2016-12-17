
/******************************************************************************************/    
#define INPUT_PIN 1
#define OUTPUT_PIN 0


#define	TRUE	1
#define	FALSE	0

/******************************************************************************************/
#define BYTE_0(x)				(BYTE)(*(((BYTE *)&x)+0))
#define BYTE_1(x)				(BYTE)(*(((BYTE *)&x)+1))    
#define BYTE_2(x)				(BYTE)(*(((BYTE *)&x)+2))
#define BYTE_3(x)				(BYTE)(*(((BYTE *)&x)+3))  

/******************************************************************************************/
#define TESTBIT(var, b)   	((var) & (1 <<(b)))
#define SETBIT(var, b)    	((var) |= (1 << (b)))
#define CLRBIT(var, b)    	((var) &= ~(1 << (b)))   
/******************************************************************************************/
#define MOD_DOUBLE(data)	BYTE_3(data) = BYTE_3(data)&0x7f;
	
/******************************************************************************************/	
#ifndef UCHAR
#define UCHAR	unsigned char
#endif

#ifndef UINT
#define UINT	unsigned int
#endif

#ifndef ULONG
#define ULONG	unsigned long
#endif


