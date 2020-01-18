#include <stdio.h>
#include <stdlib.h>
#include "dev_random.h"

#ifndef MAX_BUFFER_SIZE
	#define MAX_BUFFER_SIZE 255
#endif

unsigned char *encrypt_otp(unsigned char *buffer, unsigned int buffer_len);
unsigned char *xor_buffers(unsigned char *buffer_1, unsigned char *buffer_2, unsigned int buffer_len);


unsigned char *encrypt_otp(unsigned char *buf, unsigned int len){
	unsigned char k_buf[MAX_BUFFER_SIZE];
	unsigned int i = 0;
	int seed = 0;

	/* generate one-time pad */
	seed = randomInt(len * (len * len) * len * (len * len) * len * (len * len));
	printf("Random Int: %d\n", seed);
	/*for(i = 0; i < )
	sprintf(k_buf, )*/

	k_buf[0] = '\0';
	return xor_buffers(buf, k_buf, len);
}

unsigned char *xor_buffers(unsigned char *buf_1, unsigned char *buf_2, unsigned int len){
	unsigned int i = 0;
	for(i = 0; i < len; i++){
		buf_1[i] ^= buf_2[i];
	}
	return buf_1;
}
