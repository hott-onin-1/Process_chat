#include "definitions.h"
#include "utilities.h"

void handle_signals(int sig)
{
    switch(sig)
    {
        case SIGUSR1:
        {
            if(state == CLOSING) return;
            if(state != RUNNING)
            {
                fprintf(stderr, "Received SIGUSR1 in unexpected state!\n");
                exit(EXIT_FAILURE);
            }
            state = CLOSING;
            printf("\nRemote client asked for termination!\n");
            printf("Terminating...\n");
            my_close();
            sleep(1); // Waiting for remote to close
            my_clean();
        }
        break;

        case SIGINT:
        {
            switch(state)
            {
                case CLOSING:
                case NO_STOP:
                return;

                case RUNNING:
                state = CLOSING;
                printf("\n\nTerminating...\n");
                my_close();
                kill(remote.pid, SIGUSR1);
                sleep(1); // Waiting for remote to close
                my_clean(); // Will terminate, no break needed
                // break;

                case INIT:
                printf("\n\nTerminating...\n");
                my_clean(); // Will terminate, no break needed
                // break;

                default:
                fprintf(stderr, "handle_signals: Unknow state!\n");
                exit(EXIT_FAILURE);
            }
        }
        break;

        default:
        {
            fprintf(stderr, "handle_signals: Received unknown signal!\n");
            exit(EXIT_FAILURE);
        }
    }
}
