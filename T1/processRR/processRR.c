#include <sys/types.h>
#include <unistd.h>
#include "process.h"

typedef struct processRR {

    pid_t pid;

} tpProcessRR ;

void createProcessRR(tppProcessRR * pProcessRR, pid_t pid) {

    tppProcessRR newProcess = NULL;
    newProcess = (tpProcessRR *)malloc(sizeof(tpProcessRR));

    if (newProcess == NULL) {
        exit(1);
    }
    newProcess->pid = pid;

    *pProcessRR = newProcess;
}

pid_t getProcessRRPid(tppProcessRR pProcessRR) {
    return pProcessRR->pid;
}
