#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define GPIO_EXPORT_PATH	"/sys/class/gpio/export"
#define MODE_NORMAL	0
#define MODE_WOR_TX	1
#define MODE_WOR_RX	2
#define MODE_CFG	3

int gpio_init();
int mode_set();
int gpio_set();
int gpio_read();


int main()
{
    gpio_init(28);
    gpio_init(29);
    gpio_init(30);

    gpio_set(28, 1, 1);
    mode_set(MODE_NORMAL);

    while(1){
        while(gpio_read(28));
	printf("checkm8!\n");
	usleep(5000);
    }
    //printf("gpio value:%d\n", gpio_read(28));

    return 0;
}

int mode_set(int mode)
{
    if (mode < 0 && mode > 3) {
	printf("Wrong mode!\n");
	return -1;
    } else if (mode == 0) {
	gpio_set(29, 0, 0);
	gpio_set(30, 0, 0);
    } else if (mode == 1) {
	gpio_set(29, 0, 0);
	gpio_set(30, 0, 1);
    } else if (mode == 2) {
	gpio_set(29, 0, 1);
	gpio_set(30, 0, 0);
    } else if (mode == 2) {
	gpio_set(29, 0, 1);
	gpio_set(30, 0, 1);
    }
}

int gpio_init(int gn)
{
    //Set gpio path
    char gpio_folder_path[128];
    snprintf(gpio_folder_path, sizeof(gpio_folder_path), "/sys/class/gpio/gpio%d", gn);
    //printf("%s\n", gpio_folder_path);

    //Export gpio
    if (access(gpio_folder_path, F_OK) == 0) {
	printf("%s already exists\n", gpio_folder_path);
    } else if (access(gpio_folder_path, F_OK) == -1) {
	int fd_export = open(GPIO_EXPORT_PATH, O_WRONLY);
	if (fd_export == -1) {
	    perror("Error opening export file");
	    return -1;
	}

	char gn_char[10];
	snprintf(gn_char, sizeof(gn_char), "%d", gn);
	if (write(fd_export, gn_char, sizeof(gn)) == -1) {
	    char err[128];
	    snprintf(err, sizeof(err), "Error exporting gpio%d", gn);
	    perror(err);
	    return -1;
	}
	close(fd_export);
	printf("gpio%d exported\n", gn);
    }
}

int gpio_set(int gn, int dir, int val)
{
    //Set gpio path
    char gpio_folder_path[128];
    snprintf(gpio_folder_path, sizeof(gpio_folder_path), "/sys/class/gpio/gpio%d", gn);
    //Set direction
    const char *dirString = (dir == 0) ? "out" : "in";
    char gpio_direction[128];
    //printf("dir: %s\n", dirString);
    snprintf(gpio_direction, sizeof(gpio_direction), "%s/direction", gpio_folder_path);
    //printf("%s\n", gpio_opetator);
    int fd_direction = open(gpio_direction, O_WRONLY);
    if (fd_direction == -1) {
	char err[128];
	snprintf(err, sizeof(err), "Error opening gpio%d", gn);
	perror(err);
	return -1;
    }

    write(fd_direction, dirString, strlen(dirString));
    if (fd_direction == 0) {
	printf("gpio%d set direction to %d", gn, dir);
    } else if (fd_direction == -1) {
	char err[128];
	snprintf(err, sizeof(err), "Error setting gpio%d direction", gn);
	perror(err);
	return -1;
    }
    close(fd_direction);
    printf("gpio%d direction: %s\n", gn, dirString);

    //Set value
    char gpio_write[128];
    snprintf(gpio_write, sizeof(gpio_write), "%s/value", gpio_folder_path);
    //printf("%s", gpio_write);
    int fd_write = open(gpio_write, O_WRONLY);
    if (fd_write == -1) {
	char err[128];
	snprintf(err, sizeof(err), "Error opening %s", gpio_write);
	return -1;
    }
    char gn_value[2];
    snprintf(gn_value, sizeof(gn_value), "%d", val);
    if (write(fd_write, gn_value, sizeof(gn_value)) == -1) {
	char err[128];
	snprintf(err, sizeof(err), "Error setting value %d of gpio%d", val, gn);
	return -1;
    }
    close(fd_write);
    return 0;
}

int gpio_read(int gn)
{
    //Set path
    char gpio_value_path[128];
    snprintf(gpio_value_path, sizeof(gpio_value_path), "/sys/class/gpio/gpio%d/value", gn);
    //Read  value
    char read_value[3];
    int fd_read = open(gpio_value_path, O_RDONLY);
    if (fd_read == -1) {
	char err[128];
	snprintf(err, sizeof(128), "Error opening gpio%d", gn);
	perror(err);
	return -1;
    }

    //printf("path: %s", gpio_value);
    if (read(fd_read, read_value, sizeof(read_value)) == -1){
        perror("Error reading gpio");
	return -1;
    }
    //printf("gpio%d value:%s\n", gn, read_value);
    close(fd_read);
    return (atoi(read_value));
}
/*
int gpio_read(int gn)
{
    char path[128];
    char value_str[3];
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gn);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open gpio value for reading!");
        return -1;
    }

    if (read(fd, value_str, 3) < 0) {
        perror("Failed to read value!");
        return -1;
    }

    close(fd);
    return (atoi(value_str));
}   //get gpio(n)'s value
*/
