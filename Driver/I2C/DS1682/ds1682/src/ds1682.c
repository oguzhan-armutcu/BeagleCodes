/**********************************************************
 * @file       : ds1682.c
 * @author     : Oguzhan Armutcu
 * @date       : 12 Jan 2018
 * Description : DS1682 -İ2C1
 *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ds1682.h"


int main ( int argc, char * const argv[ ] )
{
	int i;
	unsigned etc;	/*  	Elapsed Time Counter	  	*/
	unsigned ec;		/*		Event Counter					*/
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TMP100 I2C address is 0x6B
	ioctl(file, I2C_SLAVE, DS1682_ADDRESS);

	//TEST DS1682
	printf("\n DS1682 TEST");
	printf("\n__________________________\n");


	/*******************************************************************
	*******************READ 1 BYTE REG_CONFIG ************************
	********************************************************************/
	unsigned char dataConfigRegister[1];
    dataConfigRegister[0]= DS1682_REG_CONFIG;

	if( write(file,dataConfigRegister,1) != 1)
	{
		printf("Fail writing to REG_CONFIG\n");
		exit(1);
	}
	sleep(1);
	if ( read(file,dataConfigRegister,1) != 1)
	{
		printf("Error : Input/Output error \n");
		printf("Fail writing to REG_CONFIG\n");
		exit(1);
	}
	else
		printf("DS1682 DS1682_REG_CONFIG : 0x%x \n",dataConfigRegister[0]);



	/*******************************************************************
	************READ 4 BYTE DS1682_REG_ELAPSED  *********************
	********************************************************************/
	unsigned char dataRegElapsed[4] = {0};
	unsigned addRegElapsed[1] = {DS1682_REG_ELAPSED};

	if( write(file,addRegElapsed,1) != 1 )
	{
		printf("Fail writing to REG_ELAPSED\n");
		exit(1);
	}
	sleep(1);
	if ( read(file,dataRegElapsed,4) != 4) 		// Donmesi gereken return yanlis olabilir. Debug yaparken Kontrol et
	{
		printf("Error : Input/Output error \n");
		exit(1);
	}
	else
	{
		for(i=0;i<4;i++)
		{
			printf("Elapsed data[%d]: 0x%x\n",i,dataRegElapsed[i]);
		}
		etc = ( dataRegElapsed[3] << 24 ) + ( dataRegElapsed[2]  << 16 ) + ( dataRegElapsed[1] << 8 ) + dataRegElapsed[0] ;
		printf( " Elapsed Time : 0x%x \n" , etc );

	}


	pauseNanoSec(250000);								//Delay suresi hatali olabilir. DS1682 gelince Kontrol et.



//	/********************************************************************
//	********READ 4 BYTE DS1682_REG_ELAPSED AFTER250 msec    **********
//	*********************************************************************/

	write(file,addRegElapsed,1);
	sleep(1);
	if ( read(file,dataRegElapsed,4) != 4) 		// Donmesi gereken return yanlis olabilir. Debug yaparken Kontrol et
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		etc = ( dataRegElapsed[3] << 24 ) + ( dataRegElapsed[2]  << 16 ) + ( dataRegElapsed[1] << 8 ) + dataRegElapsed[0] ;
		printf( " Elapsed Time : 0x%x \n" , etc );
	}


//	/*******************************************************************
//	**********READ 2 BYTE DS1682_REG_EVT_CNTR  **********************
//	********************************************************************/

	unsigned char dataRegEvt[2] = {0};
	unsigned addRegEvt [1] = {DS1682_REG_EVT_CNTR};

	if ( write(file,addRegEvt,1) != 1)
	{
		printf("Fail writing to REG_EVT\n");
		exit(1);
	}

	sleep(1);

	if ( read(file,dataRegEvt,2) != 2)
	{
		printf("Error : Input/Output error \n");
		exit(1);
	}
	else
	{
		ec = ( dataRegEvt[1] << 8 ) + dataRegEvt[0] ;
		printf( " DS1682 DS1682_REG_EVT_CNTR : 0x%x \n" , ec );
	}




//	/*******************************************************************
//	*******READ DS1682_EEPROM_SIZE BYTE DS1682_REG_EEPROM  ***
//	********************************************************************/
	unsigned char dataRegEeprom[10] = {0};
	unsigned addRegEeprom[1] = {DS1682_REG_EEPROM};
	if ( write(file,addRegEeprom,1) != 1)
	{
		printf("Fail writing to REG_EEPROM\n");
		exit(1);
	}
	sleep(1);
	if ( read(file,dataRegEeprom,DS1682_EEPROM_SIZE) != 10)
	{
		printf("Error : Input/Output error \n");
		printf("__________________________\n");
	}
	else
	{
		for ( i=0 ; i < DS1682_EEPROM_SIZE; i++ )
		{
			printf(" Val[%d] : 0x%x\n", i , dataRegEeprom[i] );
		}
	}
	return 1;
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
