#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

struct request {
	uint8_t cmd;
	uint8_t args[3];
};

int main(int argc, char **argv)
{
	int fd;
	struct request r = {0};

	if ((argc != 2) && (argc != 4)) {
		printf("Missing argument!\n");
		printf("Usage: %s <hidraw> <io> <val>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return fd;
	}

	if (argc == 2) {
		// Read state of the button 
		r.cmd = 'g';
		write(fd, &r, sizeof(r));
		read(fd, &r, sizeof(r));
		printf("Button is %spressed\n", r.args[0] == 0 ? "" : "not ");
	}
	if (argc == 4) {
		r.cmd = 's';
		r.args[0] = atoi(argv[2]);
		r.args[1] = atoi(argv[3]);
		write(fd, &r, sizeof(r));
	}

	close(fd);
	return 0;
}

