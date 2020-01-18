/* Software Serial library for serial device[s] reading and writing
 *
 * Created by bross@unomaha.edu
 *
 * Free for Redistribution and Modification
 *
 */
#ifndef SSERIAL
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define _POSIX_SOURCE 1
#define MAX_BUFFER_SIZE 255					/* max size of message buffer */
#define FALSE 0								/* boolean value false */
#define TRUE 1								/* boolean value true */
#define CTRL_Z '\x1A'						/* CTRL-Z control character */
#define S_WRITE_CMD 'w'						/* mode to write command to serial port */
#define S_READ 'r'							/* mode to read from serial port */
#define S_WR '+'							/* mode to write then read from serial port */
#define SERVER_IP "ec2-52-34-250-13.us-west-2.compute.amazonaws.com"			/* IP address of server (changes with AWS */
#define SERVER_PORT 52321					/* port UDP C server side code uses */

#ifndef SERIAL_DEV_1
	#define SERIAL_DEV_1 "/dev/ttyAMA0"		/* This points to the FONA 808 GPS/GSM unit */
#endif
#ifndef SERIAL_DEV_2
	#define SERIAL_DEV_2 "/dev/ttyUSB0" 	/* This points to the Adafruit Ultimate GPS breakout board (over USB serial port) */
#endif
#ifndef SD1_BAUDRATE
	#define SD1_BAUDRATE B115200			/* Baud rate for FONA 808 */
#endif
#ifndef SD2_BAUDRATE
	#define SD2_BAUDRATE B9600				/* Baud rate for GPS breakout */
#endif

/* GSM COMMANDS UDP */
#define SET_APN "AT+CSTT=\"CMNET\"\n"
#define CHECK_WIRELESS "AT+CIICR\n"
#define GET_LOCAL_IP "AT+CIFSR\n"
#define START_UDP "AT+CIPSTART=\"UDP\",\"52.34.250.13\",\"52321\"\n"
#define SEND_UDP_MSG "AT+CIPSEND\n"
#define CLOSE_UDP "AT+CIPCLOSE\n"

/*******************************************\\\\\\\\ MACROS ////////*******************************************
 **************************************************************************************************************
 ***************************************** Let's make smaller byte code! **************************************
 **************************************************************************************************************
 **************************************************************************************************************
 **************************************************************************************************************/
/**************************************************************************************************************
 * XSTR_TO_BYTE: converts a string of characters to a unsigned byte                                           *
 * @param xstring: String of characters                                                                       *
 * @param position: position in the String to convert that character to a unsigned byte (size 8)              *
 * return: returns byte of the converted character in string at position                                      *
 **************************************************************************************************************/
 /* may change to reading characters and remove this */
#define XSTR_TO_BYTE(xstring, position) (uint8_t) xstring[position]

/**************************************************************************************************************
 * Runs init_sserial based on device, baud, command, and mode                                                 *
 * @param fd: valid file descriptor                                                                           *
 * @param dev: serial device to open to perform read/write operations                                         *
 * @param baud: baud rate to use to open serial device                                                        *
 * @param cmd: command to execute                                                                             *
 * @param mode: 'r' equals read, 'w' equals write '+' equals write then read                                  *
 * return: fd: did the serial port initialize correctly                                                       *
 **************************************************************************************************************/
#define RUN_CMD(fd, dev, baud, cmd, mode) do{ \
	if((fd = init_sserial(dev, baud, cmd, mode)) < 0){ perror("ERROR: Couldn't initialize serial port!"); return -1; } \
	close(fd); \
}while(0)

/**************************************************************************************************************
 * initializes GSM UDP client via serial port based on file descriptor, device, and baud                      *
 * @param fd: valid file descriptor                                                                           *
 * @param dev: serial device to open to perform read/write operations                                         *
 * @param baud: baud rate to use to open serial device                                                        *
 * Starts GSM data connection to server via FONA 808 2G GPRS, error checks, and closes file descriptor        *
 **************************************************************************************************************/
#define INIT_GSM_UDP(fd, dev, baud) do{ \
	if((fd = init_gsm_udp(fd, dev, baud)) < 0){ perror("ERROR: Couldn't initialize GSM connection!"); return -1; } \
	close(fd); \
}while(0)

/**************************************************************************************************************
 * sends message[MAX_BUFFER_SIZE] global variable over UDP                                                    *
 * @param fd: valid file descriptor                                                                           *
 * @param dev: serial device to open to perform read/write operations                                         *
 * @param baud: baud rate to use to open serial device                                                        *
 * Sends message, error checks, and closes file descriptor                                                    *
 **************************************************************************************************************/
