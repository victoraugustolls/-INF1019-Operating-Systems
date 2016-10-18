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

#define PROGRAM_FILE "commands.txt"

#define INFO_FLAG_KEY 8672
#define TYPE_KEY 8673
#define PRIORITY_KEY 8674
#define PATH_KEY 8675
#define END_KEY 8676

int main(int argc, char const *argv[])
{

    FILE *commandsFile;
    char priority[2];
    char programPath[20];
	// int status;
    int j = 0;
    int shouldRewind = 1;
    int type = 0;
	// pid_t schedulerPid;

    // Shared memory variables
    int endSegment, newInfoSegment, pathSegment, prioritySegment, typeSegment;
    char pathBuffer[BUFFER_SIZE][PATH_SIZE];
    char priorityBuffer[BUFFER_SIZE][2];
	int *end, *priorityValue, *type, *newInfo;

    // Shared memory access
    newInfoSegment = shmget(INFO_FLAG_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if(newInfoSegment < 0)
    {
        printf("Error when creating newInfoSegment\n");
        exit(1);
    }
    newInfo = (int *)shmat(newInfoSegment, 0, 0);
    *newInfo = 0;

    endSegment = shmget(END_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if(endSegment < 0)
    {
        printf("Error when creating endSegment\n");
        exit(1);
    }
    end = (int *)shmat(endSegment, 0, 0);
    *end = 0;

    prioritySegment = shmget(PRIORITY_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if(prioritySegment < 0)
    {
        printf("Error when creating prioritySegment\n");
        exit(1);
    }
    priorityValue = (int *)shmat(prioritySegment, 0, 0);
    *priorityValue = 0;

    typeSegment = shmget(TYPE_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    if(typeSegment < 0)
    {
        printf("Error when creating typeSegment\n");
        exit(1);
    }
    type = (int *)shmat(typeSegment, 0, 0);
    *type = 0;

    //Read file
    commandsFile = fopen(PROGRAM_FILE, "r");
    if (commandsFile == NULL)
    {
        printf("Failed to open commands file\n");
        exit(1);
    }
    while (fscanf(commandsFile, "exec %s prioridade=%s\n", programPath, priority) == 2) //Priority
    {
        printf("Type: 0 | Path: %s | Priority: %s\n", programPath, priority);

        strcpy(pathBuffer[j], programPath);
        strcpy(priorityBuffer[j], priority);
        j++;

        shouldRewind = 0;
        type = 0;
    }
    if (shouldRewind)
    {
        j = 0;
        rewind(commandsFile);
        while (fscanf(commandsFile, "exec %s\n", programPath) == 1) //Round Robin
        {
            printf("Type: 1 | Path: %s\n", programPath);
            strcpy(pathBuffer[j], programPath);
            j++;
            type = 1;
        }
    }
    //

    for (int i = 0; i < j; i++)
    {
        printf("%s\n", pathBuffer[i]);
        printf("%s\n", priorityBuffer[i]);
    }

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
            
        }

    }


    fclose(commandsFile);

	// Libera memoria compartilhada do processo
	shmdt(type);
	shmdt(priorityValue);
	shmdt(newInfo);
	shmdt(end);

	// Libera memoria compartilhada
	shmctl(typeSegment, IPC_RMID, 0);
	shmctl(prioritySegment, IPC_RMID, 0);
	shmctl(newInfoSegment, IPC_RMID, 0);
	shmctl(endSegment, IPC_RMID, 0);

    return 0;
}
