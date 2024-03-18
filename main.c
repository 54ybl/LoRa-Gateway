#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define lora0	"/dev/ttyUSB0"
#define lora1	"/dev/ttyACM0"

int main(){
    int fd;
    fd = open(lora1, O_RDWR | O_NOCTTY);
    if (fd < 0) {
	perror("Error opening serial port");
	return -1;
    }

    struct termios tty;
    tcgetattr(fd, &tty);
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
	perror("Error configuring serial");
	return -1;
    }

    system("stty -F /dev/ttyACM0 speed");

    while(1){
	char buf[255];
	//RX
	//printf("Ready to RX\n");
	int fd_read = read(fd, buf, sizeof(buf));
	printf("fd read: %s\n", buf);
	sleep(1);

	//TX
	//printf("Ready to TX\n");
	//int fd_write = write(fd, buf, sizeof(buf));
	//printf("TX Done, return %d\n", fd_write);
    }
    close(fd);
}
