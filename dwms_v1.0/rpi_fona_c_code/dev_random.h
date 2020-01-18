#ifndef DEV_RANDOM
#define DEV_RANDOM
#define BUFFER_SIZE 32
#include <stdio.h>  /* For Input/Output on Streams */
#include <stdint.h> /* strictly to use uintN_t, for N(8) bit unsigned integer type */
#include <errno.h>  /* handle signal errors, variable: errno */
#include <fcntl.h>  /* to use open(), read(), and close() functions for files*/
#include <unistd.h>
/*#include <signal.h>*/
#include <linux/random.h> /* For RNDGETENTCNT flag */
#include <time.h>
#include <sys/ioctl.h>

ssize_t safe_read(int file, void* buffer, size_t count);
int randomInt(int entropy);

/* Produce a seed for cryptographic pseudo-random number generator (CPRNG) */
int randomInt(int entropy){
    /* ssize_t randomLen  */
    ssize_t randomLen = 0;

    /* uint8_t buffer:
     * 8-bit usigned MAX needed is 256-bits
     * buffer size 32 or less because--
     * no cryptographic primitive available today can hope to promise more than 256 (32*8) bits of security */
    uint8_t buffer[BUFFER_SIZE];
    /* int fd
     * open /dev/{u}random (for speed use urandom, security use random), as read only
     *
     */
    int fd = open("/dev/urandom", O_RDONLY);
    int myRandomInt; /* random integer generated we are going to get from /dev/{u}random */

    /* int entropy
     * entropy is another pseudo-random number, needs to be a large number M^3.
     */

    /* int entropy = (time(NULL))*(time(NULL) * time(NULL) * time(NULL)); */

    /* RNDGETENTCNT - Retrieve the entropy count of the input pool, the contents
     * will be the same as the entropy_avail file under proc. The result will be
     * stored in the int pointed to by the argument (entropy).
     *
     * We use ioctl to call straight to the device driver (/dev/random).
     * ioctl(int openFile, requestCodeNumber, unsigned int of 8 bits)
     *
     */

    ssize_t result;
    printf("%s%d\n", "Entropy: ", entropy);
    if(!ioctl(fd, RNDGETENTCNT, &entropy) && (entropy >= (sizeof(buffer) * 8))){

        /* result will safe_read(*,*,*) and fill result to the size of buffer */
        result = safe_read(fd, buffer, sizeof(buffer));
        if(result > 0){
            /* printf("%s", "Success!");*/
            while(randomLen < sizeof(myRandomInt)){
                ssize_t result = safe_read(fd, ((char*)&myRandomInt) + randomLen, (sizeof(myRandomInt) - randomLen));
                if(result < 0){
                    printf("%s", "Couldn't read /dev/{u}random");
                }
                randomLen += result;
                /*printf("%s%zd\n", "Size of myRandomInt: ", sizeof(myRandomInt));*/
            }

            /*If result does not become larger than our buffer due to an interupted safe_read,
              this will continue the process til complete. Had to force it to happen so far.*/
            if(result < sizeof(buffer)){
                printf("%s", "Filling Buffer.....");
                while(randomLen < sizeof(myRandomInt)){
                    result = safe_read(fd, ((char*)&myRandomInt) + randomLen, (sizeof(myRandomInt) - randomLen));
                    if(result < 0){
                        printf("Couldn't read /dev/{u}random");
                    }
                    randomLen += result;
                    /* printf("%d\n", myRandomInt); */
                }
            }
        }
    }
    close(fd);  /* close /dev/{u}random */
    return myRandomInt;
}


/* ssize_t safe_read(int file READONLY, void* bufferSize, size_t count)
 * this will read a file , if interupted it will still continue to read
 * and return the amount of the size of read_amount that was collected
 *
 */
ssize_t safe_read(int fd, void* buf, size_t count){
    ssize_t read_amount = 0;
    ssize_t r; /* number of read bytes from read() */

    while((size_t)read_amount < count){

        /* The read() function attempts to read nbytes from the file associated with handle, and places the characters
         * read into buffer. If the file is opened using O_TEXT, it removes carriage returns and detects the end of the file.
         * The function returns the number of bytes read. On end-of-file, 0 is returned, on error it returns -1,
         * setting errno to indicate the type of error that occurred.
         *
         */
        r = read(fd, ((char*)buf + read_amount), (count - read_amount));
        if(r > 0){
            read_amount += r;
        }else if(!r){
            printf("%s", "Could not open /dev/{u}random");
            break;
        }
        /*
         * EINTR - The read operation was terminated due to the receipt of a signal, and no data was transferred.
         * I had to force this error by cat /dev/random in another shell, and have it be my only if statement.
         * Nonetheless, better safe than sorry.
         *
         */
        else if(errno != EINTR){
            read_amount = -1;
            break;
        }
    }
    return read_amount;
}

#endif
