#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int fd;

	if (argc != 2)
		abort();
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("mksocket");
		return 1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if (strlen(argv[1]) >= sizeof(addr.sun_path)) {
		errno = ENAMETOOLONG;
		perror("mksocket");
		return 1;
	}
	strcpy(addr.sun_path, argv[1]);
	if (bind(fd, (void *)&addr, (socklen_t)sizeof(addr))) {
		perror("mksocket");
		return 1;
	}
	close(fd);

	return 0;
}
