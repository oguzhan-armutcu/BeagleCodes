/*
 * @file        : tmp100.c
 * @author      : Oguzhan Armutcu
 * @date        : 12 Jan 2018
 * Description  : TMP100 temperature sensor in C, "I2C-1"
 *********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define TMP100_ADDRESS 0x4A

void main()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TMP100 I2C address is 0x4F(79)
	ioctl(file, I2C_SLAVE, TMP100_ADDRESS);

	//TEST TMP100
	printf("\n\n TMP100 TEST");
	printf("\n__________________________\n");

	// Select configuration register(0x01)
	// Continuous conversion, comparator mode, 12-bit resolution(0x60)
	char config[2] = {0};
	config[0] = 0x01;
	config[1] = 0x60;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x00)
	// temp msb, temp lsb
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
		printf("__________________________\n");
	}
	else
	{
		// Convert the data to 12-bits
		int temp = (data[0] * 256 + (data[1] & 0xF0)) / 16;
		if(temp > 2047)
		{
			temp -= 4096;
		}
		float cTemp = temp * 0.0625;
		float fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		printf("Temperature in Celsius is : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit is : %.2f F \n", fTemp);
	}
}
