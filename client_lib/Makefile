CC = gcc
CFLAGS = -I ../

libsts: libsts.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -shared -o libsts.so libsts.c -ldl

.PHONY:
clean:
	rm *.so
