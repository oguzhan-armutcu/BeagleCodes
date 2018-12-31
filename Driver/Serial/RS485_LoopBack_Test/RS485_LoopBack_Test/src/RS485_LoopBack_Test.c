/*
 * @file       : test_serial_2_4.c
 * @author     : Oguzhan Armutcu
 * @date       : 10 Apr 2018
 * Description : Loopback Test between UART_2 and UART4
 *
 **********************************************************/

#include <stdio.h>
#include <fcntl.h>          /* File Control Definitions           */
#include <termios.h>        /* POSIX Terminal Control Definitions */
#include <unistd.h>      	/* UNIX Standard Definitions          */
#include <errno.h>       	/* ERROR Number Definitions           */
#include <sys/ioctl.h>      /* ioctl()                            */
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include <linux/serial.h>
#include "test_serial_2_4.h"

int main(void) {

	int fd_Ch1;
	int fd_Ch2;
	int fd_Ch3;
	char bufCh1[30] = ch1_tty;
	char bufCh2[30] = ch2_tty;
	char bufCh3[30] = ch3_tty;

	fd_Ch1 = openPort(bufCh1);

	if (fd_Ch1 == 0) {
		printf("Channel 1 Port failed to open successfully. \n");
		exit(1);
	}

	fd_Ch2 = openPort(bufCh2);
	if (fd_Ch2 == 0) {
		printf("Channel 2 Port failed to open successfully. \n");
		exit(1);
	}
	fd_Ch3 = openPort(bufCh3);
	if (fd_Ch3 == 0) {
		printf("Channel 3 Port failed to open successfully. \n");
		exit(1);
	}

	sleep(2); //required to make flush work, for some reason
	tcflush(fd_Ch1,TCIOFLUSH);
	tcflush(fd_Ch2,TCIOFLUSH);
	tcflush(fd_Ch3,TCIOFLUSH);

	/*
	 *  Ch3 rts pin asserted low for receive mode
	 */
	setRTS(fd_Ch3,1);


	setAttributes(fd_Ch1, fd_Ch2);

	sendFromCh1ToCh2(fd_Ch1, fd_Ch2);
	sendFromCh2ToCh1(fd_Ch1, fd_Ch2);




	return 1;

}




int openPort(char *devicename)
{

	/*-------------------------- Opening the Serial Port -----------------------	*/
	int fd;
	/* Change "buf[]" to the one corresponding to your system 		*/

	fd = open(devicename, O_RDWR | O_NOCTTY);
	/* O_RDWR Read/Write access to serial port           		*/
	/* O_NOCTTY - No terminal will control the process   	*/

	if (fd == -1) /* Error Checking */
	{
		printf("\n  Error! in Opening port  ");
		return 0;
	}

	else {
		printf("\n +----------------------------------+");
		printf("\n  Port Opened Successfully ");
		return fd;
	}
}

