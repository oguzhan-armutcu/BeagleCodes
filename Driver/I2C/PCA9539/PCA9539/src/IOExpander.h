/*
 * IOExpander.h
 *
 *  Created on: Nov 16, 2017
 *      Author: root
 */

#ifndef IOEXPANDER_H_
#define IOEXPANDER_H_

#define I2C_RST_L                76 //gpio No
#define EXP1ADDR              0x74
#define EXP2ADDR              0x75
#define EXP3ADDR              0x76


#define EXP1CONFIG         	(Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2 | Config_Input_P0_3 | Config_Input_P0_4 |                     \
                                                    Config_Input_P0_5 | Config_Output_P0_6 | Config_Output_P0_7 | Config_Output_P1_0 | Config_Output_P1_1 |          \
                                                    Config_Output_P1_2 |	Config_Output_P1_3 | Config_Output_P1_4 | Config_Output_P1_5 | Config_Output_P1_6 |    \
                                                    Config_Output_P1_7 )

#define EXP2CONFIG         	(Config_Input_P0_0 | Config_Input_P0_1 | Config_Input_P0_2 | Config_Input_P0_3 | Config_Input_P0_4 |                     \
                                                    Config_Input_P0_5 | Config_Output_P0_6 | Config_Output_P0_7 | Config_Input_P1_0 | Config_Input_P1_1 |                      \
                                                    Config_Input_P1_2 | Config_Input_P1_3 | Config_Input_P1_4 | Config_Input_P1_5 | Config_Output_P1_6 	|                     \
                                                    Config_Output_P1_7 )

#define EXP3CONFIG         	(Config_Output_P0_0 | Config_Output_P0_1 | Config_Input_P0_2 |  Config_Input_P0_3 | Config_Input_P0_4 |                                    \
                                                     Config_Input_P0_5 |   Config_Input_P0_6 |Config_Input_P0_7 | Config_Input_P1_0 | Config_Input_P1_1 | Config_Input_P1_2    |   \
                                                    Config_Input_P1_3 | Config_Input_P1_4 | Config_Input_P1_5 | Config_Output_P1_6 | Config_Output_P1_7 )



enum pca9539_cmd
{
	PCA9539_INPUT_0		= 0,
	PCA9539_INPUT_1		= 1,
	PCA9539_OUTPUT_0	= 2,
	PCA9539_OUTPUT_1	= 3,
	PCA9539_INVERT_0	= 4,
	PCA9539_INVERT_1	= 5,
	PCA9539_DIRECTION_0	= 6,
	PCA9539_DIRECTION_1	= 7,
};


#define Config_Input_P0_0			(1<<0)
#define Config_Input_P0_1			(1<<1)
#define Config_Input_P0_2			(1<<2)
#define Config_Input_P0_3			(1<<3)
#define Config_Input_P0_4			(1<<4)
#define Config_Input_P0_5			(1<<5)
#define Config_Input_P0_6			(1<<6)
#define Config_Input_P0_7			(1<<7)
#define Config_Input_P1_0			(1<<8)
#define Config_Input_P1_1			(1<<9)
#define Config_Input_P1_2			(1<<10)
#define Config_Input_P1_3			(1<<11)
#define Config_Input_P1_4			(1<<12)
#define Config_Input_P1_5			(1<<13)
#define Config_Input_P1_6			(1<<14)
#define Config_Input_P1_7			(1<<15)


#define Config_Output_P0_0			(0<<0)
#define Config_Output_P0_1			(0<<1)
#define Config_Output_P0_2			(0<<2)
#define Config_Output_P0_3			(0<<3)
#define Config_Output_P0_4			(0<<4)
#define Config_Output_P0_5			(0<<5)
#define Config_Output_P0_6			(0<<6)
#define Config_Output_P0_7			(0<<7)
#define Config_Output_P1_0			(0<<8)
#define Config_Output_P1_1			(0<<9)
#define Config_Output_P1_2			(0<<10)
#define Config_Output_P1_3			(0<<11)
#define Config_Output_P1_4			(0<<12)
#define Config_Output_P1_5			(0<<13)
#define Config_Output_P1_6			(0<<14)
#define Config_Output_P1_7			(0<<15)

#define Output_High_P0_0			(1<<0)
#define Output_High_P0_1			(1<<1)
#define Output_High_P0_2			(1<<2)
#define Output_High_P0_3			(1<<3)
#define Output_High_P0_4			(1<<4)
#define Output_High_P0_5			(1<<5)
#define Output_High_P0_6			(1<<6)
#define Output_High_P0_7			(1<<7)
#define Output_High_P1_0			(1<<8)
#define Output_High_P1_1			(1<<9)
#define Output_High_P1_2			(1<<10)
#define Output_High_P1_3			(1<<11)
#define Output_High_P1_4			(1<<12)
#define Output_High_P1_5			(1<<13)
#define Output_High_P1_6			(1<<14)
#define Output_High_P1_7			(1<<15)

#define Output_Low_P0_0			(0<<0)
#define Output_Low_P0_1			(0<<1)
#define Output_Low_P0_2			(0<<2)
#define Output_Low_P0_3			(0<<3)
#define Output_Low_P0_4			(0<<4)
#define Output_Low_P0_5			(0<<5)
#define Output_Low_P0_6			(0<<6)
#define Output_Low_P0_7			(0<<7)
#define Output_Low_P1_0			(0<<8)
#define Output_Low_P1_1			(0<<9)
#define Output_Low_P1_2			(0<<10)
#define Output_Low_P1_3			(0<<11)
#define Output_Low_P1_4			(0<<12)
#define Output_Low_P1_5			(0<<13)
#define Output_Low_P1_6			(0<<14)
#define Output_Low_P1_7			(0<<15)





int initI2C( unsigned char address);
int IOEXP_Write_config_reg ( int device, unsigned short buf );
int IOEXP_Write_output_reg ( int device, unsigned short buf );
int IOEXP_Read_input_reg ( int device, unsigned char *buf);


#endif /* IOEXPANDER_H_ */
