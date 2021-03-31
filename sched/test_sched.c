#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/prctl.h>

int main()
{
	int ret;
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return -EINVAL;
	else if (pid == 0) {
		/* child process */
		pid_t child_pid;
		struct sched_param sd;
		/* 1~99 */
		sd.sched_priority = 10;
		child_pid = getpid();
		printf("pid of child process:%d\n", child_pid);
		ret = prctl(PR_SET_NAME, "child", NULL, NULL, NULL);
		if (ret < 0 ) {
			printf("set child process name fail\n");
			return -EINVAL;
		}
		ret = sched_setscheduler(child_pid, SCHED_FIFO, &sd);
		if (ret < 0) {
			printf("set scheduler fail\n");
			return -EINVAL;
		}
		while(1);
	} else {
		/* father process */
		pid_t father_pid;
		struct sched_param sd;
		/* 1~99 */
		sd.sched_priority = 10;
		father_pid = getpid();
		ret = prctl(PR_SET_NAME, "father", NULL, NULL, NULL);
		if (ret < 0 ) {
			printf("set child process name fail\n");
			return -EINVAL;
		}
	//	ret = sched_setscheduler(father_pid, SCHED_FIFO, &sd);
		printf("pid of father process:%d\n", father_pid);
		while (1) {
			sleep(1);
			printf("father process doing...\n");
		}
	}
	return 0;
}
