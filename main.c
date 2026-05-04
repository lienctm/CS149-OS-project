/**
 * CS149-Operating System
 * Project: File management
 * Author:
 * Date: May 2026
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#define MAX_NAME 30
#define MAX_CONTENTS 120
#define MAX_FILES 20
#define MAX_DIRS 10

#include <fcntl.h>     // open(), O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>    // close()
#include <stdio.h>     // perror()
#include <stdlib.h>    // exit()


typedef struct Directory Directory;

typedef struct File {
    char fileName[MAX_NAME];
    char contents[MAX_CONTENTS];
    bool isOpen;
} File;

typedef struct {
    int inFd;
    int outFd;
    char *inputFile;
    char *outputFile;
} FileHandles;

typedef struct Directory {
    char name[MAX_NAME];
    struct Directory *currentDir;
    struct Directory *subDir[MAX_DIRS];
    File files[MAX_FILES];
    int subDirCount;
    int fileCount;  
} Directory;

Directory *create_Directory(Directory *currentDir, char *name);
Directory *search_Directory(Directory *currentDir, char *name);
int search_index(Directory *current, char *name);
void create_files(Directory *current, char *name);
void open_files(Directory *current, char *name);
void close_files(Directory *current, char *name);
void search_files(Directory *current, char *name);
void free_Directory(Directory *dir);
Directory* cdcmd(Directory *current, char *name);

Directory *create_Directory (Directory *currentDir, char *name) {
    // allocating a new memory for new directory
    Directory *dir = malloc(sizeof(Directory));
    if (dir == NULL) {
        printf("Memory allocated fail.");
        exit(1);
    }
    // initialise directory members
    
    strcpy(dir->name, name);
    dir->currentDir = currentDir;
    dir->subDirCount = 0;
    dir->fileCount = 0;
    return dir;
}

Directory *search_Directory (Directory *currentDir, char *name) {
    for(int i = 0; i < currentDir->subDirCount; i++){
        if(strcmp(currentDir->subDir[i]->name, name) == 0) {
            return currentDir->subDir[i];
        }
    }
    return NULL;
}


void create_files(Directory *current, char *name) {
    if(current->fileCount >= MAX_FILES) {
        printf("Files limit exceeded.\n");
        return;
    }

    if(search_index(current, name) != -1) {
        printf("File already exist.\n");
        return;
    }
    
    strcpy(current->files[current->fileCount].fileName, name);
    current->fileCount++;
    printf("File is created successfully.\n");
}


void open_files(Directory *current, char *name) {
    int index = search_index(current, name);
    if (index == -1) {
        printf("Error: File not found.\n");
        return;
    }
    current->files[index].isOpen = true;
    printf("Opening virtual file: %s\n", name);
}

void close_files(Directory *current, char *name) {
    int index = search_index(current, name);
    if (index == -1) {
        printf("File not found.\n");
        return;
    }
    if(current->files[index].isOpen == false) {
        printf("Error: The file has not opened yet.\n");
    } else {
        printf("Closing virtual file: %s\n", name);
    }
}

void search_files(Directory *current, char *name) {
    if (current == NULL) return;

    int index = search_index(current, name);
    if (index != -1) {
        printf("Found: %s/%s\n", current->name, current->files[index].fileName);
        
    }
    else {
        printf("Find not found.\n");
        return;
    }

    for (int i = 0; i < current->subDirCount; i++) {
        if (current->subDir[i] != NULL) {
            search_files(current->subDir[i], name);
        }
    }
   
}
Directory* cdcmd(Directory *current, char *name) {
    // Standard notation for parent is ".."
    if (strcmp(name, "..") == 0) {
        if (current->currentDir != NULL) {
            printf("Moving up to: %s\n", current->currentDir->name);
            return current->currentDir;
        } else {
            printf("Already at root.\n");
            return current;
        }
    }

    // Move into a subdirectory
    Directory *dir = search_Directory(current, name);
    if (dir != NULL) {
        return dir;
    } else {
        printf("Directory '%s' not found.\n", name);
        return current;
    }
}

// This function looks at the position of a file in the array of files.
// Return index if file exist, otherwise -1
int search_index(Directory *current, char *name) {
    for(int i = 0; i < current->fileCount; i++) {
        if(strcmp(current->files[i].fileName, name) == 0) {
            return i;
        }
    }
    return -1;
}
void free_Directory(Directory *dir) {
    if (dir == NULL) return;

    // First, free all subdirectories recursively
    for (int i = 0; i < dir->subDirCount; i++) {
        free_Directory(dir->subDir[i]);
    }

    // After children are gone, free this directory
    free(dir);
}

int main(void) {
    Directory *root = create_Directory(NULL, "root");
    Directory *currentWorkingDir = root;

    char command[20];
    char argument[MAX_NAME];

    printf("Simple File Management System\n");
    printf("Commands: cd, ls, search, create, close, open, exit, mkdir\n\n");
    printf("-----------*---*---*---------\n");

    while (1) {
        printf("%s> ", currentWorkingDir->name);
        
        if (scanf(" %s", command) == EOF) break;


        if (strcmp(command, "exit") == 0) {
            printf("Shutting down...\n");
            break;
        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", argument);
            currentWorkingDir = cdcmd(currentWorkingDir, argument);
        } 
        else if (strcmp(command, "search") == 0) {
            scanf("%s", argument);
            printf("Searching for '%s' starting from %s...\n", argument, currentWorkingDir->name);
            search_files(currentWorkingDir, argument);
        } 
        else if (strcmp(command, "ls") == 0) {
            printf("Contents of %s:\n", currentWorkingDir->name);
            for(int i = 0; i < currentWorkingDir->subDirCount; i++)
                printf(" exit[DIR] %s\n", currentWorkingDir->subDir[i]->name);
            for(int i = 0; i < currentWorkingDir->fileCount; i++)
                printf(" [FILE] %s\n", currentWorkingDir->files[i].fileName);
        }
        else if (strcmp(command, "create") == 0) {
            scanf("%s", argument);
            create_files(currentWorkingDir, argument);
        }
        else if (strcmp(command, "close") == 0) {
            scanf("%s", argument);
            close_files(currentWorkingDir, argument);
        }
        else if (strcmp(command, "open") == 0) {
            scanf("%s", argument);
            open_files(currentWorkingDir, argument);
        }
        else if (strcmp(command, "mkdir") == 0) {
            scanf("%s", argument);
            if (currentWorkingDir->subDirCount < MAX_DIRS) {
                currentWorkingDir->subDir[currentWorkingDir->subDirCount] = create_Directory(currentWorkingDir, argument);
                currentWorkingDir->subDirCount++;
                printf("Directory created.\n");
            } else {
                printf("Directory limit reached.\n");
            }
        }else {
            printf("Unknown command: %s\n", command);
        }
    }

    // createDirectory use malloc to allocated new memory
    // we need to free that memory when not use.
    free_Directory(root); 

    return 0;
}