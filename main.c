/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file main.c
 * \brief Main functions
 * \version 1.0
 * \date April 2020
*/

/*
File: main.c
Contain the main of the program
*/

// Local includes
#include "definitions.h"
#include "utilities.h"
#include "core.h"

// Global variables:
t_client local, remote;
t_state state;

int main(void)
{
    // Local variables
    int err;    // Used to store err codes returned by functions

    // Make sure stdin is in NONBLOCKING mode, so we can read it correctly
    err = fcntl(STDIN_FILENO, F_SETFL, ~O_NONBLOCK);
    check_error(err, "fcntl");

    // Initialize file names variables
    sprintf(local.file_name, FILE_PATH);
    sprintf(remote.file_name, FILE_PATH);

    // Initialize pipe variables
    remote.pipe_fd = -1;
    local.pipe_fd = -1;

    // Initialize state and pid
    state = INIT;
    local.pid = getpid();

    // Set up signal handlers
    signal(SIGUSR1, handle_signals);
    signal(SIGINT, handle_signals);

    // Display intro
    display_intro();

    // Retreive available ID (= retreive available file name)
    local.id = find_file(local.file_name, X_POS);
    if(local.id == -1)
    {
        fprintf(stderr, "find_file: Couldn't create fifo pipe file\n");
        printf("\nTerminated!\n");
        exit(EXIT_FAILURE);
    }

    // Update file name with correct ID
    local.file_name[X_POS] = '0' + local.id;

    // Create fifo file
    err = mkfifo(local.file_name, 0666);
    check_error(err, "mkfifo");

    // Print local ID
    printf("Local ID is \"%d\"\n", local.id);

    // Ask user for remote ID
    remote.id = ask_id(local.id);

    // Update file name with correct ID
    remote.file_name[X_POS] = '0' + remote.id;

    // Debugging
    // fprintf(stderr, "Remote file name: *%s*\n", remote.file_name);
    // fprintf(stderr, "Local file name: *%s*\n", local.file_name);

    // Etablish connexion between remote and local
    etablish_connexion(&remote, &local);

    // Ready to chat
    printf("\nLet's chat!\n\n");

    // Make stdin and reading pipe Non blocking
    fcntl(remote.pipe_fd, F_SETFL, O_NONBLOCK);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    // Update state
    state = RUNNING;

    // Launch chatting
    chat_loop(&remote, &local);

    // We should never reach this, termination is handled by signals
    fprintf(stderr, "Unexpected: Reached end\n");
    terminate(FAILURE);

    return 1;
}
