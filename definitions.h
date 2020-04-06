#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define FILE_PATH "/tmp/file.fifo.X"
#define X_POS 15
#define MSG_SIZE 100
#define POLLING_PERIOD 100

typedef enum State
{
    INIT,
    RUNNING,
    CLOSING
} t_state;

typedef struct Client
{
    char file_name[20];
    pid_t pid;
    int id;
    int pipe_fd;
} t_client;

extern t_client local, remote;
extern t_state state;

#endif // DEFINITIONS_H