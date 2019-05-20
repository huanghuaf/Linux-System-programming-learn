/*
 * tee_test:an exercise of the linux programming interface
 *
 * author:huafenghuang
 *
 * compile with:
 *
 *	gcc -o tee_test tee_test.c
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
#include <getopt.h>
#include <regex.h>
#include <errno.h>

struct option opts[] = {
	{ "append", 0, NULL, 'a' },
	{ "--ignore-interrupts", 0, NULL, 'i' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, 0, NULL, 0 }
};


static void usage(void)
{
	printf("tee_test 5/18/2019. (c) 2019 huafenghuang/(c).\n\n"
		"tee_test [-ai] file\n"
		"tee_test -h\n"
		"-a|--append                     append to the given FILEs, do not overwriten\n"
		"-h|--help                       Show usage information\n"
		"-i|--ignore-interrupts          ignore interrupt signals\n"
	);
}
/*
 *	handle_file(char *file)
 *arg:
 *	file:the file name,include path*
 *
 */
static int handle_file(int argc, char *file)
{
	int err = 0;
	int fd = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if(file == NULL) {
		err = -1;
		printf("use -a|--append must give a file!\n");
		return err;
	}

	if (argc < 3) {
		fd = open(file, O_RDWR, 0666);
		if(fd < 0) {
			fd = open(file, O_RDWR | O_CREAT, 0666);
			if(fd < 0) {
				err = -1;
				printf("creat file %s failed!\n",file);
				return err;
			}
		}
	}
	else {
		//1.判断文件是否存在，不存在则新建且打开，存在，则打开
		fd = open(file, O_RDWR|O_APPEND, 0666);

		if(fd < 0) {
			fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
			if(fd < 0) {
				err = -1;
				printf("creat file %s failed!\n",file);
				return err;
			}
		}
	}
	//2.对打开的文件，在文件最后面追加内容
	while((read = getline(&line, &len, stdin)) != -1) {
		err = write(fd, line, read);
		if(err<0) {
			close(fd);
			return err;
		}
	}
	close(fd);
	return err;
}
int main(int argc, char *argv[])
{
	int c;
	int err = 0;

	if (argc < 3) {
		err = handle_file(argc, argv[1]);
		if(err < 0) {
			printf("write char to %s error!\n",argv[1]);
			return err;
		}
		return 0;
	}
	while((c = getopt_long(argc, argv, "aih", opts, NULL)) != -1)
		switch(c) {
			case 'a':
				if (argc < 3) {
					printf("use -a|--append must give a file!\n");
					err = -1;
					return err;
				}
				err = handle_file(argc, argv[2]);
				if(err < 0) {
					printf("append to %s error!\n",argv[2]);
					return err;
				}
				break;
			case 'i':
				break;
			case 'h':
				usage();
				return 0;

		}

	return 0;
}