#define SEND_MSG_UDP(fd, dev, baud) do{ \
	if((fd = send_string_udp(fd, dev, baud)) < 0){ perror("ERROR:Couldn't send message via UDP!"); return -1; } \
	close(fd); \
}while(0)


void signal_handler_IO(int status);
int init_sserial(char *device, int baud, char *command, char mode);
int init_sserial_mode(int fd, char *command, char mode);
int sserial_write_cmd(int fd, char *command);
int sserial_read(int fd, char *command);
char *parse_nema_sentence(char *nema_buffer);
int send_string_UDP(int fd, char *dev, unsigned int baud);
int init_gsm_udp(int fd, char *dev, unsigned int baud);

int wait_flag = TRUE;														/* wait to make sure read is valid during loops */
unsigned int packet;														/* packet number of current run */
char message[MAX_BUFFER_SIZE];												/* character buffer, can be used to store up to size */

volatile int STOP = FALSE;													/* used for looping while reading serial device */

/**************************************************************************************************************
 * Initialize Serial Device                                                                                   *
 * Implementation based on code from: http://en.tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html#AEN144      *
 **************************************************************************************************************
 * init_sserial: initialize software serial for serial device (dev)                                           *
 * @param dev: serial device to open to perform read/write operations                                         *
 * @param baud: baud rate to use to open serial device                                                        *
 * @param cmd: command to execute                                                                             *
 * @param mode: 'r' equals read, 'w' equals write '+' equals write then read                                  *
 * return fd: did the serial port initialize correctly                                                        *
 **************************************************************************************************************/
int init_sserial(char *dev, int baud, char *cmd, char mode){
	int fd = 0;
	int n = 0;
	struct termios t_old_io, t_new_io;
	struct sigaction sig_act_io;

	/**********************************************************************************************************
	 * open serial device for reading (read will return immediately)                                          *
	 * @oflag O_RDWR:    File Access Mode                                                                     *
	 * @mode O_NOCTTY:   If pathname refers to a terminal device—see tty(4)—it will not become the process's  *
	 *                   controlling terminal even if the process does not have one.                          *
	 * @mode O_NONBLOCK: When possible, the file is opened in nonblocking mode.                               *
	 *                   Neither the open() nor any subsequent operations on the file descriptor which is     *
	 *                   returned will cause the calling process to wait.                                     *
	 **********************************************************************************************************/
	fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(fd < 0){
		perror("ERROR: Couldn't open device!");
		return -1;
	}
	/* install the signal handler before making the device asynchronous */
	sig_act_io.sa_handler = signal_handler_IO;
	sigemptyset(&sig_act_io.sa_mask);
	sigaddset(&sig_act_io.sa_mask, 0);
	sig_act_io.sa_flags = 0;
	sig_act_io.sa_restorer = NULL;
	sigaction(SIGIO, &sig_act_io, NULL);

	fcntl(fd, F_SETOWN, getpid());	/* allow the process to receive SIGIO (signal input and output) */
	/************************************************************************************************
	 * Make the file descriptor asynchronous (the manual page says                                  *
	 * only O_APPEND and O_NONBLOCK, will work with F_SETFL...)                                     *
	 ************************************************************************************************/
	fcntl(fd, F_SETFL, FASYNC);
	tcgetattr(fd, &t_old_io);		/* save current port settings */

	/* set new port settings for canonical input processing */
	t_new_io.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;
	t_new_io.c_iflag = IGNPAR | ICRNL;
	t_new_io.c_oflag = 0;
	t_new_io.c_lflag = ICANON;
	t_new_io.c_cc[VMIN] = 1;
	t_new_io.c_cc[VTIME] = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &t_new_io);

	/* start processing while device is waiting for input. */
	n = init_sserial_mode(fd, cmd, mode);
	tcsetattr(fd, TCSANOW, &t_old_io);
	return fd;
}

/**************************************************************************************************************
 * init_sserial_mode: initialize software serial to read ('r'), write ('w'), and write then read ('+')        *
 * @param fd: valid file descriptor                                                                           *
 * @param cmd: command to execute                                                                             *
 * @param mode: 'r' equals read, 'w' equals write '+' equals write then read                                  *
 * return fd: did the serial port initialize correctly (fd), return -1 on error                               *
 **************************************************************************************************************/
int init_sserial_mode(int fd, char *cmd, char mode){

	int n = 0;
	/* write/read command */
	if(mode == S_WR){
		if((n = sserial_write_cmd(fd, cmd)) < 0){
			perror("ERROR: Couldn't write to serial port");
			return -1;
		}
		if((n = sserial_read(fd, cmd) < 0)){
			perror("ERROR: Couldn't read from serial port");
			return -1;
		}
	}
	/* write command string */
	if(mode == S_WRITE_CMD && (n = sserial_write_cmd(fd, cmd) < 0)){
		perror("ERROR: Couldn't write to serial port");
		return -1;
	}
	/* read based on command */
	if(mode == S_READ && (n = sserial_read(fd, cmd) < 0)){
		perror("ERROR: Couldn't read from serial port");
		return -1;
	}
	return fd;
}

