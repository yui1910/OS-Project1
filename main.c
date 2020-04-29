#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run.h"
#define MAX_N 1000
#define MAX_S 1000

Data P[MAX_N];

int GET_TYPE()
{
	int type = -1;
	char type_name[MAX_S];
	scanf("%s", type_name);
	if (!strcmp(type_name, "FIFO"))
		type = 0;
	else if (!strcmp(type_name, "RR"))
		type = 1;
	else if (!strcmp(type_name, "SJF"))
		type = 2;
	else if (!strcmp(type_name, "PSJF"))
		type = 3;
	return type;
}

int INPUT()
{
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%s", P[i].name);
		scanf("%d%d", &(P[i].ready_t), &(P[i].exec_t));
	}
	for (int i = 0; i < N; i++) //初始化
		P[i].PID = -1;
	return N;
}

int SORT_COMPARE(const void *p1, const void *p2)
{
	Data *P1 = (Data *)p1;
	Data *P2 = (Data *)p2;
	int time1 = P1->ready_t;
	int time2 = P2->ready_t;
	if (time1 < time2)
		return -1;
	else if (time1 > time2)
		return 1;
	return 0;
}

void SORT(int N)
{
	qsort(P, N, sizeof(struct data), SORT_COMPARE);
	return;
}

int main() 
{
	// name, ready_t, exe_t for N process, Data P[]
	int type = GET_TYPE();
	if (type == -1) {
		printf("error! type = -1\n");
		exit(1);
	}
	int N = INPUT();
	for (int i = 0; i < N; i++) //初始化
		P[i].PID = -1;
	SORT(N); //把ready_t先的process排在前面	
	SCHEDULE(P, N, type);
	return 0;
}
