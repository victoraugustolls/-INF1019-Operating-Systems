#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        
        return NULL;
    }
    
    if(!strcmp(params[0], "FI-REQ"))
    {
        char* path = params[1];

        fileInfo(path);
        
        return NULL;
    }
    
    if(!strcmp(params[0], "DC-REQ"))
    {
        char* path = params[1];
        char* name = params[3];
        
        dirCreate(path, name);
        
        return NULL;
    }

    if(!strcmp(params[0], "DR-REQ"))
    {
        char* path = params[1];
        char* name = params[3];

        dirRemove(path, name);
        
        return NULL;
    }

    if(!strcmp(params[0], "DL-REQ"))
    {
        char* path = params[1];
        
        dirList(path);
        
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
    command = strdup("DC-REQ path/to/file 5 newDir 5");
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
    printf("dirCreate -- path: %s, name: %s\n", path, name);

    return NULL;
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

