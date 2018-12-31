/**********************************************************
 * @file       : test_3593.c
 * @author     : Oguzhan Armutcu
 * @date       : 15 May 2018
 * Description : main file for 3593 test app
 *       	-- Master Reset
 -- ACLK register = 0x18
 -- read ACLK register for SPI test.
 -- (no change) FLAG/INTERRUPT ASSIGNMENT REGISTER : 0X00 (default)
 -- R1FLAG = 0 => not empty
 -- TCR <= 0x30 -- TFLIP OFF, TMODE ON, SELFTEST ON, RATE OFF(HIGH SPEED)
 -- TFIFO <= 0xAA55AA30
 *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "IOExpander.h"
#include "test3593.h"
#include "3593ext.h"
#include "359x.h"

int ExDev1, ExDev2, ExDev3;
#define DEBUG 1

spi_t spi;

int initSPI(void);

int main(void) {
	unsigned char inputBuf_Ex[2];
	//Using sysfs we need to write the gpio number to /sys/class/gpio/export
	gpio_export(I2C_RST_L);

	//SET DIRECTION
	gpio_set_dir(I2C_RST_L, "out"); 	// I2C_RST_L  PIN will be output

	gpio_set_value(I2C_RST_L, 1); // Set to high

	usleep(500);

	unsigned short OutputState[3];
	ExDev1 = initI2C(EXP1ADDR);
	ExDev2 = initI2C(EXP2ADDR);
	ExDev3 = initI2C(EXP3ADDR);
	/*
	 *  I2C Expanders I/O Configuration
	 */
	IOEXP_Write_config_reg(ExDev1, EXP1CONFIG);
	IOEXP_Write_config_reg(ExDev2, EXP2CONFIG);
	IOEXP_Write_config_reg(ExDev3, EXP3CONFIG);

	/*
	 *                       I2C EXPANDER ADDRESS = 0X74
	 *    P0_0------->INPUT
	 *    P0_1------->INPUT
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6 = OUTPUT          --> LOW
	 *    P0_7 = OUTPUT          --> HIGH
	 *    P1_0 = OUTPUT          --> LOW
	 *    P1_1 = OUTPUT          --> LOW
	 *    P1_2 = OUTPUT          --> HIGH
	 *    P1_3 = OUTPUT          --> HIGH
	 *    P1_4 = OUTPUT          --> HIGH
	 *    P1_5 = HI3593_MR       --> LOW
	 *    P1_6 = OUTPUT          --> LOW
	 *    P1_7 = OUTPUT          --> LOW
	 */
	//OutputState[0] = 0x1ebf;
	OutputState[0] = ( Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2
			| Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 |
			Output_Low_P0_6 | Output_High_P0_7 | Output_Low_P1_0
			| Output_Low_P1_1 | Output_High_P1_2 | Output_High_P1_3 |
			Output_High_P1_4 | Output_Low_P1_5 | Output_Low_P1_6
			| Output_Low_P1_7);
	/*
	 *                       I2C EXPANDER ADDRESS = 0X75
	 *    P0_0------->INPUT
	 *    P0_1------->INPUT
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6 = LED D900                                               --> LOW
	 *    P0_7 = LED D901                                               --> LOW
	 *    P1_0------->INPUT
	 *    P1_1------->INPUT
	 *    P1_2 ------>INPUT
	 *    P1_3 ------>INPUT
	 *    P1_4 ------>INPUT
	 *    P1_5 ------>INPUT
	 *    P1_6 = LED D900                                               --> HIGH
	 *    P1_7 = LED D901                                               --> LOW
	 */
	//OutputState[1] = 0x7fbf;
	OutputState[1] = ( Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2
			| Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 |
			Output_Low_P0_6 | Output_Low_P0_7 | Config_Input_P1_0
			| Config_Input_P1_1 | Config_Input_P1_2 | Config_Input_P1_3 |
			Config_Input_P1_4 | Config_Input_P1_5 | Output_High_P1_6
			| Output_Low_P1_7);
	/*
	 *                       I2C EXPANDER ADDRESS = 0X76
	 *    P0_0 = OUTPUT          --> HIGH
	 *    P0_1 = OUTPUT          --> LOW
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6------->INPUT
	 *    P0_7------->INPUT
	 *    P1_0------->INPUT
	 *    P1_1------->INPUT
	 *    P1_2 ------>INPUT
	 *    P1_3 ------>INPUT
	 *    P1_4 ------>INPUT
	 *    P1_5 ------>INPUT
	 *    P1_6 = OUTPUT          --> LOW
	 *    P1_7 = OUTPUT          --> HIGH
	 */
	//OutputState[2] = 0xbffd;
	OutputState[2] = ( Output_High_P0_0 | Output_Low_P0_1 | Config_Input_P0_2
			| Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 |
			Config_Input_P0_6 | Config_Input_P0_7 | Config_Input_P1_0
			| Config_Input_P1_1 | Config_Input_P1_2 | Config_Input_P1_3 |
			Config_Input_P1_4 | Config_Input_P1_5 | Output_Low_P1_6
			| Output_High_P1_7);

	IOEXP_Write_output_reg(ExDev1, OutputState[0]);
	IOEXP_Write_output_reg(ExDev2, OutputState[1]);
	IOEXP_Write_output_reg(ExDev3, OutputState[2]);

	unsigned short temp;

	if (initSPI() < 1)
		printf("Fail init SPI");

	printf("\n\nThis test is for HI3593 Prototype Acceptance Test \n");
	printf("%s %s", __DATE__, __TIME__);

	EXT_3593_MasterReset();

	usleep(100000);
	printf("\nAfter Master Reset\n");
	if (EXT_3593_ReadRT1() == 0)
		printf("RECEIVER_1 FIFO IS FULL.\n");
	else
		printf("RECEIVER_1 FIFO IS EMPTY.\n");

	if (EXT_3593_ReadRT2() == 0)
		printf("RECEIVER_2 FIFO IS FULL.\n");
	else
		printf("RECEIVER_2 FIFO IS EMPTY.\n");

	GEN_3593_WriteRegNum_OneByte( OPCODE_WRITE_ACLK_DIV, DATA_ACLK_12MHZ, &spi);

	if (GEN_3593_ReadRegNum_OneByte( OPCODE_READ_ACLK_DIV, &spi) != 0x18) {
		printf("Reading SPI test is failed.\n");
	} else {
		printf("Reading SPI test successful.\n");
	}

	//HIZ = 0, TFLIP = 0, TMODE = 1, SELFTEST = 0, ODDEVEN = 0, TPARITY = 0, RATE = 0
	GEN_3593_WriteRegNum_OneByte(OPCODE_WRITE_TRANSMIT_CONTROL,
	DATA_WRITE_TRANSMIT_CONTROL_REGISTER, &spi);

	if (GEN_3593_ReadRegNum_OneByte( OPCODE_READ_TRANSMIT_CONTROL, &spi)
			!= 0x30) {
		printf("Writing to Transmit Control Register is failed.\n");
	} else
		printf("Writing to Transmit Control Register is successful.\n");

	temp = GEN_3593_ReadRegNum_OneByte(OPCODE_READ_TRANSMIT_STATUS, &spi);

	GEN_3593_WriteRegNum_MultiBytes(OPCODE_WRITE_ARINC429_FIFO,
	DATA_WRITE_ARINC429_FIFO, &spi);

	printf("Transmit 0%x to ARINC429 FIFO.\n", DATA_WRITE_ARINC429_FIFO);

	usleep(326000);

	printf("After Transmiting 0x%x\n", DATA_WRITE_ARINC429_FIFO);
	if (EXT_3593_ReadRT1() == 0)
		printf("RECEIVER_1 FIFO IS FULL.\n");
	else
		printf("RECEIVER_1 FIFO IS EMPTY.\n");

	if (EXT_3593_ReadRT2() == 0)
		printf("RECEIVER_2 FIFO IS FULL.\n");
	else
		printf("RECEIVER_2 FIFO IS EMPTY.\n");

	if (GEN_3593_ReadRegNum_MultiBytes(OPCODE_READ_RECEIVER1_ARINC429_FIFO,
			&spi) == DATA_WRITE_ARINC429_FIFO) {
		printf("TX-RECEIVER1 LOOPBACK TEST SUCCESSFUL.\n");
	} else {
		printf("TX-RECEIVER1 LOOPBACK TEST IS FAILED.\n");

	}

	if (GEN_3593_ReadRegNum_MultiBytes(OPCODE_READ_RECEIVER2_ARINC429_FIFO,
			&spi) == DATA_WRITE_ARINC429_FIFO ) {
		printf("TX-RECEIVER2 LOOPBACK TEST SUCCESSFUL.\n");
	} else {
		printf("TX-RECEIVER2 LOOPBACK TEST IS FAILED.\n");
	}

}

int initSPI(void) {

	/* Open spidev1.0 with mode 0 and max speed 10MHz */
	if (spi_open(&spi, "/dev/spidev1.1", 0, 10000000) < 0) {
		fprintf(stderr, "spi_open(): %s\n", spi_errmsg(&spi));
		return 0;
	}
	return 1;
}
