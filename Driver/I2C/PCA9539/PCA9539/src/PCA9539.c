/**********************************************************
 * @file       : PCA9539.c
 * @author     : Oguzhan Armutcu
 * @date       : 16 Nov 2017
 * Description : This is the main file for test application.
 * This application is intended to test
 * IOEXPANDER PCA9539 tests for another test application.
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "IOExpander.h"
#include "gpio-utils.h"

#define DEBUG 1
unsigned char data [3];
//unsigned char buf;
int main(void) {

	unsigned char inputBuf_Ex[2];
	//Using sysfs we need to write the gpio number to /sys/class/gpio/export
	gpio_export(I2C_RST_L);

	//SET DIRECTION
	gpio_set_dir(I2C_RST_L, "out"); 	// I2C_RST_L  PIN will be output

	gpio_set_value(I2C_RST_L, 1); // Set to high

	usleep(500);



	int ExDev1, ExDev2, ExDev3;
	unsigned short OutputState [3];
	ExDev1 = initI2C(EXP1ADDR);
	ExDev2 = initI2C(EXP2ADDR);
    ExDev3 = initI2C(EXP3ADDR);




    /*
     *  I2C Expanders I/O Configuration
     */
	IOEXP_Write_config_reg( ExDev1, EXP1CONFIG);
	IOEXP_Write_config_reg( ExDev2, EXP2CONFIG);
	IOEXP_Write_config_reg( ExDev3, EXP3CONFIG);


	/*
	 *                       I2C EXPANDER ADDRESS = 0X74
	 *    P0_0------->INPUT
	 *    P0_1------->INPUT
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6 = OUTPUT--> LOW
	 *    P0_7 = OUTPUT--> HIGH
	 *    P1_0 = OUTPUT--> LOW
	 *    P1_1 = OUTPUT--> HIGH
	 *    P1_2 = OUTPUT--> HIGH
	 *    P1_3 = OUTPUT--> HIGH
	 *    P1_4 = OUTPUT--> HIGH
	 *    P1_5 = OUTPUT--> LOW
	 *    P1_6 = OUTPUT--> LOW
	 *    P1_7 = OUTPUT--> LOW
	 */
	//OutputState[0] = 0x1ebf;
	OutputState[0] = ( Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2 | Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 | \
			                              Output_Low_P0_6 | Output_High_P0_7 |  Output_High_P1_0 | Output_High_P1_1 | Output_High_P1_2 | Output_High_P1_3 |  \
										  Output_Low_P1_4 | Output_Low_P1_5 | Output_Low_P1_6 | Output_Low_P1_7);
	/*
	 *                       I2C EXPANDER ADDRESS = 0X75
	 *    P0_0------->INPUT
	 *    P0_1------->INPUT
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6 = LED D900--> LOW
	 *    P0_7 = LED D901--> HIGH
	 *    P1_0------->INPUT
	 *    P1_1------->INPUT
	 *    P1_2 ------>INPUT
	 *    P1_3 ------>INPUT
	 *    P1_4 ------>INPUT
	 *    P1_5 ------>INPUT
	 *    P1_6 = LED D900--> HIGH
	 *    P1_7 = LED D901--> LOW
	 */
	//OutputState[1] = 0x7fbf;
	OutputState[1] = ( Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2 | Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 | \
			                              Output_Low_P0_6 | Output_High_P0_7 | Config_Input_P1_0 | Config_Input_P1_1 | Config_Input_P1_2 | Config_Input_P1_3 |  \
										  Config_Input_P1_4 | Config_Input_P1_5 | Output_High_P1_6 | Output_Low_P1_7);
	/*
	 *                       I2C EXPANDER ADDRESS = 0X76
	 *    P0_0 = OUTPUT--> LOW
	 *    P0_1 = OUTPUT--> LOW
	 *    P0_2------->INPUT
	 *    P0_3------->INPUT
	 *    P0_4------->INPUT
	 *    P0_5------->INPUT
	 *    P0_6 = OUTPUT--> LOW
	 *    P0_7 = OUTPUT--> HIGH
	 *    P1_0------->INPUT
	 *    P1_1------->INPUT
	 *    P1_2 ------>INPUT
	 *    P1_3 ------>INPUT
	 *    P1_4 ------>INPUT
	 *    P1_5 ------>INPUT
	 *    P1_6= OUTPUT--> LOW
	 *    P1_7 =OUTPUT--> HIGH
	 */
	//OutputState[2] = 0xbffd;
	OutputState[2] = ( Output_High_P0_0 | Output_Low_P0_1 | Config_Input_P0_2 | Config_Input_P0_3 | Config_Input_P0_4 | Config_Input_P0_5 | \
			                             Config_Input_P0_6 | Config_Input_P0_7 | Config_Input_P1_0 | Config_Input_P1_1 | Config_Input_P1_2 | Config_Input_P1_3 |  \
										  Config_Input_P1_4 | Config_Input_P1_5 | Output_High_P1_6 | Output_Low_P1_7);


	IOEXP_Write_output_reg(ExDev1, OutputState[0]);
	IOEXP_Write_output_reg(ExDev2, OutputState[1]);
	IOEXP_Write_output_reg(ExDev3, OutputState[2]);

	usleep(200000);



	IOEXP_Read_input_reg(ExDev1, inputBuf_Ex);
	printf("\nEXPANDER 1 :\n");
	printf("\nFirst byte:0x%x",inputBuf_Ex[0]);
	printf("\nSecond byte:0x%x",inputBuf_Ex[1]);

	IOEXP_Read_input_reg(ExDev2, inputBuf_Ex);
	printf("\nEXPANDER 2 :\n");
	printf("\nFirst byte:0x%x",inputBuf_Ex[0]);
	printf("\nSecond byte:0x%x",inputBuf_Ex[1]);

	IOEXP_Read_input_reg(ExDev3, inputBuf_Ex);
	printf("\nEXPANDER 3 :\n");
	printf("\nFirst byte:0x%x",inputBuf_Ex[0]);
	printf("\nSecond byte:0x%x",inputBuf_Ex[1]);

	return EXIT_SUCCESS;
}


