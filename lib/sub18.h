#include <pic18.h>
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "macro.h"
#include "delay.h"


/******************************************************************************************/
#ifndef MAX_UNIPRINT
#define MAX_UNIPRINT 999.9
#endif

#ifndef MIN_UNIPRINT
#define MIN_UNIPRINT (-999.9)
#endif

/******************************************************************************************/
double polynom(BYTE adress, const double x, BYTE fit);

void replace(char* str, char old, char new);

void print_a_b(char* buf, double f_data, char a, char b);

void pwm1_load(WORD load);
void pwm2_load(WORD load);




