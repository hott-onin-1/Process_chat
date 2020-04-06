/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file core.c
 * \brief Core fuctions
 * \version 1.0
 * \date April 2020
*/

/*
File: core.c
Contains the core functions
*/

// Local includes
#include "definitions.h"
#include "utilities.h"

// Handle signals SIGINT and SIGUSR1 received by the process
void handle_signals(int sig)
{
    // Which signal was received?
    switch(sig)
    {
        case SIGUSR1:
        {
            // Check state
            switch(state)
            {
                case CLOSING:
                return;             // Nothing to do if program is already closing

                case INIT:
                fprintf(stderr, "Received SIGUSR1 in unexpected state!\n");
                terminate(FAILURE); // Can't receive SIGUSR1 if connexion is not etablished

                case RUNNING:
                printf("Remote client asked for termination!\n");
                terminate(SUCCESS); // Remote client sent termination signal with SUGUSR1, terminating

                default:
                fprintf(stderr, "handle_signals: Unknow state!\n");
                terminate(FAILURE); // This should never be reached
            }
        }
        break;

        case SIGINT:
        {
            // Check state
            switch(state)
            {
                case CLOSING:
                return;                 // Nothing to do if program is already closing

                case INIT:
                terminate(SUCCESS);     // Closing success

                case RUNNING:
                terminate(SEND_TERM);   // Closing, success and send termination signal to remote

                default:
                fprintf(stderr, "handle_signals: Unknow state!\n");
                terminate(FAILURE);     // This should never be reached
            }
        }
        break;

        default:
        {
            fprintf(stderr, "handle_signals: Received unknown signal!\n");
            terminate(FAILURE);         // This should never be reached
        }
    }
}

// Ask the user for remote ID
int ask_id(int forbidden_id)
{
    // Variables for temporary data:
    int tmp;
    char buf[2];

    // Variable for the ID
    int id = -1;

    // If remote is not launched, ther will be an error during connexion
    printf("Make sure remote client is launched, and enter its ID\n");

    // While no valid ID
    while(id == -1)
    {
        // Ask for ID
        printf("ID of remote client (0 -> 9)?\n");

        // read 2 characters of stdin
        tmp = read(STDIN_FILENO, buf, sizeof(buf));
        check_error(tmp, "read stdin");

        // Take first char as ID
        tmp = buf[0] - '0';

        // If second char is line return, user inputed one char only
        if(buf[1] != '\n')
        {
            stdin_flush();                      // If not, we clean stdin
            fprintf(stderr, "\nInvalid ID!\n"); // And ID is obviously not valid (must be one digit)
        }
        else if(tmp >=0 && tmp <= 9)            // If ID is a valid digit
        {
            // If ID is the forbidden one
            if(tmp == forbidden_id) fprintf(stderr, "\nInvalid ID: Remote client can not be local client!\n");
            else id = tmp;                      // else ID is validated
        }
        else                                    // ID is not a valid digit
        {
            fprintf(stderr, "\nInvalid ID!\n");
        }
    }
    printf("\n");

    // Return valid id digit
    return id;
}

// Etablish connexion between remote and local
void etablish_connexion(t_client * remote, t_client * local)
{
    // Variable for error storage
    int err;

    // If both clients try to connect the same way, deadlock.
    // So client with the lowest ID read first
    if(remote->id > local->id)
    {
        // Open read pipe
        printf("Waiting for a connexion...\n");
        remote->pipe_fd = open(remote->file_name, O_RDONLY);  // Blocking call
        if(remote->pipe_fd == -1)
        {
            fprintf(stderr, "ERROR: Make sure that remote is launched before trying to connect!\n");
            check_error(-1, "open remote");
        }

        // Open write pipe
        printf("Setting connexion...\n");
        local->pipe_fd = open(local->file_name, O_WRONLY);    // Should not block
        check_error(local->pipe_fd, "open local");

        // Exchange pid with remote
        // Send local pid
        printf("Authentification...\n");
        err = write(local->pipe_fd, &(local->pid), sizeof(int));
        check_error(err, "write");

        // Read remote pid
        err = read(remote->pipe_fd, &(remote->pid), sizeof(int));
        check_error(err, "read");
    }
    else    // Client with the biggest ID write first
    {
        // Open write pipe
        printf("Waiting for a connexion...\n");
        local->pipe_fd = open(local->file_name, O_WRONLY);    // Blocking call
        check_error(local->pipe_fd, "open local");

        // Open read pipe
        printf("Setting connexion...\n");
        remote->pipe_fd = open(remote->file_name, O_RDONLY);  // Should not block
        check_error(remote->pipe_fd, "open remote");

        // Exchange pid with remote
        // Read remote pid
        printf("Authentification...\n");
        err = read(remote->pipe_fd, &(remote->pid), sizeof(int));
        check_error(err, "read");

        // Send local pid
        err = write(local->pipe_fd, &(local->pid), sizeof(int));
        check_error(err, "write");
    }

    // Connexion succesfull if we reach this
    printf("Local pid is \"%d\"\n", local->pid);
    printf("Remote pid is \"%d\"\n", remote->pid);
    printf("Connected!\n");
}

// Handle the chatting between client and remote
void chat_loop(t_client * remote, t_client * local)
{
    // Variable for temporary results
    int result;

    // Buffer vraiables for messages
    char receive[MSG_SIZE];
    char send[MSG_SIZE];

    // Leaving the program is handled with signals
    // Hence the infinite loop (while(1))
    while(1)
    {
        // Reading and writing on MSG_SIZE-2 bytes because last two will be \n and \0
        // Reading from remote
        result = read(remote->pipe_fd, receive, MSG_SIZE-2);

        // If something is received:
        if(result!=-1)
        {
            search_and_clean(receive, MSG_SIZE);    // Clean message
            printf("Received > %s\n", receive);     // Display it
        }

        // Reading from stdin (user)
        result = read(STDIN_FILENO, send, MSG_SIZE-2);

        // If something is written:
        if(result!=-1 && send[0]!='\n')
        {
            search_and_clean(send, MSG_SIZE);                   // Clean message
            result = write(local->pipe_fd, send, MSG_SIZE-2);    // Write it
            printf("Sent > %s\n", send);                        // Display what we sent
        }

        // Sleep for some time
        // So we don't use 100% of available CPU time (unecessary)
        usleep(POLLING_PERIOD);
    }
}
