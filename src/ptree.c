#include "ptree.h"

//Checks if directory is process directory
int isPidFolder(const struct dirent *entry) {
    for (const char *p = entry->d_name; *p; p++) {
        if (!isdigit((int)*p)){
            return 0;
        }
    }
    return 1;
}

void printInfo(processInfo *pInfo, int indent){
    printf("%6d %c ", pInfo->PID, pInfo->status);
    for (int i=0; i<indent; i++){
        printf(" ");
    }
    if (indent != 0)
        printf("* ");
    printf("%s\n", pInfo->cmdline);
}

//Parses /proc/PID/cmdline file
void parseCmdline(const char *pidPath, char* retCmdline){
    char _path[PATH_LEN];
    strcpy(_path, pidPath);
    strcat(_path, "/cmdline");

    int fd = open(_path, O_RDONLY);
    if (fd){
        int nbytesread = read(fd, retCmdline, CMDLINE_LEN);
        for (int i=0; i<nbytesread; i++){
            if (!retCmdline[i]){
                retCmdline[i] = ' ';
            }
        }
        close(fd);
    }
}

//Checks if process is has cmdline (not a thread)
int isProcess(const struct dirent *entry){
    char _path[PATH_LEN] = "/proc/";
    strcat(_path, entry->d_name);
    char cmdline[CMDLINE_LEN] = {0};
    parseCmdline(_path, cmdline);
    if (strlen(cmdline)){
        return 1;
    }else{
        return 0;
    }
}

//Parses /proc/PID/stat file in order to grab process status and PPID
void parseStat(const char *pidPath, char* retStatus, int* retPpid){
    char _path[PATH_LEN];
    strcpy(_path, pidPath);
    strcat(_path, "/stat");
    char c;
    long pos = 0;

    FILE *fh = fopen(_path, "r");
    if(fh){
        while(1)
        {
            c = fgetc(fh);
            if (c == EOF) break;
            if (c == ')') pos = ftell(fh);
        }
        fseek(fh, pos, 0);

        int rc = fscanf(fh, " %c %d", retStatus, retPpid);
        if (rc != 2){
            printf("ERROR. Can not parse /proc/PID/stat\n");
        }
        fclose(fh);
    }
}

void findChild(char * PPID){
    #define PROC_DIR "/proc/"
    static int indent = 0;
    struct dirent *dir;
    char _path[PATH_LEN] = PROC_DIR;
    DIR *d = opendir(_path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (isPidFolder(dir) && isProcess(dir)){
                processInfo pInfo = {0};
                strcat(_path, dir->d_name);
                parseStat(_path, &pInfo.status, &pInfo.PPID);

                if (pInfo.PPID == atoi(PPID)){
                    pInfo.PID = atoi(dir->d_name);
                    parseCmdline(_path, pInfo.cmdline);
                    printInfo(&pInfo, indent);
                    indent++;
                    findChild(dir->d_name);
                }
                strcpy(_path, PROC_DIR);
            }
        }
        closedir(d);
    }
    indent--;
}

