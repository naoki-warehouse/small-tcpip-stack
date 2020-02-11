CC = gcc
CFLAGS = -O3

src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
sts_obj = $(filter-out client.o, $(obj))
headers = $(wildcard *.h)

sts: $(sts_obj) client
	$(CC) $(CFLAGS) $(sts_obj) -o sts -lpthread

client :client.o my_sock.o unix_sock.o
	$(CC) $(CFLAGS) client.o my_sock.o unix_sock.o -o client

%.o: %.c ${headers}
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm sts client *.o
