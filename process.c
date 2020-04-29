#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <fcntl.h>
#include "process.h"

void TO_DMESG(char *tag, char *s, int len)
{
	int tag_len = 3;
    char buffer[128] = {0};
    memcpy(&buffer[0], tag, tag_len);
    memcpy(&buffer[tag_len], s, len );
    int fd_kmsg = open("/dev/kmsg", O_WRONLY);
	if (fd_kmsg == -1) {
		printf("error!, can't open dmesg\n");
	} else { 
	write(fd_kmsg, &buffer, tag_len + len);
    close(fd_kmsg);
	}
	return;
}

int START(Data *P, int index)
{
	pid_t pid = fork();
	if (pid < 0) { //error
		printf("error! pid < 0\n");
		exit(1);
	}
	if (pid == 0) { //child
		struct timeval ST, FT;
		char to_dmesg[MAX_S] = {0};
		int st_check = gettimeofday(&ST, NULL);

		for (int nowt = 0; nowt < P[index].exec_t; nowt++) {
			volatile unsigned t; //UNIT TIME
			for (t = 0; t < 1000000UL; t++); //跑一次迴圈等於一nowt
		}

		gettimeofday(&FT, NULL);
		sprintf(to_dmesg, "[Project1] %d %lu.%09lu %lu.%09lu\n",
				getpid(), ST.tv_sec, ST.tv_usec, FT.tv_sec, FT.tv_usec);
		TO_DMESG("<5>", to_dmesg, strlen(to_dmesg));
		exit(0);
	}
	//parent
	return pid; 
}

void BLOCK(Data *P, int index)
{
	struct sched_param p;
	p.sched_priority = 0;

	//設成for running very low priority background jobs
	sched_setscheduler(P[index].PID, SCHED_IDLE, &p);
	return;
}
void WAKEUP(Data *P, int index)
{
	struct sched_param p;
	p.sched_priority = 0;

	//設成普通process
	sched_setscheduler(P[index].PID, SCHED_OTHER, &p);
	return;
}
