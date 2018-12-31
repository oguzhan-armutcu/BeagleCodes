/**********************************************************
 * @file       : RS485_HalfDuplex_Test.c
 * @author     : Oguzhan Armutcu
 * @date       : 28 Nov 2018
 * Description : UART_2 RS485 Test.
 *
 **********************************************************/

#include <stdio.h>
#include <fcntl.h>        /* File Control Definitions           */
#include <termios.h>      /* POSIX Terminal Control Definitions */
#include <unistd.h>       /* UNIX Standard Definitions          */
#include <errno.h>        /* ERROR Number Definitions           */
#include <sys/ioctl.h>    /* ioctl()   							*/
#include <string.h>
#include <sys/types.h>
#include <stddef.h>
#include "gpio-utils.h"
#define UART2_HF                116 //gpio No
#define message_size            256
/* Two globals for use by this module only */
static int fd;

int setRTS(unsigned short level);
int main(int argc, char *argv[]) {

	//Using sysfs we need to write the gpio number to /sys/class/gpio/export
	gpio_export(UART2_HF);

	//SET DIRECTION
	gpio_set_dir(UART2_HF, "out"); 	// UART2_HF  PIN will be output

	gpio_set_value(UART2_HF, 1); // Set to high

	usleep(500);

	char buf[30] = "/dev/ttyO2";

	printf("\n +----------------------------------+");
	printf("\n |      RS485 Half Duplex           |");
	printf("\n +----------------------------------+");
	/*-------------------------- Opening the Serial Port -----------------------	*/

	/* Change "buf[]" to the one corresponding to your system 		*/

	fd = open(buf, O_RDWR | O_NOCTTY);
	/* O_RDWR Read/Write access to serial port           		*/
	/* O_NOCTTY - No terminal will control the process   	*/

	if (fd == -1) /* Error Checking */
		printf("\n  Error! in Opening port  ");
	else {
		printf("\n +----------------------------------+");
		printf("\n  Port Opened Successfully ");
	}

	/*---------- Setting the Attributes of the serial port using termios structure --------- */
	struct termios SerialPortSettings; /* Create the structure	*/

	tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

	cfsetispeed(&SerialPortSettings, B115200); /* Set Read  Speed as 115200                     */
	cfsetospeed(&SerialPortSettings, B115200); /* Set Write Speed as 115200                     */
	SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE; /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |= CS8; /* Set the data bits =8                                 */
	SerialPortSettings.c_cflag |= CRTSCTS; /*  Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */
	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

	if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
		printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none");

//		int RTS_flag,DTR_flag;
//
//		RTS_flag = TIOCM_RTS;	/* Modem Constant for RTS pin */
//		DTR_flag = TIOCM_DTR;	/* Modem Constant for DTR pin */
//
//		ioctl(fd,TIOCMBIC,&RTS_flag);/* ~RTS = 1,So ~RE pin of MAX485 is HIGH                       */
//		ioctl(fd,TIOCMBIC,&DTR_flag);/* ~DTR = 1,So  DE pin of MAX485 is HIGH,Transmit Mode enabled */

	setRTS(0);

	int k;

	/*------------------------------- Write data to serial port -----------------------------*/

	unsigned char write_buffer[message_size]; /* Buffer containing characters to write into port	     */
	for (k = 0; k < sizeof(write_buffer); k++) {
		if(k%2)
		{
			write_buffer [k] = 0xAA;
		}
		else
		{
			write_buffer [k] = 0x55;
		}
	}
	int bytes_written = 0; /* Value for storing the number of bytes written to the port */

	bytes_written = write(fd, write_buffer, sizeof(write_buffer));

	/* use write() to send data to port                                            */
	/* "fd"                   - file descriptor pointing to the opened serial port */
	/*	"write_buffer"         - address of the buffer containing data	            */
	/* "sizeof(write_buffer)" - No of bytes to write                               */

	//printf("\n  %s written to %s",write_buffer,buf);
	printf("\n  %d Bytes written to %s", bytes_written, buf); //check
	printf("\n +----------------------------------+\n\n");
	printf("Press 'enter' key to read data\n");
	getchar();
	/*------------------------------- Read data from serial port -----------------------------*/

	tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */

	setRTS(1);

	unsigned char read_buffer[message_size]; /* Buffer to store the data received              */
	int bytes_read = 0; /* Number of bytes read by the read() system call */
	int i = 0;

	//bytes_read = read(fd,&read_buffer,sizeof(read_buffer)); /* Read the data                   */

	bytes_read = read(fd, &read_buffer, message_size); /* Read the data                   */
	printf("\n\n  Bytes Rxed %d", bytes_read); /* Print the number of bytes read */
	printf("\n\n  ");

	for (i = 0; i < bytes_read; i++) /*printing only the received characters*/
	{

		printf("[%d] = %d\t", i, read_buffer[i]);
		if (i % 6 == 5)
			printf("\n");
	}

	printf("\n +----------------------------------+\n\n\n");
	close(fd);/* Close the Serial port */

}

/* For the two functions below:
 *     level=0 to set line to LOW
 *     level=1 to set line to HIGH
 */

int setRTS(unsigned short level) {
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
