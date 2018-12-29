/*
 * ds1682.h
 *
 *  Created on: Dec 29, 2018
 *      Author: oguzhanarmutcu
 */

#ifndef DS1682_H_
#define DS1682_H_

/*
 *  DEFINES
 */
#define DS1682_ADDRESS 0x6B

/*
 * DEVICE REGISTER
*/
#define DS1682_REG_CONFIG              0x00
#define DS1682_REG_ALARM               0x01
#define DS1682_REG_ELAPSED             0x05
#define DS1682_REG_EVT_CNTR            0x09
#define DS1682_REG_EEPROM              0x0b
#define DS1682_REG_RESET               0x1d
#define DS1682_REG_WRITE_DISABLE       0x1e
#define DS1682_REG_WRITE_MEM_DISABLE   0x1f

#define DS1682_EEPROM_SIZE             10


/*
 * FUNCTION PROTOTYPES
 */
int pauseNanoSec(long nano);


#endif /* DS1682_H_ */
