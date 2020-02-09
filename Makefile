CC=gcc

sts: sts.c file.o
	$(CC) sts.c -o sts file.o

file.o: file.c
	$(CC) -c file.c

clean: sts file.o
	rm sts file.o
