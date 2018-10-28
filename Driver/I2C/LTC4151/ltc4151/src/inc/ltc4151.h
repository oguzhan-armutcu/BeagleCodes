/**********************************************************
 *
 * @file       : ltc4151.h
 * @author     : Oguzhan Armutcu
 * @date       : 20 Aug 2016
 * Description : This  file is for driver definitions
 * for i2c interface.
 *
 *********************************************************/

#ifndef LTC4151_H_
#define LTC4151_H_

#define LTC4151_I2C_ADDRESS   0x6f
#define I2C_RST_L                76 //gpio No
/* chip registers */
#define LTC4151_SENSE_H	0x00
#define LTC4151_SENSE_L	0x01
#define LTC4151_VIN_H	0x02
#define LTC4151_VIN_L	0x03
#define LTC4151_ADIN_H	0x04
#define LTC4151_ADIN_L	0x05
#define LTC4151_CONTREG	 0x06
/************************** Function Prototypes ****************************/
int pauseNanoSec(long nano);


#endif /* LTC4151_H_ */
