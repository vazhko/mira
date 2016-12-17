#include 	<pic18.h>
#include 	<stdlib.h>
#include 	<stdio.h>
#include 	<math.h>
#include 	"slide_filter.h"

#ifdef	FLTR_MODE2
// коэффициенты от малых весов к большим
static	const double coef[]={0.01033386, 0.20756121, 0.56420986};
#else
// двойное скольжение
static const double coef[]={0.1111111111,0.2222222222,0.3333333333};
#endif


/******************************************************************************************/
static	unsigned char	err_summ[FLTR_DATA_COUNT];
FF ff[FLTR_DATA_COUNT];
	
/******************************************************************************************/
void filter_ini(double data, unsigned char ch){
	
	BYTE i;
	for(i = 0; i < 5; i ++)
		ff[ch].filter[i] = data;
}

/******************************************************************************************/
void filter(double *data, unsigned char ch, unsigned char do_init){
	double f_delta = 0;	
	
	
	if(do_init){
		filter_ini(*data, ch);
		return;
	}
		

	f_delta	= *data - ff[ch].filter[2];
	MOD_DOUBLE(f_delta);
	
	// защита от одиночек	
	if(f_delta > FLTR_ERR_VALUE){
		err_summ[ch]++;
		if (err_summ[ch] > FLTR_ERR_COUNT){
			filter_ini(*data, ch);
			err_summ[ch] = 0;
			return;
		}
	} else err_summ[ch] = 0;
	

	ff[ch].filter[4] = 	ff[ch].filter[3];
	ff[ch].filter[3] = 	ff[ch].filter[2];
	ff[ch].filter[2] = 	ff[ch].filter[1];
	ff[ch].filter[1] = 	ff[ch].filter[0];
	ff[ch].filter[0] = *data;	
	*data=coef[0]*(ff[ch].filter[0]+ff[ch].filter[4])+coef[1]*(ff[ch].filter[1]+ff[ch].filter[3])+coef[2]*ff[ch].filter[2];				
	ff[ch].filter[2] = *data;
				
}
