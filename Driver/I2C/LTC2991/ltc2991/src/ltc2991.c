/**********************************************************
 * @file       : ltc2991.c
 * @author     : Oguzhan Armutcu
 * @date       : 13 Jan 2018
 * Description : ADC LTC2991 -I2C1
 *
 **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "inc/ltc2991.h"
#include "inc/gpio-utils.h"



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
	double voltage_at_pin [10];
	double value[10];
	volatile unsigned short raw[10];
	unsigned char data[2] = {0};

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

	// Get I2C device, LTC2991 I2C address is 0x4C
	ioctl(file, I2C_SLAVE, LTC2991_I2C_ADDRESS);

	for( i = 0 ; i < 10 ; i++ )
	{
		voltage_at_pin[i] = 0.0;
		value[i] = 0.0;
		raw[i] = 0.0;
	}

	//TEST LTC2991
	printf("\n LTC2991 Access TEST");
	printf("\n________________________\n");


	/*
	 *  LTC2991_CONTROL_V1234_REG = 0x11
	 */
	data[0] = LTC2991_CONTROL_V1234_REG;
	data[1] = 0x11;
	//	data[1] = (LTC2991_V3_V4_DIFFERENTIAL_ENABLE || LTC2991_V1_V2_DIFFERENTIAL_ENABLE  );
	if ( write(file, data, 2) != 2)
	{
		printf("Fail writing to Status High Register\n");
		exit(1);
	}

	sleep(1);

	/*
	 * LTC2991_CONTROL_V5678_REG = 0x01
	 */
	data[0] =LTC2991_CONTROL_V5678_REG;
	data[1] = LTC2991_V5_V6_DIFFERENTIAL_ENABLE;
	if ( write(file, data, 2) != 2)
	{
		printf("Fail writing to Status High Register\n");
		exit(1);
	}

	sleep(1);

	/*******************************************************************
	************    ENABLE ALL CHANNELS      *********** ****************
	********************************************************************
	********************************************************************/
	data[0] = LTC2991_STATUS_HIGH_REG;
	data[1] = LTC2991_ENABLE_ALL_CHANNELS;
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
	for( i =0 ; i <10 ; i++  )
	{
		addRegV1Msb [0] = ( LTC2991_V1_MSB_REG + ( i*2 ) ) ;
		double sign = -1.0;
		if(write(file,addRegV1Msb ,1) != 1 )
		{
			printf("Fail writing to REG V1MSB\n");
			exit(1);
		}


		if(read(file, data, 2) != 2)
		{
			printf("Error : Input/Output error \n");
			exit(1);
		}
		else
		{
			pauseNanoSec(10000);
			raw[i] = ( data[0] << 8 ) + data[1];
		}

		if( i == 0 ||  i == 2 || i==4 || i>5)
		{
			if( ( data[0] & 0x40) == 0x40 )
			{
				sign = -1.0;
				voltage_at_pin[i] = ( ( double )  (0x3FFF - ( ( ( data[0] & 0x3F ) << 8 ) + data[1] ) ) * LTC2991_SINGLE_ENDED_lsb ) * sign;
			}
			else
			{
				sign = 1.0;
				voltage_at_pin[i] =(double) ( ( ( data[0] & 0x3F) << 8 ) + data[1] ) * LTC2991_SINGLE_ENDED_lsb * sign ;
			}
		}
		else
		{
			if( ( data[0] & 0x40) == 0x40 )
			{
				sign = -1.0;
				voltage_at_pin[i] = ( ( double )  (0x3FFF - ( ( ( data[0] & 0x3F ) << 8 ) + data[1] ) ) * LTC2991_DIFFERENTIAL_lsb ) * sign;
			}
			else
			{
				sign = 1.0;
				voltage_at_pin[i] =(double) ( ( ( data[0] & 0x3F) << 8 ) + data[1] ) * LTC2991_DIFFERENTIAL_lsb * sign ;
			}
		}

	}



	value[0] = voltage_at_pin[0] ;
	value[1] = voltage_at_pin[1] / (0.1);
	value[2] = voltage_at_pin[2] *1.67;
	value[3] = voltage_at_pin[3]/ (0.1)*1.67;
	value[4] = voltage_at_pin[4]*4.19;
	value[5] = voltage_at_pin[5]/ (0.1)*4.19;
	value[6] = voltage_at_pin[6]  * 1.67;
	value[7] = voltage_at_pin[7] *4;
	value[8] = (voltage_at_pin[8] * 0.0625) / (LTC2991_SINGLE_ENDED_lsb);
	value[9] = voltage_at_pin[9]  + 2.5;

	printf("\n \t Measured Voltage @:");		PRINT_DOUBLE3F ((value[0] ) ) ; printf( " V 0x%x\n",  raw[0] ) ;
	printf("\t Measured Current  @:");		PRINT_DOUBLE3F ((value[1] ) ) ; printf( " A 0x%x\n",  raw[1] ) ;
	printf(" \t Measured Voltage @");		PRINT_DOUBLE3F ((value[2] ) ) ; printf( " V 0x%x\n",  raw[2] ) ;
	printf(" \t Measured Current @");		PRINT_DOUBLE3F ((value[3] ) ) ; printf( " A 0x%x\n",  raw[3] ) ;
	printf(" \t Measured Voltage @");		PRINT_DOUBLE3F ((value[4] ) ) ; printf( " V 0x%x\n",  raw[4] ) ;
	printf(" \t Measured Current @");		PRINT_DOUBLE3F ((value[5] ) ) ; printf( " A 0x%x\n",  raw[5] ) ;
	printf(" \t Measured Voltage @");		PRINT_DOUBLE3F ((value[6] ) ) ; printf( " V 0x%x\n",  raw[6] ) ;
	printf(" \t Measured Voltage @");		PRINT_DOUBLE3F ((value[7] ) ) ; printf( " V 0x%x\n",  raw[7] ) ;

	printf(" \t Measured Voltage @:");		PRINT_DOUBLE3F ((value[9] ) ) ; printf( " V 0x%x\n",  raw[9] ) ;

	printf("Board Temp. :");       PRINT_DOUBLE3F((  value[8] ));  printf(" Celsius 0x%x\n", raw[8]);


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
