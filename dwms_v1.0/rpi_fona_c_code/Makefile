CC = gcc

CFLAGS = -g -Wall -pedantic
LMFLAG = -lm

TARGET = dwms_gps_logger

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).c $(LMFLAG)

clean:
	$(RM) $(TARGET)
