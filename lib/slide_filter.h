#include	"HardwareProfile.h"
#include	"GenericTypeDefs.h"
#include 	"macro.h"

/******************************************************************************************/
// ������� � HardwareProfile.h!!!
//#define	FLTR_DATA_COUNT		2		// ���������� �������
//#define	FLTR_ERR_COUNT		1		// ���������� ��������� �����
//#define	FLTR_ERR_VALUE		2.0	// �������� ����������, ����������������, ��� "��������"
//#define	FLTR_MODE2			1

/******************************************************************************************/

typedef struct tagFF{
	double  filter[5];
} FF;


/******************************************************************************************/
void filter(double *data, unsigned char ch, unsigned char do_init);