int initI2C( unsigned char address )
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device,  I2C address
	ioctl(file, I2C_SLAVE, address);
	return file;
}



/** Write to Configuration Registers
 * @pre
 * @param[in] adress  Slave Address
 * @param[in] buf Data to Register
 * @return 0:FAIL 1:PASS
 */
int IOEXP_Write_config_reg ( int device, unsigned short buf )
{
	unsigned char data[3];
	int len;

	data [0] = PCA9539_DIRECTION_0; // Register Command

	data [1] = buf ;

	data [2] = buf >> 8 ;

	len = sizeof(data);

	if( write ( device, data, len) != len )
	{
		printf("Fail writing");
	}
	return 1;
}

/** Write to Output Port Register
 * @pre
 * @param[in] adress  Slave Address
 * @param[in] buf Data to Register
 * @return 0:FAIL 1:PASS
 */
int IOEXP_Write_output_reg ( int device, unsigned short buf )
{

	unsigned char data[3];
	int len;
	data [0] = PCA9539_OUTPUT_0 ;
	data [1] = buf;
	data [2] = ( buf >> 8 );
	len = sizeof(data);
	if ( write ( device, data, len) != len )
	{
		printf("Fail writing");
	}
	return 1;
}


/** Read to Input Port Register
 * @pre
 * @param[in] adress  Slave Address
 * @param[in] buf Data to Register
 * @return 0:FAIL 1:PASS
 */
int IOEXP_Read_input_reg ( int device, unsigned char *buf)
{
	//unsigned char data[2];
	unsigned char reg[1];

	reg[0] = ( PCA9539_INPUT_0 );
	if ( write ( device, reg, 1) != 1 )
	{
		printf("Fail writing");

	}


	if(read(device, buf, 2) != 2)
	{
#if DEBUG
		printf("Error : Input/Output error \n");
		printf("__________________________\n");
#endif
		return 0;
	}



	return 1;
}

