#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG_PRO


#define HEAD_SIZE     16
#define TS_SIZE       8
#define TS_OFFSET     0
#define TEXT_SIZE     2
#define RECORD_OFFSET 8
#define TEXT_OFFSET   10
#define DICT_OFFSET   12
#define PREFIX_MAX    32
#define LOG_LINE_MAX  (1024 - PREFIX_MAX)
#define BUF_SIZE      (PREFIX_MAX+LOG_LINE_MAX)

#define ROUNDUP(x,y)	((((x)+(y)-1)/(y))*(y))

enum log_flags {
	LOG_NOCONS	= 1,	/* already flushed, do not print to console */
	LOG_NEWLINE	= 2,	/* text ended with a newline */
	LOG_PREFIX	= 4,	/* text started with a prefix */
	LOG_CONT	= 8,	/* text is a fragment of a continuation line */
};

int main(int argc, char *argv[])
{
	int ret, count=0;
	int fd;
	int noprefix_nr = 1;
	int find_flag = 0;
	__off_t start_offset, cur_offset, end_offset;
	FILE *file;
	unsigned char text_buf[BUF_SIZE];
	unsigned char temp_buf[BUF_SIZE];
	unsigned char head_buf[HEAD_SIZE];
	unsigned long ts;
	unsigned short text_size, text_size_roundup, record_size, dict_size;
	unsigned char print_prefix = 0;

	if(argc < 2)
	{
		printf("please input file name\n");
		exit(-1);
	}

	if (argv[1] != NULL) {
		fd =open(argv[1], O_RDONLY);
	} else {
		printf("file name error\n");
		exit(-1);
	}

	if (fd < 0) {
		printf("cannot open input file:%s\n", argv[1]);
		exit(-2);
	}

	printf("<-----------------------Copyright (C) 2015~2016  Superm Wu-------------------------->\n");

	while (1) {
read_head:
		ret = read(fd, head_buf, HEAD_SIZE);
		if(ret < 0)
		{
			printf("err to read head\n");
			exit(-3);
		}
		else if ((ret != HEAD_SIZE) && (ret >= 0))
		{
			printf("<-----------------------Copyright (C) 2015~2016  Superm Wu-------------------------->\n");
			close(fd);
			return 0;
		}

		memcpy((void *)&ts, (void *)head_buf, TS_SIZE);
		memcpy((void *)&text_size, (void *)(&head_buf[TEXT_OFFSET]), 2);
		memcpy((void *)&record_size, (void *)(&head_buf[RECORD_OFFSET]), 2);
		memcpy((void *)&dict_size, (void *)(&head_buf[DICT_OFFSET]), 2);
		print_prefix = head_buf[HEAD_SIZE-1];
		text_size_roundup = ROUNDUP((text_size + dict_size), 4);

		if ((record_size != (text_size_roundup + HEAD_SIZE)) || (text_size_roundup > BUF_SIZE )) {
			if (find_flag == 0) {
				start_offset = lseek(fd, -0x10, SEEK_CUR);
				find_flag = 1;
				if (noprefix_nr == 1) {
					printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^end^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
					printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvstartvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
				}
			}
			lseek(fd, (-HEAD_SIZE+1), SEEK_CUR);
			goto read_head;
		}

		if (find_flag == 1) {
			cur_offset = lseek(fd, 0, SEEK_CUR);
			end_offset = cur_offset - 0x10;
			lseek(fd, start_offset, SEEK_SET);
			read(fd, text_buf, end_offset - start_offset);
			text_buf[end_offset - start_offset] = '\0';
			printf("%s\n", text_buf);
			lseek(fd, cur_offset, SEEK_SET);
			find_flag = 0;
			noprefix_nr++;
		}

		if(read(fd, text_buf, text_size_roundup) < 0)
		{
			printf("err to read text\n");
			exit(-3);
		}

		if ((print_prefix & LOG_NEWLINE) == LOG_NEWLINE) {
			if ((print_prefix & LOG_PREFIX)) {
				if (text_size) {
					memcpy(temp_buf, text_buf, text_size);
					temp_buf[text_size] = '\n';
				}
				if (dict_size) {
					for (count = 0; count <= dict_size;) {
						count += sprintf(temp_buf+text_size+count+1, "%s", text_buf+text_size+count);
						count++;
						temp_buf[text_size+count] = '\n';
					}
					temp_buf[text_size+count] = '\n';
					temp_buf[text_size+count+1] = '\0';
				} else {
					temp_buf[text_size+1] = '\0';
				}
				printf("[%5lu.%06lu] %s", (unsigned long)ts/1000000000, (unsigned long)(ts%1000000000)/1000, temp_buf);
			} else {
				text_buf[text_size] = '\n';
				text_buf[text_size+1] = '\0';
				printf("[%5lu.%06lu] %s", (unsigned long)ts/1000000000, (unsigned long)(ts%1000000000)/1000, text_buf);
			}
		} else if ((print_prefix & LOG_CONT) == LOG_CONT) {
			printf("%s", text_buf);
		}
	}
}
