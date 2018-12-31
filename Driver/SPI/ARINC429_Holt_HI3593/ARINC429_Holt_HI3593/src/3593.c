/*
 * 359x.c
 *
 *  Created on: Feb 12, 2016
 *      Author: oguzhanarmutcu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "359x.h"

/** Write to HI3593 Register according it's Number
 * @pre
 * @param[in] opcode  Opcode Number
 * @param[in] data   data  to write
 * @param[in] struct spi configuration
 * @return N/A
 * @details Writes the data to Register
 * If Register Number is not valid it does nothing.
 */
void GEN_3593_WriteRegNum_OneByte(unsigned char opcode, unsigned char data,
		spi_t *spi) {

	unsigned char spi_data[2];
	unsigned char receivebuf[3];

	spi_data[0] = opcode;
	spi_data[1] = data;

	if (spi_transfer(spi, spi_data, receivebuf, sizeof(spi_data)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(spi));
	}

	return;
}

/** Write to HI3593 Register according it's Number
 * @pre
 * @param[in] opcode  Opcode Number
 * @param[in] data   data  to write
 * @param[in] struct spi configuration
 * @return N/A
 * @details Writes multi datas to Register
 * If Register Number is not valid it does nothing.
 */
void GEN_3593_WriteRegNum_MultiBytes(unsigned char opcode, unsigned long data,
		spi_t *spi) {

	unsigned char spi_data[5];
	unsigned char receivebuf[5];

	spi_data[0] = opcode;
	spi_data[1] = data >> 24;
	spi_data[2] = data >> 16;
	spi_data[3] = data >> 8;
	spi_data[4] = data;
	if (spi_transfer(spi, spi_data, receivebuf, sizeof(spi_data)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(spi));
	}

	return;
}

/** Read HI3593 Register according it's number
 * @pre
 * @param[in] Opcode
 * @param[in] struct spi configuration
 * @return data
 * @details Read HI3593 Register according it's number
 * If Register Number is not valid it returns 0xFFFF.
 */
unsigned short GEN_3593_ReadRegNum_OneByte(unsigned char opcode, spi_t *spi) {

	unsigned short temp;
	unsigned char spi_data[3];
	unsigned char receivebuf[2] = { 0x00 };

	spi_data[0] = opcode;
	if (spi_transfer(spi, spi_data, receivebuf, sizeof(spi_data)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(spi));
	}

	temp = receivebuf[1];

	return (temp);

}

/** Read HI3593 Register according it's number
 * @pre
 * @param[in] Opcode
 * @param[in] struct spi configuration
 * @return data
 * @details Read HI3593 Register according it's number
 * If Register Number is not valid it returns 0xFFFF.
 */
unsigned long GEN_3593_ReadRegNum_MultiBytes(unsigned char opcode, spi_t *spi) {

	unsigned long temp;
	unsigned char spi_data[5];
	unsigned char receivebuf[5] = { 0x00 };

	spi_data[0] = opcode;
	if (spi_transfer(spi, spi_data, receivebuf, sizeof(spi_data)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(spi));
	}

	temp = ((receivebuf[1] << 24) + (receivebuf[2] << 16) + (receivebuf[3] << 8)
			+ (receivebuf[4]));

	return (temp);

}
