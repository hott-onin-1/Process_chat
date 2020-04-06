#include "definitions.h"
#include "utilities.h"

void handle_signals(int sig)
{
    switch(sig)
    {
        case SIGUSR1:
        {
            switch(state)
            {
                case CLOSING:
                return;

                case INIT:
                fprintf(stderr, "Received SIGUSR1 in unexpected state!\n");
                terminate(0);

                case RUNNING:
                printf("Remote client asked for termination!\n");
                terminate(1);

                default:
                fprintf(stderr, "handle_signals: Unknow state!\n");
                terminate(0);
            }
        }
        break;

        case SIGINT:
        {
            switch(state)
            {
                case CLOSING:
                return;

                case INIT:
                terminate(1);

                case RUNNING:
                kill(remote.pid, SIGUSR1);
                terminate(1);

                default:
                fprintf(stderr, "handle_signals: Unknow state!\n");
                terminate(0);
            }
        }
        break;

        default:
        {
            fprintf(stderr, "handle_signals: Received unknown signal!\n");
            terminate(0);
        }
    }
}
