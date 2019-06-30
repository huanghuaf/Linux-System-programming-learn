/*
 * atomic_append:an exercise of the linux programming interface
 * 
 * test open file with O_APPEND to keep open file atomic.
 *
 * author:huafenghuang
 *
 * compile with:
 *
 *	gcc -o atomic_append atomic_append.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
static void usage(char *name)
{
	if(!name)
		return;
	printf("usage:\n%s file numbyte [x]\n",name);
}

int main(int argc, char *argv[])
{
	int err = 0;
	int fd;
	int num_bytes;
	char *file = NULL;
	int flag = O_RDWR;
	char *buf = "F";

	if (argc < 3) {
		usage(argv[0]);
		err = -1;
		return err;
	} else {
		file = argv[1];
		num_bytes = atoi(argv[2]);
	}

	if (argc == 4 && !strcmp(argv[3],"x"))
		flag |= O_APPEND;

	fd = open(file, flag, 0666);
	if (fd < 0) {
		fd = open(file, flag | O_CREAT, 0666);
		if (fd < 0){
			err = -1;
			printf("create file %s failed!\n", file);
			close(fd);
			return err;
		}
	}

	while(num_bytes){
		if (flag & O_APPEND) {
			//printf("open filw with O_APPEND arg\n");
			err = write(fd, buf,strlen(buf));
			if(err < 0) {
				printf("write buf to file error!\n");
				close(fd);
				return err;
			}
		} else {
			lseek(fd, 0, SEEK_END);
			err = write(fd, buf, strlen(buf));
			if(err < 0) {
				printf("write buf to file error!\n");
				close(fd);
				return err;
			}
		}
		num_bytes--;
	}
	return 0;
}
