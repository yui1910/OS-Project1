#define MAX_N 1000
#define MAX_S 1000

typedef struct data {
	char name[MAX_S];
	int ready_t;
	int exec_t;
	int PID;
} Data;

void TO_DMESG(char *tag, char *s, int len);

int START(Data *P, int index);
void BLOCK(Data *P, int index);
void WAKEUP(Data *P, int index);
