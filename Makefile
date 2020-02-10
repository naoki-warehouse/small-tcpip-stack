CC=gcc

sts: sts.c file.o unix_sock.o dev_tap.o utils.o netdev.o my_sock.o socket_manager.o client
	$(CC) sts.c -o sts file.o unix_sock.o dev_tap.o utils.o netdev.o socket_manager.o -lpthread

client :client.c my_sock.o unix_sock.o
	$(CC) client.c -o client my_sock.o unix_sock.o

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

my_sock.o: my_sock.c
	$(CC) -c my_sock.c

socket_manager.o: socket_manager.c
	$(CC) -c socket_manager.c

clean:
	rm sts file.o unix_sock.o dev_tap.o utils.o netdev.o my_sock.o socket_manager.o
