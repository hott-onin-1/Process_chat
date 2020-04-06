/*
===============================================================

                         Process Chat
Inter-process chat application for Operating Systems classes

                           ECE Paris
                          April 2020

===============================================================
*/

/**
 * \file utilities.c
 * \brief Process Chat application
 * \version 0.2
 * \date April 2020
*/

/*
File: utilities.c
Contains the utilities functions
*/

#include "definitions.h"
#include "utilities.h"

// Parse a string and terminate it properly with \n and \0
void search_and_clean(char str[], int len)
{
    int i=0;

    // Go through string
    while(i < len-2 && (str[i] != '\n' && str[i] != '\0') ) i++;

    // Terminate it properly
    str[i] = '\n';
    str[i+1] = '\0';
}

// If err is -1, display error and terminate
void check_error(const int err, const char str[])
{
    if(err == -1)
    {
        perror(str);
        terminate(0);
    }
}

// Display intro message
void display_intro(void)
{
    system("clear");
    printf("\n=====================================================\n\n");
    printf("Welcome in Process-Chat 0.2\n");
    printf("Use Ctrl+C to quit\n");
    printf("\n=====================================================\n\n");
}

// Close pipe file descriptors
void my_close(void)
{
    int err;

    // If active
    if(remote.pipe_fd != -1)
    {
        err = close(remote.pipe_fd);                    // Close
        if(err == -1) perror("close remote.pipe_fd");   // Error
    }

    // If active
    if(local.pipe_fd != -1)
    {
        err = close(local.pipe_fd);                     // Close
        if(err == -1) perror("close local.pipe_fd");    // Error
    }
}

// Clean fifo file
void my_clean(void)
{
    int err;
    char remove[100];

    // If file exist
    if(access(local.file_name, F_OK) != -1)
    {
        sprintf(remove, "rm %s", local.file_name);  // Create command string
        err = system(remove);                       // Execute command string
        if(err == -1) perror("remove");             // Error
    }
}

// Search for a free name for fifo file
int find_file(char pipe_name[], int number_pos)
{
    for(int i=0; i < 10; i++)               // 10 possibilities
    {
        pipe_name[number_pos]='0'+i;        // Iterating through names

        if(access(pipe_name, F_OK) == -1)   // If file doesn't exist yet, name is free
        {
            return i;                       // Return id
        }
    }
    return -1;                              // Else error
}

// Clean stdin buffer
void stdin_flush(void)
{
    int c;
    do
    {
        c = getchar();                      // Go through chars
    } while(c != '\n' && c != EOF);         // Until we dounf end of stdin character
}

// Terminate
void terminate(int sucess)
{
    state = CLOSING;                // Update state
    printf("\nTerminating...\n");
    my_close();                     // Close pipes
    usleep(500);                    // Waiting for remote to close
    my_clean();                     // Clean fifo file
    printf("Terminated!\n");
    exit(sucess? EXIT_SUCCESS : EXIT_FAILURE);
}