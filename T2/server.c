#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <stddef.h>
#include <fcntl.h>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER  100
#define BUFSIZE 1024


static char* fileRead(char* path, int *nrbytes, int offset, int client);
static int fileWrite(char* path, char* payload, int nrbytes, int offset, int client);
static void fileInfo(char* path); // TODO: return type

static char* dirCreate(char* path, char* name, int client, char* ownerPerm, char* otherPerm);
static char* dirRemove(char* path, char* name);
static void dirList(char* path); // TODO: return type

static char* getDirectory();
static int filesFilter(const struct dirent* nameList);
static void error(char *msg);
static int fileExist (char* filename);

char* runCommand(char* command)
{
	char* params[8];

	int param_num = 0;
	for(int i = 0; (params[i] = strsep(&command, " ")) != NULL; i++, param_num++)
	{
		if(i==7)
			return "ERROR: to many parameters!";
	}

    printf("PARAM %d\n", param_num);
	if(!strcmp(params[0], "RD-REQ"))
	{
		if(param_num != 6)
			return "ERROR: wrong number of parameters";

		char* path = params[1];
		int len = atoi(params[2]);
		char* payload;
		int nrbytes = atoi(params[3]);
		int offset = atoi(params[4]);
		int client = atoi(params[5]);

		char* fullpath = (char*)malloc(BUFFER * sizeof(char));
		char lenPath[20];
		char bt[20];

		fullpath[0] = '\0';
		
		payload = fileRead(path, &nrbytes, offset, client);
		printf("Payload: %s\n", payload);

		if (payload == NULL)
		{
			printf("Error reading file\n");
			return NULL;
		}

		snprintf(lenPath, 20, "%lu", strlen(path));
		snprintf(bt, 20, "%d", nrbytes);

		strcpy(fullpath, "RD-REP ");
		strcat(fullpath, path);
		strcat(fullpath, " ");
		strcat(fullpath, lenPath);
		strcat(fullpath, " ");
		strcat(fullpath, payload);
		strcat(fullpath, " ");
		strcat(fullpath, bt);
		strcat(fullpath, " ");
		strcat(fullpath, params[4]);

		printf("Response: %s\n", fullpath);

		// path(string), strlen(int), payload (string), nrbytes lidos(int), offset igual ao do R-REQ(int)
		return fullpath;
	}
	
	if(!strcmp(params[0], "WR-REQ"))
	{
		if(param_num != 7)
			return "ERROR: wrong number of parameters";

		char* path = params[1];
		int len = atoi(params[2]);
		char* payload = params[3];
		int nrbytes = atoi(params[4]);
		int offset = atoi(params[5]);
		int client = atoi(params[6]);

		char* fullpath = (char*)malloc(BUFFER * sizeof(char));
		char lenPath[20];
		char bt[20];

		fullpath[0] = '\0';
		
		nrbytes = fileWrite(path, payload, nrbytes, offset, client);
		printf("Bytes Written Response: %d\n", nrbytes);

		if (nrbytes == -1)
		{
			printf("Error writing in file\n");
			return NULL;
		}

		snprintf(lenPath, 20, "%lu", strlen(path));
		snprintf(bt, 20, "%d", nrbytes);

		strcpy(fullpath, "WR-REP ");
		strcat(fullpath, path);
		strcat(fullpath, " ");
		strcat(fullpath, lenPath);
		strcat(fullpath, " ");
		strcat(fullpath, bt);
		strcat(fullpath, " ");
		strcat(fullpath, params[4]);

		printf("Response: %s\n", fullpath);
		
		// path (string), strlen(int), payload(string vazio), nrbytes escritos (int), offset igual ao do W-REQ(int)
		return fullpath;
	}
	
	if(!strcmp(params[0], "FI-REQ"))
	{
		char* path = params[1];

		fileInfo(path);
		
		// path (string), strlen(int), owner(int), permissions (2char), filelength (int)
		return NULL;
	}
	
	if(!strcmp(params[0], "DC-REQ")) // OK!
	{
		if(param_num != 5)
			return "ERROR: wrong number of parameters\n";

		char* path = params[1];
		char* name = params[3];

		char* fullpath = (char*)malloc(BUFFER * sizeof(char));
		char* answer;
		char  len[20];

		fullpath[0] = '\0';
		
		answer = dirCreate(path, name, 1, "W", "W");

		if(answer == NULL) {
			printf("Error creating directory\n");
			return "Error creating directory\n";
		}

		snprintf(len, 20, "%lu", strlen(answer));

		strcpy(fullpath, "DC-REP ");
		strcat(fullpath, answer);
		strcat(fullpath, " ");
		strcat(fullpath, len);
		
		// path(string), strlen(int)
		return fullpath;
	}

	if(!strcmp(params[0], "DR-REQ")) // OK!
	{
		if(param_num != 5)
			return "ERROR: wrong number of parameters";

		char* path = params[1];
		char* name = params[3];

		char* fullpath = (char*)malloc(BUFFER * sizeof(char));
		char* answer;
		char  len[20];

		fullpath[0] = '\0';

		answer = dirRemove(path, name);

		printf("Answer: %s\n", answer);

		if (answer == NULL)
		{
			snprintf(len, 20, "%d", 0);
		}
		else
		{
			snprintf(len, 20, "%lu", strlen(path));
		}

		printf("Tamanho do path: %s\n", len);
		
		strcpy(fullpath, "DR-REP ");
		strcat(fullpath, path);
		strcat(fullpath, " ");
		strcat(fullpath, len);

		// path(string), strlen(int)
		return fullpath;
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

static void runServer(int port)
{
    int sockfd; /* socket */
    int portno; /* port to listen on */
    unsigned int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
    
    char name[BUFSIZE];   // name of the file received from client
    int cmd;              // cmd received from client
    
    portno = port;
    
    /* 
     * socket: create the parent socket 
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    /* 
     * bind: associate the parent socket with a port 
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        error("ERROR on binding");

    /* 
     * main loop: wait for a datagram, then echo it
     */
    clientlen = sizeof(clientaddr);
    while (1) 
    {
        /*
         * recvfrom: receive a UDP datagram from a client
         */
        bzero(buf, BUFSIZE);
        n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
        if (n < 0)
            error("ERROR in recvfrom");
        
        /* 
         * gethostbyaddr: determine who sent the datagram
         */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
            error("ERROR on gethostbyaddr");

        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL)
            error("ERROR on inet_ntoa\n");
    
        printf("Server received datagram from %s (%s)\n",  hostp->h_name, hostaddrp);
        printf("Server received %lu/%d bytes: %s\n", strlen(buf), n, buf);

        char* reply;
        if( !(reply = runCommand(buf)) ) {
            reply = strdup("Error: could not understand command!");
        }
        printf("Reply: %s\n", reply);
        
        /* 
         * sendto: echo the input back to the client 
         */
        n = sendto(sockfd, reply, strlen(reply) + 1, 0, (struct sockaddr *) &clientaddr, clientlen);
        if (n < 0) 
            error("ERROR in sendto");
        printf("Answer sent\n");
    }
}


int main(int argc, char **argv)
{
	char* command;
	/*
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

    */


    /* 
     * check command line arguments 
     */
    if (argc != 2) 
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int portno = atoi(argv[1]);

    runServer(portno);

	return 0;
}

static char* fileRead(char* path, int* nrbytes, int offset, int client)
{
	FILE* control;
	char *payload = (char*)malloc(BUFSIZE * sizeof(char));
	int descriptor;
	int bytes;

	//Check client
	int clientId;
	char* pathdup = strdup(path);
	char* nameWithDot;
	char* name;
	char* aux;

	while((aux = strsep(&pathdup, "/")) != NULL) name = aux;

	nameWithDot = (char*)malloc((strlen(name)+2)*sizeof(char));
	strcpy(nameWithDot, ".");
	strcat(nameWithDot, name);

	control = fopen(nameWithDot, "rb");
	fscanf(control, "%d", &clientId);
	if (clientId != client)
	{
		fclose(control);
		*nrbytes = 0;
		return "Cliente nao autorizado";
	}
	fclose(control);
	//

	printf("fileRead -- path: %s, nrbytes: %d, offset: %d\n", path, *nrbytes, offset);

	descriptor = open(path, O_RDONLY);

	bytes = pread(descriptor, payload, *nrbytes, offset);

	*nrbytes = bytes;

	if (bytes == -1)
	{
		return NULL;
	}
	
	return payload;
}

static int fileWrite(char* path, char* payload, int nrbytes, int offset, int client)
{
	FILE* new;
	struct stat buf;
	int descriptor;
	int written;
	int size;

	//Client
	char* pathdup = strdup(path);
	char* nameWithDot;
	char* name;
	char* aux;
	int clientId;

	while((aux = strsep(&pathdup, "/")) != NULL) name = aux;
	nameWithDot = (char*)malloc((strlen(name)+2)*sizeof(char));
	strcpy(nameWithDot, ".");
	strcat(nameWithDot, name);
	//
	
	if (!fileExist(path))
	{
		printf("path: %s / aux: %s / name: %s\n", path, aux, name);
		new = fopen(path, "wb");
		fclose(new);
		new = fopen(nameWithDot, "wb");
		fprintf(new, "%d", client);
		fclose(new);
	}
	else
	{
		new = fopen(nameWithDot, "rb");
		fscanf(new, "%d", &clientId);
		if (clientId != client)
		{
			fclose(new);
			return 0;
		}
		fclose(new);
	}

	if (nrbytes == 0)
	{
		if (remove(path) == -1)
		{
			printf("Error deleting file: %s\n", path);
		}

		return 0;
	}

	stat(path, &buf);
	size = buf.st_size;

	offset = size < offset ? size : offset;

	printf("fileWrite -- path: %s, payload: %s, nrbytes: %d, offset: %d\n", path, payload, nrbytes, offset);

	descriptor = open(path, O_WRONLY);

	printf("Passed descriptor\n");

	written = pwrite(descriptor, payload, nrbytes, offset);

	printf("Bytes written: %d\n", written);
	
	return written;
}

static void fileInfo(char* path)
{
	struct stat sb;
	char* fullPath = getDirectory();

	printf("fileInfo -- path: %s\n", path);

	strcat(fullPath, path);

	stat(fullPath, &sb);

	return;
}

static char* dirCreate(char* path, char* name, int client, char* ownerPerm, char* otherPerm)
{
	struct stat st = {0};
	char* fullpath = (char*)malloc((strlen(path) + strlen(name)) * sizeof(char));
	char* authPath;
	char  buf[20];
	mode_t permissao = S_IRWXU | S_IROTH | S_IWOTH | S_IXOTH;
	int descriptor;

	printf("dirCreate -- path: %s, name: %s, fullpath: %s\n", path, name, fullpath);

	strcpy(fullpath, path);
	strcat(fullpath, "/");
	strcat(fullpath, name);

	authPath = strdup(fullpath);
	strcat(authPath, "/.");
	strcat(authPath, name);

	if (stat(fullpath, &st) == -1) 
	{
		if (mkdir(fullpath, permissao) != 0)
		{
			printf("Error in mkdir\n");
			return NULL;
		}
	}
	else 
	{
		printf("Stat error\n");
		return NULL;
	}

	snprintf(buf, 20, "%d", client);
	strcat(buf, " ");
	strcat(buf, ownerPerm);
	strcat(buf, " ");
	strcat(buf, otherPerm);

	printf("Buffer: %s\n", buf);

	descriptor = open(authPath, O_WRONLY | O_CREAT);
	pwrite(descriptor, buf, 6, 0);

	return fullpath;
}

static char* dirRemove(char* path, char* name)
{
	char* fullpath = getDirectory();

	printf("dirRemove -- path: %s, name: %s\n", path, name);

	strcpy(fullpath, path);
	strcat(fullpath, "/");
	strcat(fullpath, name);

	if (rmdir(fullpath) == -1)
	{
		return NULL;
	}

	return fullpath;
}

static void dirList(char* path)
{
	char* fullpath = getDirectory();
	int count;
	struct dirent** nameList;

	printf("dirList -- path: %s\n", path);

	strcat(fullpath, path);

	count = scandir(fullpath, &nameList, filesFilter, alphasort);
	//quantidade de arquivos no diretorio

}

static char* getDirectory()
{
	char* currentDir = (char*)malloc(BUFFER*sizeof(char)) ;

	if (getwd(currentDir) == NULL )
	{
		printf("Error getting path\n"); 
		exit(0);
	}

	return currentDir;
}

static int filesFilter(const struct dirent* nameList)
{
	if ((nameList->d_type == DT_DIR) || (strcmp(nameList->d_name, ".") == 0) || (strcmp(nameList->d_name, "..") == 0) )  
		return 0; 
	else
		return 1;
}

static void error(char *msg)
{
    perror(msg);
    exit(1);
}

static int fileExist (char* filename)
{
  struct stat buffer;   
  return stat(filename, &buffer) == 0;
}
