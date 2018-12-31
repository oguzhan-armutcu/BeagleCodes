/**********************************************************
 * @file       : IOExpander.c
 * @author     : Oguzhan Armutcu
 * @date       : 16 Nov 2017
 * Description : This is the main file for test application.
 * This application is intended to test
 * IOEXPANDER PCA9539 tests for another test application.
 *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "IOExpander.h"
#include "gpio-utils.h"

unsigned char data [3];

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
//	unsigned char read_data[2];
//	unsigned char reg[1];
//
//	reg[0] = ( PCA9539_OUTPUT_0 );
//	if( write ( device, reg, 1) != 1 )
//	{
//		printf("Fail writing");
//	}
//
//	if(read(device, read_data, 2) != 2)
//	{
//#if DEBUG
//		printf("Error : Input/Output error \n");
//		printf("__________________________\n");
//#endif
//		return 0;
//	}
//
//	unsigned char data[3];
//	int len;
//
//	unsigned char mask_buf_lsb = (buf);
//	unsigned char mask_buf_msb = (( buf >> 8 ));
//
//	read_data[0]  = ( read_data[0]  | mask_buf_lsb );
//
//	read_data[1]  = ( read_data[1]  | mask_buf_msb );
//
//	data [0] = PCA9539_OUTPUT_0 ;
//
//	data [1] =read_data[0];
//	data [2] =read_data[1];
//
//
//
//	len = sizeof(data);
//
//	if ( write ( device, data, len) != len )
//	{
//		printf("Fail writing");
//	}

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
