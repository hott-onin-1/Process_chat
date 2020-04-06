#include "definitions.h"
#include "utilities.h"

void search_and_clean(char str[], int len)
{
    int i=0;
    while(i < len-2 && (str[i] != '\n' && str[i] != '\0') ) i++;
    str[i] = '\n';
    str[i+1] = '\0';
}

void check_error(const int err, const char str[])
{
    if(err == -1)
    {
        perror(str);

        exit(EXIT_FAILURE);
    }
}

void display_intro(void)
{
    system("clear");
    printf("\n=====================================================\n\n");
    printf("Welcome in Process-Chat 0.1\n");
    printf("Use Ctrl+C to quit\n");
    printf("\n=====================================================\n\n");
}


void my_close(void)
{
    int err;
    err = close(remote.pipe_fd);
    check_error(err, "close pdf_r");
    err = close(local.pipe_fd);
    check_error(err, "close pdf_w");
}

void my_clean(void)
{
    int err;
    char remove[100];
    sprintf(remove, "rm %s", local.file_name);
    err = system(remove);
    check_error(err, "remove");
    printf("Terminated!\n");
    exit(EXIT_SUCCESS);
}

int find_file(char pipe_name[], int number_pos)
{
    for(int i=0; i < 10; i++)
    {
        pipe_name[number_pos]='0'+i;

        if(access(pipe_name, F_OK) == -1)
        {
            return i;
        }
    }
    return -1;
}

void stdin_flush(void)
{
    int c;
    do
    {
        c = getchar();
    } while(c != '\n' && c != EOF);
}