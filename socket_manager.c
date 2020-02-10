#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <pthread.h>

#include "socket_manager.h"
#include "my_sock.h"
#include "unix_sock.h"
#include "config.h"

struct socket_entry {
    int fd;
    pthread_t thread;
    struct socket_entry *pre;
    struct socket_entry *next;
};

struct socket_entry *socket_list_raw = NULL;

pthread_t listen_thread;

int socket_manager_init(){
    //Create Listen Thread here
    int ret = pthread_create(&listen_thread, NULL, socket_manager_listen_thread, NULL);
    if(ret != 0){
        fprintf(stderr, "Failed to create socket_manager listen_thread\n");
        return -1;
    }

}

struct rx_thread_arg {
    int fd;
    pthread_t thread;
};

void* socket_manager_listen_thread(void *arg){
    int listen_fd = unix_sock_open_server(sts_sock);
    if(listen_fd == -1) {
        fprintf(stderr, "Failed to open socket manager listen socket\n");
        return NULL;
    }
    fprintf(stdout, "Start to listen socket_manager...\n");
    while(1){
        struct sockaddr_un sun;
        int sock_len;
        struct rx_thread_arg *arg = malloc(sizeof(struct rx_thread_arg));

        arg->fd = accept(listen_fd, (struct sockaddr *)&sun, &sock_len);
        if(arg->fd == -1){
            fprintf(stderr, "Failed to accept unix domain socket %s\n", sts_sock);
            continue;
        }

        //Create RX Thread here
        int ret = pthread_create(&arg->thread, NULL, socket_manager_rx_thread, arg);
    }

}

void* socket_manager_rx_thread(void *arg){
    struct rx_thread_arg *args = (struct rx_thread_arg *)arg;
    int fd = args->fd;

    uint8_t recv_buf[CONFIG_SOCKET_MANAGER_BUF_SIZE] = {0}; 
    while(1){
        int size = recv(fd, recv_buf, sizeof(recv_buf), 0);
        if(size < 1){
            goto err;
        }
        printf("\n");

        struct my_sock_pkt_header *hdr = (struct my_sock_pkt_header *)recv_buf;

        if(hdr->pkt_type == MY_SOCK_PKT_SOCKET){
            if(hdr->op == MY_SOCK_PKT_OP_REQ){
                struct my_sock_pkt_socket_req *req_pkt = (struct my_sock_pkt_socket_req *)recv_buf;
                if(req_pkt->sock_type == MY_SOCK_PKT_TYPE_RAW){
                    fprintf(stdout, "Receive request to open raw socket\n");
                    struct socket_entry *entry = socket_manager_create_raw_socket(fd);
                    if(entry == NULL){
                        fprintf(stderr, "Failed to open raw socket\n");
                        goto err;
                    }
                    entry->thread = args->thread;
                    fprintf(stdout, "Open raw socket. fd:%d\n", entry->fd);

                    struct my_sock_pkt_socket_res res_pkt;
                    res_pkt.hdr.pkt_type = MY_SOCK_PKT_SOCKET;
                    res_pkt.hdr.op = MY_SOCK_PKT_OP_RES;
                    res_pkt.res = MY_SOCK_PKT_RES_OK;

                    send(entry->fd, &res_pkt, sizeof(res_pkt), 0);
                }else{
                    fprintf(stderr, "Unexpected socket type:%d\n", req_pkt->sock_type);
                    goto err;
                }
            }else{
                    fprintf(stderr, "Unexpected opcode:%d\n", hdr->op);
                    goto err;
            }
        }else{ 
            fprintf(stderr, "Unexpected packet type:%d\n", hdr->pkt_type);
            goto err;
        }
    }

err:
    fprintf(stdout, "Closing socket fd:%d...\n", fd);
    socket_manager_delete_raw_socket(fd);
    close(fd);
    free(arg);
    return NULL;
}

int socket_manager_tx_thread(){

}

struct socket_entry* socket_manager_create_raw_socket(int fd){
    struct socket_entry *tail = socket_list_raw;
    struct socket_entry *pre = NULL;

    if(tail != NULL){
        pre = socket_list_raw;
        while(tail->next != NULL){
            pre = tail;
            tail = tail->next;
        }
        tail->next = malloc(sizeof(struct socket_entry));
        tail = tail->next;
    }else{
        socket_list_raw = malloc(sizeof(struct socket_entry));
        tail = socket_list_raw;
    }

    if(tail == NULL){
        fprintf(stderr, "Failed to allocate socket_entry for raw socket\n");
        return NULL;
    }

    tail->fd = fd;
    tail->pre = pre;
    tail->next = NULL;
}

int socket_manager_delete_raw_socket(int fd){
    struct socket_entry *entry = socket_manager_get_raw_socket(fd);
    if(entry == NULL){
        return 0;
    }
    if(entry->pre == NULL){
        socket_list_raw = entry->next;
    }else{
        entry->pre->next = entry->next;
    }
    if(entry->next != NULL){
        entry->next->pre = entry->pre;
    }
    free(entry);
}

struct socket_entry* socket_manager_get_raw_socket(int fd){
    struct socket_entry *entry_now = socket_list_raw;
    while(entry_now != NULL){
        if(entry_now->fd == fd){
            return entry_now;
        }
        entry_now = entry_now->next;
    }
    return NULL;
}

void socket_manager_list_raw_socket(){
    struct socket_entry *entry_now = socket_list_raw;
    printf("RAW Socket List\n");
    while(entry_now != NULL){
        printf("thread:%08X fd:%d\n", entry_now->thread, entry_now->fd);
        entry_now = entry_now->next;
    }
}

int socket_manager_add_raw_packet(){

}

int socket_manager_get_raw_packet(){

}
