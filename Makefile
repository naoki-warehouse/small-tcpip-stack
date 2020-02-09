CC=gcc
sts: sts.c
	$(CC) sts.c -o sts

clean: sts
	rm sts
