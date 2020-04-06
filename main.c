#include "definitions.h"
#include "utilities.h"
#include "core.h"

// Global variables:
t_client local, remote;
t_state state;

int main(void)
{
    int err, tmp;
    char buf[2];

    err = fcntl(STDIN_FILENO, F_SETFL, ~O_NONBLOCK);
    check_error(err, "fcntl");

    sprintf(local.file_name, FILE_PATH);
    sprintf(remote.file_name, FILE_PATH);

    remote.pipe_fd = -1;
    local.pipe_fd = -1;

    state = INIT;
    local.pid = getpid();

    signal(SIGUSR1, handle_signals);
    signal(SIGINT, handle_signals);

    display_intro();

    local.id = find_file(local.file_name, X_POS);
    if(local.id == -1)
    {
        fprintf(stderr, "find_file: Couldn't create fifo pipe file\n");
        printf("\nTerminated!\n");
        exit(EXIT_FAILURE);
    }

    local.file_name[X_POS] = '0' + local.id;

    err = mkfifo(local.file_name, 0666);
    check_error(err, "mkfifo");

    printf("Local id is \"%d\"\n", local.id);

    remote.id = -1;
    while(remote.id == -1)
    {
        printf("Id of remote client (0 -> 9)?\n");

        err = read(STDIN_FILENO, buf, sizeof(buf));
        check_error(err, "read stdin");

        if(buf[1] != '\n')
        {
            stdin_flush();
            fprintf(stderr, "\nInvalid id!\n");
        }
        else if(buf[0]>='0' && buf[0]<='9')
        {
            tmp = buf[0] - '0';
            if(tmp == local.id) fprintf(stderr, "\nInvalid id: Remote client can not be local client!\n");
            else remote.id = tmp;
        }
        else
        {
            fprintf(stderr, "\nInvalid id!\n");
        }
    }
    printf("\n");

    remote.file_name[X_POS] = '0' + remote.id;
    fprintf(stderr, "Remote file name: *%s*\n", remote.file_name);
    fprintf(stderr, "Local file name: *%s*\n", local.file_name);

    if(remote.id > local.id)
    {
        printf("Waiting for a connexion...\n");
        remote.pipe_fd = open(remote.file_name, O_RDONLY); // Blocking call
        check_error(remote.pipe_fd, "open remote");

        printf("Setting connexion...\n");
        local.pipe_fd = open(local.file_name, O_WRONLY); // Should not block
        check_error(local.pipe_fd, "open local");

        printf("Authentification...\n");
        err = write(local.pipe_fd, &(local.pid), sizeof(int));
        check_error(err, "write");

        err = read(remote.pipe_fd, &(remote.pid), sizeof(int));
        check_error(err, "read");
    }
    else
    {
        printf("Waiting for a connexion...\n");
        local.pipe_fd = open(local.file_name, O_WRONLY); // Blocking call
        check_error(local.pipe_fd, "open local");

        printf("Setting connexion...\n");
        remote.pipe_fd = open(remote.file_name, O_RDONLY); // Should not block
        check_error(remote.pipe_fd, "open remote");

        printf("Authentification...\n");
        err = read(remote.pipe_fd, &(remote.pid), sizeof(int));
        check_error(err, "read");

        err = write(local.pipe_fd, &(local.pid), sizeof(int));
        check_error(err, "write");
    }

    printf("Local pid is \"%d\"\n", local.pid);
    printf("Remote pid is \"%d\"\n", remote.pid);
    printf("Connected!\n"); // Success

    printf("\nLet's chat!\n\n");

    fcntl(remote.pipe_fd, F_SETFL, O_NONBLOCK);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    state = RUNNING;
    char receive[MSG_SIZE];
    char send[MSG_SIZE];

    // Leaving the program is handled with signals
    // Hence the infinite loop (while(1))
    while(1)
    {
        // Reading from remote
        err = read(remote.pipe_fd, receive, MSG_SIZE-2);

        // If something is received:
        if(err!=-1)
        {
            search_and_clean(receive, MSG_SIZE); // Clean message
            printf("Received > %s\n", receive); // Display it
        }

        // Reading from stdin (user)
        err = read(STDIN_FILENO, send, MSG_SIZE-2);

        // If something is written:
        if(err!=-1 && send[0]!='\n')
        {
            search_and_clean(send, MSG_SIZE); // Clean message
            err = write(local.pipe_fd, send, MSG_SIZE-2); // Write it
            printf("Sent > %s\n", send); // Display what we sent
        }

        usleep(POLLING_PERIOD);
    }

    fprintf(stderr, "Unexpected: Reached end\n");
    terminate(0);
}