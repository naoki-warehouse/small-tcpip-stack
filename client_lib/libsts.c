#define _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <dlfcn.h>
#include <sys/socket.h>


static int (*__start_main)(int (*main) (int, char * *, char * *), int argc, \
                           char * * ubp_av, void (*init) (void), void (*fini) (void), \
                           void (*rtld_fini) (void), void (* stack_end));

static int (*_socket)(int domain, int type, int protocol) = NULL;

int socket(int domain, int type, int protocol)
{
    return -1;
}

int __libc_start_main(int (*main) (int, char * *, char * *), int argc,
                      char * * ubp_av, void (*init) (void), void (*fini) (void),
                      void (*rtld_fini) (void), void (* stack_end))
{
    __start_main = dlsym(RTLD_NEXT, "__libc_start_main");
    _socket = dlsym(RTLD_NEXT, "socket");
    return __start_main(main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}

