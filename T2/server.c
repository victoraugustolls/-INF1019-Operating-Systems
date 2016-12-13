#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER 100


static char* fileRead(char* path, int nrbytes, int offset);
static int fileWrite(char* path, char* payload, int nrbytes, int offset);
static void fileInfo(char* path); // TODO: return type

static char* dirCreate(char* path, char* name);
static char* dirRemove(char* path, char* name);
static void dirList(char* path); // TODO: return type


char* runCommand(char* command)
{
    char* params[6];
    
    for(int i = 0; (params[i] = strsep(&command, " ")) != NULL; i++);
    
    if(!strcmp(params[0], "RD-REQ"))
    {
        char* path = params[1];
        int len = atoi(params[2]);
        char payload[BUFFER];
        int nrbytes = atoi(params[3]);
        int offset = atoi(params[4]);
        
        fileRead(path, nrbytes, offset);

        // path(string), strlen(int), payload (string), nrbytes lidos(int), offset igual ao do R-REQ(int)
        return NULL;
    }
    
    if(!strcmp(params[0], "WR-REQ"))
    {
        char* path = params[1];
        int len = atoi(params[2]);
        char* payload = params[3];
        int nrbytes = atoi(params[4]);
        int offset = atoi(params[5]);
        
        fileWrite(path, payload, nrbytes, offset);
        
        // path (string), strlen(int), payload(string vazio), nrbytes escritos (int), offset igual ao do W-REQ(int)
        return NULL;
    }
    
    if(!strcmp(params[0], "FI-REQ"))
    {
        char* path = params[1];

        fileInfo(path);
        
        // path (string), strlen(int), owner(int), permissions (2char), filelength (int)
        return NULL;
    }
    
    if(!strcmp(params[0], "DC-REQ"))
    {
        char* path = params[1];
        char* name = params[3];
        char* fullpath = (char*)malloc(BUFFER * sizeof(char));
        char* answer;
        char  len[20];

        fullpath[0] = '\0';
        
        answer = dirCreate(path, name);
        if(answer == NULL) {
            printf("Error creating directory\n");
            return NULL;
        }

        snprintf(len, 20, "%lu", strlen(answer));

        strcpy(fullpath, "DC-REP ");
        strcat(fullpath, answer);
        strcat(fullpath, " ");
        strcat(fullpath, len);
        
        // path(string), strlen(int)
        return fullpath;
    }

    if(!strcmp(params[0], "DR-REQ"))
    {
        char* path = params[1];
        char* name = params[3];

        dirRemove(path, name);
        
        // path(string), strlen(int)
        return NULL;
    }

    if(!strcmp(params[0], "DL-REQ"))
    {
        char* path = params[1];
        
        dirList(path);
        
        // allfilenames (string), fstlstpositions (array[40] of struct {int, int}), nrnames (int)
        return NULL;
    }
    
    printf("UNRECOGNIZED COMMAND!!\n");
    return NULL;
}


int main (void)
{
    char* command;
    
    // path(string), strlen(int), payload (string vazio), nrbytes(int), offset(int)
    command = strdup("RD-REQ path/to/file 5 5 0");
    runCommand(command);
    free(command);
    
    // path(string), strlen(int), payload(string), nrbytes(int), offset(int)
    command = strdup("WR-REQ path/to/file 5 loadpay 5 0");
    runCommand(command);
    free(command);
    
    
    // path (string), strlen(int)
    command = strdup("FI-REQ path/to/file 5");
    runCommand(command);
    free(command);

    
    // path(string), strlen(int), dirname(string), strlen(int)
    command = strdup("DC-REQ . 5 newDir 5");
    runCommand(command);
    free(command);

    
    // path (string), strlen(int), dirname(string), strlen(int)
    command = strdup("DR-REQ path/to/file 5 killDir 5");
    runCommand(command);
    free(command);

    
    // path(string), strlen(int)
    command = strdup("DL-REQ path/to/file 5");
    runCommand(command);
    free(command);

    return 0;
}


static char* fileRead(char* path, int nrbytes, int offset)
{
    printf("fileRead -- path: %s, nrbytes: %d, offset: %d\n", path, nrbytes, offset);
    
    return strdup("fileContent");
}

static int fileWrite(char* path, char* payload, int nrbytes, int offset)
{
    printf("fileWrite -- path: %s, payload: %s, nrbytes: %d,  offset: %d\n", path, payload, nrbytes, offset);
    
    return 0;
}


static void fileInfo(char* path)
{
    printf("fileInfo -- path: %s\n", path);
}



static char* dirCreate(char* path, char* name)
{
    struct stat st = {0};
    char* fullpath = (char*)malloc((strlen(path) + strlen(name)) * sizeof(char));
    strcpy(fullpath, path);
    strcat(fullpath, "/");
    strcat(fullpath, name);

    printf("dirCreate -- path: %s, name: %s, fullpath: %s\n", path, name, fullpath);

    if (stat(fullpath, &st) == -1) {
        if (mkdir(fullpath, 0700) != 0)
        {
            printf("Error in mkdir\n");
            return NULL;
        }
    }
    else {
        printf("Stat error\n");
        return NULL;
    }

    return fullpath;
}

static char* dirRemove(char* path, char* name)
{
    printf("dirRemove -- path: %s, name: %s\n", path, name);

    return NULL;
}

static void dirList(char* path)
{
    printf("dirList -- path: %s\n", path);
}

