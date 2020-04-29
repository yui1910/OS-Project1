#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include "run.h"

int FIND_NEXT(struct data *P, int N, int nowi, int type, int nowt, int startt)
{
	int nexti = -1;
	if (type == 0) { //FIFO
		for (int i = 0; i < N; i++) {
			if (P[i].PID == -1) //還沒開始
				continue;
			if (P[i].exec_t == 0) //已做完
				continue;
			nexti = i; //已用ready_t最小排列過
			break;
		}
	} else if (type == 1) { //RR
		if (nowi == -1) { //first time
			for (int i = 0; i < N; i++) {
				if (P[i].PID == -1)
					continue;
				if (P[i].exec_t == 0)
					continue;
				nexti = i;
				break;
			}
		} else if ((nowt - startt) % 500 == 0) { //已做超過時間
			nexti = (nowi + 1) % N; //選下一個
			while (P[nexti].PID == -1 || P[nexti].exec_t == 0)
				nexti = (nexti + 1) % N;
		} else {
			nexti = nowi;
		}
	} else if (type == 2) { //SJF
		if (nowi != -1)
			return nowi;
		for (int i = 0; i < N; i++) {
			if (P[i].PID == -1)
				continue;
			if (P[i].exec_t == 0)
				continue;
			if (nexti == -1 || P[nexti].exec_t > P[i].exec_t)
				nexti = i;
		}
	} else { //PSJF
		for (int i = 0; i < N; i++) {
			if (P[i].PID == -1)
				continue;
			if (P[i].exec_t == 0)
				continue;
			if (nexti == -1 || P[nexti].exec_t > P[i].exec_t)
				nexti = i;
		}
	}
	return nexti;
}

void SCHEDULE(struct data *P, int N, int type)
{
	//給當下process高priority
	/*struct sched_param p;
	p.sched_priority = 0;
	sched_setscheduler(getpid(), SCHED_OTHER, &p);*/

	int nowi = -1; //現在在跑的process index
	int nexti = -1;
	int startt = 0; //該process開始跑的時間
	int nowt = 0; //現在的時間
	int finish_N = 0; //跑完的process數
	while (1) {
		//確認nowi狀況
		if (nowi != -1 && P[nowi].exec_t == 0) { //nowi跑完了	
			//iwaitpid(P[nowi].PID, NULL, 0); //使child結束
			printf("%s %d\n", P[nowi].name, P[nowi].PID);
			nowi = -1;
			finish_N++;
			if (finish_N == N)
				break;
		}
		//找可以執行的P
		for (int i = 0; i < N; i++) {
			if (P[i].ready_t == nowt) {
				P[i].PID = START(P, i); //可執行
				BLOCK(P, i); //先暫停
			}
		}
		nexti = FIND_NEXT(P, N, nowi, type, nowt, startt);
		if (nexti != -1 && nexti != nowi) {
			if (nowi != -1)
				BLOCK(P, nowi);
			WAKEUP(P, nexti);
			nowi = nexti;
			startt = nowt;
		}
		//TIME
		volatile unsigned t;
		for (t = 0; t < 1000000UL; t++); //跑一次迴圈等於一nowt
		//nowP執行
		if (nowi != -1)
			P[nowi].exec_t--;
		nowt++;
	}
	return;
}
