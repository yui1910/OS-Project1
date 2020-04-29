CFLAG = -std=c99
FIND_H = -I /usr/src/linux-headers-4.15.0-96/include	

main: main.o run.o process.o
	gcc $(CFLAG) main.o run.o process.o -o main
main.o: main.c Makefile
	gcc $(CFLAG) main.c -c
run.o: run.c run.h Makefile
	gcc $(CFLAG) run.c -c
process.o: process.c process.h Makefile
	gcc $(CFLAG) process.c -c
clean:
	rm -rf *o
run:
	sudo ./main
