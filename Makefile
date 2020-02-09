CC=gcc

sts: sts.c file.o unix_sock.o dev_tap.o utils.o netdev.o client
	$(CC) sts.c -o sts file.o unix_sock.o dev_tap.o utils.o netdev.o

client :client.c unix_sock.o
	$(CC) client.c -o client unix_sock.o

file.o: file.c
	$(CC) -c file.c

unix_sock.o: unix_sock.c
	$(CC) -c unix_sock.c

dev_tap.o: dev_tap.c
	$(CC) -c dev_tap.c

utils.o: utils.c
	$(CC) -c utils.c

netdev.o: netdev.c
	$(CC) -c netdev.c

clean: sts file.o
	rm sts file.o
