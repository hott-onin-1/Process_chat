/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file definitions.h
 * \brief Commun definitions
 * \version 1.0
 * \date April 2020
*/

/*
File: definitions.h
Contain the commun definitions
*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// External includes
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

// Defines
#define FILE_PATH "/tmp/file.fifo.X"    // File path for storing fifo files
#define X_POS 15                        // Postion of 'X' in the path file (will be replaced by a number)
#define MSG_SIZE 100                    // Max size of an individual message exchanged in the chat application (in bytes)
#define POLLING_PERIOD 100              // Polling period for sending/receiving messages, in microseconds

// State enumeration: describe the current state of the program
typedef enum State
{
    INIT,
    RUNNING,
    CLOSING
} t_state;

enum Context
{
    FAILURE = 0,
    SUCCESS = 1,
    SEND_TERM = 2
};

// Client structure: contains the data of a client
typedef struct Client
{
    char file_name[20];
    pid_t pid;
    int id;
    int pipe_fd;
} t_client;

extern t_state state;           // State

#endif // DEFINITIONS_H
