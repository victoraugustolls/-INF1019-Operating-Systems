struct processRR {
    pid_t pid;
};
typedef struct processRR ProcessRR;

void enqueue(ProcessRR x);
void dequeue();
ProcessRR front();
void print();
