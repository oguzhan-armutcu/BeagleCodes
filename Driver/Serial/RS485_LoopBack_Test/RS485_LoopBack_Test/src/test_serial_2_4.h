/**********************************************************
 * @file       : test_serial_2_4.h
 * @author     : Oguzhan Armutcu
 * @date       : 23 Feb 2018
 * Description : The Header file is for Loopback Test between
 * UART_2 and UART4
 *
 **********************************************************/

#ifndef TEST_SERIAL_2_4_H_
#define TEST_SERIAL_2_4_H_

/*
 * DEFINES
 */
/* Two globals for use by this module only */
#define ch1_tty           "/dev/ttyS2"
#define ch2_tty           "/dev/ttyS4"
#define ch3_tty           "/dev/ttyS5"
#define message_size 256


/*
 *  FUNCTION PROTOTYPES
 */
int openPort ( char *devicename );
int setRTS ( int fd, unsigned short level );
void setAttributes ( int fd_Ch1, int fd_Ch2 );
void sendFromCh1ToCh2( int fd_Ch1, int fd_Ch2);
void sendFromCh2ToCh1( int fd_Ch1, int fd_Ch2);
void writeDatas(int fd);
int readAndCheckDatas( int fd );

#endif /* TEST_SERIAL_2_4_H_ */