void setAttributes ( int fd_Ch1, int fd_Ch2 )
{



	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	struct termios SerialPortSettings_Ch1; /* Create the structure	*/
	tcgetattr(fd_Ch1, &SerialPortSettings_Ch1); /* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings_Ch1, B3500000); /* Set Read  Speed as __MAX_BAUD                     */
	cfsetospeed(&SerialPortSettings_Ch1, B3500000); /* Set Write Speed as __MAX_BAUD                     */

	SerialPortSettings_Ch1.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings_Ch1.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings_Ch1.c_cflag &= ~CSIZE; /* Clears the mask for setting the data size             */
	SerialPortSettings_Ch1.c_cflag |= CS8; /* Set the data bits =8                                 */
	//SerialPortSettings_Ch1.c_cflag |= CRTSCTS;       /*  Hardware flow Control                         */
	SerialPortSettings_Ch1.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */
	SerialPortSettings_Ch1.c_iflag &= ~(IXON | IXOFF | IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings_Ch1.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */
	SerialPortSettings_Ch1.c_oflag &= ~OPOST;/*No Output Processing*/

	if ((tcsetattr(fd_Ch1, TCSANOW, &SerialPortSettings_Ch1)) != 0) /* Set the attributes to the termios structure*/
	{
		printf("\n  ERROR ! in Setting attributes CH1");
		exit(1);
	}

	else
		printf("\n Ch1 BaudRate = 4000000 \n  StopBits = 1 \n  Parity   = none");

	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	struct termios SerialPortSettings_Ch2; /* Create the structure	*/

	tcgetattr(fd_Ch2, &SerialPortSettings_Ch2); /* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings_Ch2, B3500000); /* Set Read  Speed as __MAX_BAUD                     */
	cfsetospeed(&SerialPortSettings_Ch2, B3500000); /* Set Write Speed as __MAX_BAUD                     */
	SerialPortSettings_Ch2.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings_Ch2.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings_Ch2.c_cflag &= ~CSIZE; /* Clears the mask for setting the data size             */
	SerialPortSettings_Ch2.c_cflag |= CS8; /* Set the data bits =8                                 */
	//SerialPortSettings_Ch2.c_cflag |= CRTSCTS;       /*  Hardware flow Control                         */
	SerialPortSettings_Ch2.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */
	SerialPortSettings_Ch2.c_iflag &= ~(IXON | IXOFF | IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings_Ch2.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */
	SerialPortSettings_Ch2.c_oflag &= ~OPOST;/*No Output Processing*/

	if ((tcsetattr(fd_Ch2, TCSANOW, &SerialPortSettings_Ch2)) != 0) /* Set the attributes to the termios structure*/
	{
		printf("\n  ERROR ! in Setting attributes CH2");
		exit(1);
	}

	else
		printf("\n Ch2 BaudRate = 4000000 \n  StopBits = 1 \n  Parity   = none");

}

void sendFromCh1ToCh2( int fd_Ch1, int fd_Ch2  )
{
	/*
	 *  Ch1 rts pin asserted high
	 */
	setRTS(fd_Ch1, 0);

	/*
	 *  Ch1 rts pin asserted low
	 */
	setRTS(fd_Ch2, 1);



	writeDatas(fd_Ch1);

	if (readAndCheckDatas(fd_Ch2) != 0) {

		printf("\n_____________________________________________________\n");
		printf("|              T E S T  S E R I A L                  |\n");
		printf("|        R S 4 8 5_ C H 0 -> R S 4 8 5  C H 1        |\n");
		printf("|            _T E S T   I S   F A I L E D            |\n");
		printf("|____________                             ___________|\n");
	}

	else {

		printf("\n_____________________________________________________\n");
		printf("|              T E S T  S E R I A L                  |\n");
		printf("|        R S 4 8 5_ C H 0 -> R S 4 8 5  C H 1        |\n");
		printf("|          T E S T   S U C C E S S F U L             |\n");
		printf("|____________                             ___________|\n");


	}

	usleep(500);

}
void sendFromCh2ToCh1( int fd_Ch1, int fd_Ch2  )
{
	/*
	 *  Ch1 rts pin asserted low
	 */
	setRTS(fd_Ch1, 1);

	/*
	 *  Ch2 rts pin asserted high
	 */
	setRTS(fd_Ch2, 0);

	usleep(500);
	writeDatas(fd_Ch2);

	if (readAndCheckDatas(fd_Ch1) != 0) {

		printf("\n_____________________________________________________\n");
		printf("|              T E S T  S E R I A L                  |\n");
		printf("|        R S 4 8 5_ C H 1 -> R S 4 8 5  C H 0        |\n");
		printf("|            _T E S T   I S   F A I L E D            |\n");
		printf("|____________                             ___________|\n");
	} else {

		printf("\n_____________________________________________________\n");
		printf("|              T E S T  S E R I A L                  |\n");
		printf("|        R S 4 8 5_ C H 1 -> R S 4 8 5  C H 0        |\n");
		printf("|          T E S T   S U C C E S S F U L             |\n");
		printf("|____________                             ___________|\n");
	}

}

/* For the two functions below:
 *     level=1 to set line to LOW
 *     level=0 to set line to HIGH
 */

int setRTS(int fd, unsigned short level)
{

	int status;

	if (ioctl(fd, TIOCMGET, &status) == -1) {
		perror("setRTS(): TIOCMGET");
		return 0;
	}
	if (level)
		status |= TIOCM_RTS;
	else
		status &= ~TIOCM_RTS;
	if (ioctl(fd, TIOCMSET, &status) == -1) {
		perror("setRTS(): TIOCMSET");
		return 0;
	}
	return 1;
}

void writeDatas(int fd)
{
	/*------------------------------- Write data to serial port -----------------------------*/
	int k;
	//int  j = 255;
	unsigned char write_buffer[message_size]; /* Buffer containing characters to write into port	     */
	for (k = 0; k < sizeof(write_buffer); k++) {
		if (k % 2)
			write_buffer[k] = 0x55;
		else
			write_buffer[k] = 0xAA;
		//	j--;
	}
	int bytes_written = 0; /* Value for storing the number of bytes written to the port */

	int lf[1] = { 0xA };
	//int intrup[1] = {0x1D};

	//write(fd,intrup,1);
	bytes_written = write(fd, write_buffer, sizeof(write_buffer));
	write(fd, lf, 1);

	if (bytes_written != sizeof(write_buffer)) {
		printf("Writing is not successful.\n");
		exit(1);
	}

	/* use write() to send data to port                                            */
	/* "fd"                   - file descriptor pointing to the opened serial port */
	/*	"write_buffer"         - address of the buffer containing data	            */
	/* "sizeof(write_buffer)" - No of bytes to write                               */

	//printf("\n  %s written ",write_buffer);
	printf("\n  %d Bytes written", bytes_written); //check
	printf("\n +----------------------------------+\n\n");
}

int readAndCheckDatas( int fd )
{
	/*------------------------------- Read data from serial port -----------------------------*/

	int errorCount = 0;
//		tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

//	    setRTS(1);

	unsigned char read_buffer[message_size]; /* Buffer to store the data received              */
	int bytes_read = 0; /* Number of bytes read by the read() system call */
	int i = 0;

	bytes_read = read(fd, &read_buffer, sizeof(read_buffer)); /* Read the data                   */

	printf("\n\n  Bytes Rxed %d", bytes_read); /* Print the number of bytes read */
	printf("\n\n  ");

	for (i = 0; i < bytes_read; i++) /*printing only the received characters*/
	{
		printf("[%d]: 0x%x\t", i, read_buffer[i]);
		if (i % 6 == 5)
			printf("\n");
		if (i % 2 == 0) {
			if (read_buffer[i] != 0xaa)
				errorCount++;
		} else {
			if (read_buffer[i] != 0x55)
				errorCount++;
		}
	}

	if (errorCount > 0)
		return 1;

	return 0;
	printf("\n +----------------------------------+\n\n\n");
}

