#include "sserial.h" 					/* Software serial definitions */

/* FONA 808 AT command table */
/* GPS COMMANDS */
#define _AT "AT\n"						/* AT command, no parameters */
#define GPS_PWR_ON "AT+CGNSPWR=1\n"		/* Turn on GPS */
#define GPS_PWR_OFF "AT+CGNSPWR=0\n"	/* Turn off GPS */
#define GPS_LOCATION "AT+CGNSINF\n"		/* Prints NEMA string */
#define POWER_DOWN "AT+CPOWD=1\n"

/* GSM COMMANDS */
#define SET_APN "AT+CSTT=\"CMNET\"\n"
#define CHECK_WIRELESS "AT+CIICR\n"
#define GET_LOCAL_IP "AT+CIFSR\n"
#define START_UDP "AT+CIPSTART=\"UDP\",\"52.34.250.13\",\"52321\"\n"
#define SEND_UDP_MSG "AT+CIPSEND\n"
#define CLOSE_UDP "AT+CIPCLOSE\n"

/* GPS breakout command table */
#define READ_GPS "getGPGGA"			/* Command to read GPGAA NEMA string */

int main(int argc, char *argv[]){
	unsigned int sleep_counter = 0;
	int serial_fd_1 = 0;
	int serial_fd_2 = 0;

	INIT_GSM_UDP(serial_fd_1, SERIAL_DEV_1, SD1_BAUDRATE);
	printf("\nSLEEP %d\n", sleep_counter++);
	sleep(2);
	while(1){
		RUN_CMD(serial_fd_2, SERIAL_DEV_2, SD2_BAUDRATE, READ_GPS, 'r');
		printf("\nSLEEP %d\n", sleep_counter++);
		sleep(1);
		SEND_MSG_UDP(serial_fd_1);
		printf("\nSLEEP %d\n", sleep_counter++);
		sleep(1);
	}
	/*RUN_CMD(serial_fd_1, SERIAL_DEV_1, SD1_BAUDRATE, CLOSE_UDP, 'w');*/
	return 1;
}