/**************************************************************************************************************
 * sserial_write_cmd: write command (cmd) in bytes to serial device that is waiting for write                 *
 * @param fd: valid file descriptor                                                                           *
 * @param cmd: command to execute                                                                             *
 * return fd: did the serial port initialize correctly (fd), return -1 on error                               *
 **************************************************************************************************************/
int sserial_write_cmd(int fd, char *cmd){
	uint8_t b = 0;
	int pos = 0;
	int n = 0;
	char *newline = "\n";
	char *null = "\0";

	printf("\nWRITING CMD\n\n");
	while(cmd[pos] != '\0'){
		b = XSTR_TO_BYTE(cmd, pos);
		n = write(fd, &b, 1);
		pos++;
	}
	b = XSTR_TO_BYTE(newline, 0);
	n = write(fd, &b, 1);
	b = XSTR_TO_BYTE(null, 0);
	n = write(fd, &b, 1);
	if(n != 1){
		return -1;
	}
	return fd;
}

/**************************************************************************************************************
 * sserial_read: loop reading serial device's bytes and write bytes to buffer                                 *
 * @param fd: valid file descriptor                                                                           *
 * @param cmd: command to execute                                                                             *
 * return fd: did the serial port initialize correctly (fd), return -1 on error                               *
 **************************************************************************************************************/
int sserial_read(int fd, char *cmd){
	unsigned int i = 0;
	int n = 0;
	uint8_t *buf;		/* byte buffer for reading serial device */
	char *n_str;		/* NEMA character buffer, if valid, set's to global variable 'message' */

	printf("READING\n");
	buf = (uint8_t*)malloc(MAX_BUFFER_SIZE * sizeof(uint8_t));
	while(STOP == FALSE){
		if(wait_flag == FALSE){
			/* bounds check/reset */
			if(i > MAX_BUFFER_SIZE - 1){
				i = 0;
			}
			/* read devices buffer, and store read bytes to byte buffer (buf) */
			n = read(fd, &buf[i], 1);

			/* check for end of command for success or error */
			if(i > 2 && (((char)buf[i - 1] == 'O' && (char)buf[i] == 'K') || ((char)buf[i - 4] == 'E' && (char)buf[i - 3] == 'R' && (char)buf[i - 2] == 'R' && (char)buf[i - 1] == 'O' && (char)buf[i] == 'R' ))) STOP = TRUE;

			/* This checks for a valid $GPGGA lat/lon buffer
			 * Example lat/lon buffer below
			 * $GPGGA,033443.000,4114.6549,N,09600.7724,W,2,06,2.30,325.6,M,-28.7,M,0000,0000*5C
			 */
			if((char)buf[i] == ',' && (char)buf[i - 1] == 'A' && (char)buf[i - 2] == 'G' && (char)buf[i - 3] == 'G' && (char)buf[i - 4] == 'P' && (char)buf[i - 5] == 'G' && (char)buf[i - 6] == '$'){
				free(buf);												/* no longer need uint8_t buffer */
				n_str = (char*)malloc(MAX_BUFFER_SIZE * sizeof(char));	/* allocate NEMA string's memory */
				i = 0;													/* reset index i to zero */
				while(STOP == FALSE){
					n = read(fd, &n_str[i], sizeof(char));
					if(n_str[i] == '\n'){
						n_str[i] = '\0';
						STOP = TRUE;
					}
					i++;
				}
				n_str = parse_nema_sentence(n_str);
				if(n_str[0] == '0' && n_str[8] == '0'){
					printf("Coords are 0\n");
				}else{
					if(strlen(n_str) > 0 && strlen(n_str) < MAX_BUFFER_SIZE){
						memset(message, '\0', MAX_BUFFER_SIZE + 1);
						strcpy(message, n_str);
						printf("COORDS:%s\n", message);
					}
				}
			}
		}
		i++;
	}
	wait_flag = TRUE;
	STOP = FALSE;
	if(n < 0){
		return -1;
	}
	return fd;
}

/**************************************************************************************************************
 * parse_nema_sentence: parse $GPGGA NEMA sentence and return parsed values in comma separated buffer         *
 * @param buf: $GPGGA character buffer to parse                                                               *
 * return msg: parsed $GPGGA character buffer                                                                 *
 **************************************************************************************************************/

