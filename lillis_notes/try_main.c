#include "../minishell.h"



void run_loop()
{
    char *line;
    
    // while (1)
    // {
        printf("prompt$");
    // }
}

int main()
{
    //
    printf("starting...\n");

    if (isatty(STDIN_FILENO) == 1) 
    {
        run_loop();
    }
    else
        printf("stdin not a terminal\n");
    return (0);
}