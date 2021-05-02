#ifndef __ptree
#define __ptree

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define CMDLINE_LEN 4096
#define PATH_LEN 256

typedef struct _processInfo{
    int PID;
    int level;
    int PPID;
    char status;
    char cmdline[CMDLINE_LEN];
} processInfo;

int isPidFolder(const struct dirent *entry);
int isProcess(const struct dirent *entry);
void findChild(char * PPID);
void parseCmdline(const char *pidPath, char* retCmdline);
void parseStat(const char *pidPath, char* retStatus, int* retPpid);
void printInfo(processInfo *pInfo, int indent);


#endif