char *parse_nema_sentence(char *buf){
	unsigned int i = 0;
	uint8_t degree, hour, minute, seconds;
	uint32_t time;
	float ftime;
	char *p;
	char tmp_buf[10];
	char *msg = (char*)malloc(MAX_BUFFER_SIZE * sizeof(char));
	float lat, lon;

	p = strtok(buf, ",");
/*	Can use to get time from GPS
 * 	ftime = atof(p);
 * 	time = ftime;
 * 	hour = time / 10000;
 * 	minute = (time % 10000) / 100;
 * 	seconds = (time % 100);
 * 	printf("Hour:%d Min:%d Sec:%d\n", hour, minute, seconds);
 */
	while(p != NULL){
		switch(i){
			case 1:
				strncpy(tmp_buf, p, 2);
				tmp_buf[2] = '\0';
				degree = atoi(tmp_buf);								/* store degrees */
				strncpy(tmp_buf, p + 2, 7);							/* store minutes into buffer */
				tmp_buf[7] = '\0';
				lat = ((degree) + (float)(atof(tmp_buf) / 60));		/* convert to decimal degrees */
				sprintf(tmp_buf, "%.5f", lat);						/* truncate float to 5 decimal places */
				break;
			case 2:
				if(p[0] == 'S') sprintf(tmp_buf, "%.5f", lat * -1);	/* multiply lat by -1 if south */
				strcat(msg, tmp_buf);
				strcat(msg, ",");									/* append GPS lat to message */
				break;
			case 3:
				strncpy(tmp_buf, p, 3);
				tmp_buf[3] = '\0';
				degree = atoi(tmp_buf);								/* store degrees */
				strncpy(tmp_buf, p + 3, 7);							/* store minutes into buffer */
				tmp_buf[7] = '\0';
				lon = ((degree) + (float)(atof(tmp_buf) / 60));		/* convert to decimal degrees */
				sprintf(tmp_buf, "%.5f", lon);						/* truncate float to 5 decimal places */
				break;
			case 4:
				if(p[0] == 'W') sprintf(tmp_buf, "%.5f", lon * -1);	/* multiply lon by -1 if south */
				strcat(msg, tmp_buf);
				strcat(msg, ",");									/* append GPS lon to message */
				break;
			case 5:
				strcat(msg, p);										/* append GPS signal strength to message */
				strcat(msg, ",");
				break;
			case 6:
				sprintf(tmp_buf, "%d", atoi(p));
				strcat(msg, tmp_buf);								/* append satellites in view of GPS to message */
				break;
		}
		p = strtok(NULL, ",");										/* keep on toking up! */
		i++;
	}
	return msg;
}

/* called by defined SEND_MSG_UDP(fd, dev, baud) macro, look at comment there for more! */
int send_string_udp(int fd, char *dev, unsigned int baud){
	unsigned int len = 0;

	RUN_CMD(fd, dev, baud, SEND_UDP_MSG, 'w');		/* open CIP to send message over UDP, write command ('w') */
	len = strlen(message);							/* get length of message character buffer */
	message[len] = CTRL_Z;							/* append CTRL-Z control character to end of buffer */
	len++;											/* increment character buffer length by one */
	message[len] = '\0';							/* append null terminator to end of character buffer */
	printf("SENDING COORD:%d | %s\n", ++packet, tmp_buf);
	RUN_CMD(fd, dev, baud, message, 'w');			/* insert and send message over UDP connection, write command ('w') */
	return fd;
}

/* called by defined INIT_GSM_UDP(fd, dev, baud) macro, look at comment there for more! */
int init_gsm_udp(int fd, char *dev, unsigned int baud){
	RUN_CMD(fd, dev, baud, SET_APN, '+');			/* set GSM access point, write then read checks for 'OK' ('+') */
	RUN_CMD(fd, dev, baud, CHECK_WIRELESS, '+');	/* check wireless connection, write then read checks for 'OK' ('+') */
	RUN_CMD(fd, dev, baud, GET_LOCAL_IP, 'w');		/* get local IP address of GSM unit, write command ('w') */
	RUN_CMD(fd, dev, baud, START_UDP, '+');			/* start UDP connection to server, write then read checks for 'OK' ('+') */
	return fd;
}

/**************************************************************************************************************
 * signal_handler_IO: handle signal interrupts from asynchronous serial device, sets wait_flag to FALSE,      *
 *                    to indicate characters in loop have been received.                                      *
 * @param status: set status                                                                                  *
 **************************************************************************************************************/
void signal_handler_IO(int status){
	printf("%s", "\nReceived SIGIO signal.\n");
	wait_flag = FALSE;
}

#endif
