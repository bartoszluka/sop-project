#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define ERR(source) (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
					 perror(source), kill(0, SIGKILL),               \
					 exit(EXIT_FAILURE))

off_t fsize(const char *filename)
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

int main(int argc, char *argv[])
{
	char *path = "save.save";
	int filesize = fsize(path);
	char *buff = (char *)malloc(sizeof(char) * filesize);
	if (!buff)
		ERR("malloc");

	int fd;
	if ((fd = open(path, O_RDONLY)) < 0)
		ERR("open");

	//zmienic na bulk_read/obrone przed sygnalami
	if (read(fd, buff, filesize) < 0)
	{
		ERR("read");
	}

	struct element
	{
		char *key;
		char *data;
	} e;

	// Load the buffer...

	// Get the key and its data...
	e.key = strtok(buff, "\n");
	e.data = strtok(NULL, "\n");
	printf("%s\n", e.data);
	free(buff);
	// Process the rest of the contents of the buffer...
}