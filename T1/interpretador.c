#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

#define BUFFER_SIZE 200
#define PATH_SIZE 20
#define PRIORITY_SIZE 2

#define PROGRAM_FILE "commands.txt"

int main(int argc, char const *argv[])
{

    FILE *commandsFile;

    char priority[PRIORITY_SIZE];
    char programPath[PATH_SIZE];
    char *buffer[BUFFER_SIZE];

    int j = 1; //Buffer index
    int shouldRewind = 1;
    int type = 0;

	pid_t schedulerPid;

    //Read file
    commandsFile = fopen(PROGRAM_FILE, "r");
    if (commandsFile == NULL)
    {
        printf("Failed to open commands file\n");
        exit(1);
    }
    while (fscanf(commandsFile, "exec %s prioridade=%s\n", programPath, priority) == 2) //Priority
    {
        printf("Type: 0 | Path: %s | Priority: %s | J: %d\n", programPath, priority, j);
        buffer[j] = (char*)malloc(strlen(programPath)+1);
        strcpy(buffer[j], programPath);
        j++;
        buffer[j] = (char*)malloc(strlen(priority)+1);
        strcpy(buffer[j], priority);
        j++;

        shouldRewind = 0;
        type = 0;
    }
    if (shouldRewind)
    {
        rewind(commandsFile);
        j = 1; //Reinicia index do buffer
        while (fscanf(commandsFile, "exec %s\n", programPath) == 1) //Round Robin
        {
            printf("Type: 1 | Path: %s | J: %d\n", programPath, j);
            buffer[j] = (char*)malloc(strlen(programPath)+1);
            strcpy(buffer[j], programPath);
            j++;
            type = 1;
        }
    }
    //

    for (int i = 0; i < j; i++)
    {
        printf("%s\n", buffer[i]);
    }
    buffer[j] = NULL;

    if (type)
    {
        // Start scheduler
        schedulerPid = fork();
        if (schedulerPid < 0)
        {
            printf("Error in fork!\n");
            exit(1);
        }
        else if (schedulerPid == 0)
        {
            buffer[0] = "SchedulerRR";
            execv("SchedulerRR", buffer);
        }

    }
    else {
        // Start scheduler
        schedulerPid = fork();
        if (schedulerPid < 0)
        {
            printf("Error in fork!\n");
            exit(1);
        }
        else if (schedulerPid == 0)
        {
            buffer[0] = "SchedulerPR";
            execv("SchedulerPR", buffer);
        }
    }

    waitpid(-1, NULL, 0);

    fclose(commandsFile);

    return 0;
}
