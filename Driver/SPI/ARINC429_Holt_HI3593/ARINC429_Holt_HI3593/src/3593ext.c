/*
 * 3593ext.c
 *
 *  Created on: May 8, 2018
 *      Author: oguzhanarmutcu
 */

#include "3593ext.h"
#include "gpio-utils.h"
#include "IOExpander.h"
#include "test3593.h"

/** Init All GPIOs signal
 * @pre
 * @return N/A
 * @details export all gpio pins
 *
 */
void EXT_3593_initAllGpios(void) {

}

/** HI-3593 Master Reset
 * @pre
 * @return N/A
 * @details Resets the HI-6131 with master reset.
 * It executes a reset sequence in TBD microseconds.
 */
void EXT_3593_MasterReset(void) {

	unsigned char inputBuf_Ex[2];

	unsigned short val = 0;
	if (IOEXP_Read_input_reg(ExDev1, inputBuf_Ex) != 1)
		printf("Error : Reading inputs of Expander 1 error \n");

	val = ((inputBuf_Ex[1] << 8) | (inputBuf_Ex[0]));

	val |= Output_High_P1_5;

	IOEXP_Write_output_reg(ExDev1, val);

	usleep(500);

	val = 0;

	if (IOEXP_Read_input_reg(ExDev1, inputBuf_Ex) != 1)
		printf("Error : Reading inputs of Expander 1 error \n");

	val = ((inputBuf_Ex[1] << 8) | (inputBuf_Ex[0]));

	val &= ~Output_High_P1_5;

	IOEXP_Write_output_reg(ExDev1, val);

	usleep(65000);
}

/** HI-3593 Read R1Flag
 * @pre
 * @return R1Flag pin. Return 1=HIGH, 0=LOW
 * @details Reads P1_2 = HI3593_R1_Flag from Expander 2
 */
int EXT_3593_ReadRT1(void) {

	unsigned char inputBuf_Ex[2];

	unsigned short val = 0;
	if (IOEXP_Read_input_reg(ExDev2, inputBuf_Ex) != 1)
		printf("Error : Reading inputs of Expander 2 error \n");

	val = ((inputBuf_Ex[1] << 8) | (inputBuf_Ex[0]));

	if ( ( val & Output_High_P1_2 ) == Output_High_P1_2)
		return 1;
	return 0;
}

/** HI-3593 Read R2Flag
 * @pre
 * @return R2Flag pin. Return 1=HIGH, 0=LOW
 * @details Reads P1_4 = HI3593_R2_Flag from Expander 2
 */
int EXT_3593_ReadRT2(void) {

	unsigned char inputBuf_Ex[2];

	unsigned short val = 0;
	if (IOEXP_Read_input_reg(ExDev2, inputBuf_Ex) != 1)
		printf("Error : Reading inputs of Expander 2 error \n");

	val = ((inputBuf_Ex[1] << 8) | (inputBuf_Ex[0]));

	if ( ( val & Output_High_P1_4 ) == Output_High_P1_4)
		return 1;
	return 0;
}
