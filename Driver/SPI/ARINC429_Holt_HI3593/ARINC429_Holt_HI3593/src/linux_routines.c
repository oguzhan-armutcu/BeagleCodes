/*
 * linux_routines.c
 *
 */

#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*******************************************************************************
 * Name:    kbhit
 *
 * Description:
 *
 *      Checks for keyboard input.
 *
 * In   none
 * Out  returns     1=key pressed, 0=no key press
 ******************************************************************************/

struct termios orig_termios;

void reset_terminal_mode(void)
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode(void)
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit(void)
{
#if 1
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
#else
	struct timeval  tv;
	fd_set      read_fd;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&read_fd);
	FD_SET(0, &read_fd);
	usleep(1);
	if( select(1, &read_fd, NULL, NULL, &tv) == -1 )
	{
		return 0;
	}

	if( FD_ISSET(0, &read_fd) )
	{
		return 1;
	}

	return 0;
#endif
}

int get_ch_blocking(void)
{
#if 1
	int value;
	while (!kbhit());

	value = fgetc(stdin);

	while((value == '\n') || (value == '\r'))
	{
		value = fgetc(stdin);
	}

	//printf("get_ch_blocking got(%c)\n", value);
	return value;
#else
	int ret;

	ret = getchar();

	/* flush input buffer */
	while (kbhit())
	{
		getchar();
	}

	return ret;
#endif
}
