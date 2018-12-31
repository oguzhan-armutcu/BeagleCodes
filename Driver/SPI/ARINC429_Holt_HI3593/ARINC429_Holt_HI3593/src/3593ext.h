/*
 * 3593ext.h
 *
 *  Created on: May 8, 2018
 *      Author: oguzhanarmutcu
 */

//#ifndef 3593EXT_H_
//#define 3593EXT_H_

extern int ExDev1, ExDev2, ExDev3;

void EXT_3593_initAllGpios ( void );

void EXT_3593_MasterReset( void );

int EXT_3593_ReadRT1 (void);

int EXT_3593_ReadRT2(void);

//#endif /* 3593EXT_H_ */
