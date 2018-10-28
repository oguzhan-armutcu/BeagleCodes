/**********************************************************
 *
 * @file       : ltc4151.c
 * @author     : Oguzhan Armutcu
 * @date       : 13 Jan 2018
 * Description : LTC4151 -İ2C1
 *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "inc/gpio-utils.h"
#include "inc/ltc4151.h"



/***************** Macros (Inline Functions) Definitions *******************/
#define PRINT_DOUBLE3F(NUM) do { int Integer,Fraction; \
                              Integer = (int) (NUM);\
                              if ((NUM) >= 0.0)\
                              Fraction = (int)( (( (NUM)  - (double) Integer ) )* 1000);\
                              else\
                              Fraction = (int)( -1*(( (NUM)  - (double) Integer ) )* 1000);\
                              printf(" %d.%03d",Integer,Fraction);\
                              } while (0)

void main (  )
{

	int i;
	double voltage_at_pin [3];
	double value[3];
	volatile unsigned short raw[3];
	unsigned char data[6] = {0};

	//Using sysfs we need to write the gpio number to /sys/class/gpio/export
	gpio_export(I2C_RST_L);

	//SET DIRECTION
	gpio_set_dir(I2C_RST_L, "out"); 	// I2C_RST_L  PIN will be output

	gpio_set_value(I2C_RST_L, 1); // Set to high

	usleep(500);

	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}

	// Get I2C device, LTC2991 I2C address is 0x6F
	ioctl(file, I2C_SLAVE, LTC4151_I2C_ADDRESS);

	for( i = 0 ; i < 3 ; i++ )
	{
		voltage_at_pin[i] = 0.0;
		value[i] = 0.0;
		raw[i] = 0.0;
	}

	//TEST LTC4151
	printf("\n LTC4151 Access TEST");
	printf("\n________________________\n");

	/*
	 *  CONTROL Register= 0x18
	 */
	data[0] = LTC4151_CONTREG;
	data[1] = 0x18;

	if ( write(file, data, 2) != 2)
	{
		printf("Fail writing to Status High Register\n");
		exit(1);
	}

	sleep(1);






	/*******************************************************************
		************     WRİTE 0 BYTE LTC2991_V1_MSB_REG      ****************
		********************************************************************
		********************************************************************/
		unsigned char addRegV1Msb[1];

		addRegV1Msb [0] = ( LTC4151_SENSE_H ) ;

		if(write(file,addRegV1Msb ,1) != 1 )
		{
			printf("Fail writing to REG V1MSB\n");
			exit(1);
		}



		if(read(file, data, 6) != 6)
		{
			printf("Error : Input/Output error \n");
			exit(1);
		}

		raw[0] = ( data[0] << 8 ) + data[1];
		raw[1] = ( data[2] << 8 ) + data[3];
		raw[2] = ( data[4] << 8 ) + data[5];

		voltage_at_pin[0] =(double) ( ( ( ( data[0] ) << 8 ) + data[1] ) >> 4 )* 20E-6  ;
		voltage_at_pin[1] =(double) ( ( ( ( data[2] ) << 8 ) + data[3] ) >> 4 )* 25E-3  ;
		voltage_at_pin[2] =(double) ( ( ( ( data[4] ) << 8 ) + data[5] ) >> 4 )* 500E-6  ;

		value[0] = voltage_at_pin[0] /0.01;
		value[1] = voltage_at_pin[1];
		value[2] = voltage_at_pin[2] *24.5;

		printf("\n \t Measured Current @1:");		PRINT_DOUBLE3F ((value[0] ) ) ; printf( " A 0x%x\n",  raw[0] ) ;
		printf("\t Measured Voltage @2:");		PRINT_DOUBLE3F ((value[1] ) ) ; printf( " V 0x%x\n",  raw[1] ) ;
		printf(" \t Measured Voltage @3:");		PRINT_DOUBLE3F ((value[2] ) ) ; printf( " V 0x%x\n",  raw[2] ) ;


}
int pauseNanoSec(long nano)
{
	struct timespec tmr1,tmr2;

	tmr1.tv_sec = 0;
	tmr1.tv_nsec = nano;

	if(nanosleep(&tmr1,&tmr2) < 0)
	{
		printf("Nano second pause failed\n");
		return -1;
	}
	return 1;
}
