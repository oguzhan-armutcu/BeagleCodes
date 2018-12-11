/**********************************************************
 * (c) ARTRON 2016, All Rights Reserved
 * No part of this software source code can be changed,
 * copied and used without the written consent of ARTRON.
 *
 * @file       : m24512.c
 * @author     : Oguzhan Armutcu
 * @date       : 14 Jan 2018
 * Description : M24512 EEPROM , "I2C-1" read identification page
 *
 * 00h -> ST manufacturer code --> 20h
 * 01h -> I2C family code --> E0h
 * 02h -> Memory density  --> 10h
 *
 **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define M24512_ADDRESS_MEMORY		0x50
#define M24512_ADDRESS_IDENTIFY		0x58

void main() {
	// Create I2C bus
	int file, file_idf;

	unsigned data = 0x00;

	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) {
		printf("Failed to open the bus. \n");
		exit(1);
	}

	// Get I2C device, M24512 I2C identification address is 0x58)
	ioctl(file, I2C_SLAVE, M24512_ADDRESS_IDENTIFY);

	// Select write address(0x0001)
	// Write the data(0x50)
	char config[2] = { 0x00 };

	sleep(1);

	// Read 1 byte of data from register(0x0000)
	// input data
	write(file, config, 2);

	if (read(file, &data, 1) != 1) {
		printf("Error : Input/output Error \n");
		exit(1);
	} else {
		// Output data to screen
		printf("ST manufacturer code : 0x%x \n", data);
	}

	config[1] = 0x01;

	sleep(1);

	// Read 1 byte of data from register(0x0001)
	// input data
	write(file, config, 2);

	data = 0x00;
	if (read(file, &data, 1) != 1) {
		printf("Error : Input/output Error \n");
		exit(1);
	} else {
		// Output data to screen
		printf("I2C family code: 0x%x \n", data);
	}

	config[1] = 0x02;

	sleep(1);

	// Read 1 byte of data from register(0x0002)
	// input data
	write(file, config, 2);
	data = 0x00;
	if (read(file, &data, 1) != 1) {
		printf("Error : Input/output Error \n");
		exit(1);
	} else {
		// Output data to screen
		printf("Memory density code: 0x%x \n", data);
	}

}
