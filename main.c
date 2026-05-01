/**
 * CS149-Operating System
 * Project: File management
 * Author:
 * Date: May 2026
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_NAME 30
#define MAX_CONTENTS 120
#define MAX_FILES 20
#define MAX_DIRS 10

typedef struct{
    char fileName[MAX_NAME];
    char contents[MAX_CONTENTS];
} File;


typedef struct {
    char name[MAX_NAME];
    struct Directory *currentDir;
    struct Directory *subDir[MAX_DIRS];
    File files[MAX_FILES];
    int subDirCount;
    int fileCount;


} Directory;

Directory *create_Directory (Directory *currentDir, char *name) {
    // allocating a new memory for new directory
    Directory *dir = malloc(sizeof(Directory));
    if (dir == NULL) {
        printf("Memory allocated fail.");
        exit(1);
    }
    // initialise directory members
    dir->currentDir = currentDir;
    strcpy(dir->subDir, name);
    dir->subDirCount = 0;
    dir->fileCount = 0;

    return dir;
}

Directory *search_Directory (Directory *currentDir, char *name) {
    int i;
    for(int i = 0; i < currentDir->subDirCount; i++){
        if(strcmp(currentDir->subDir[i], name) == 0) {
            currentDir = currentDir->subDir[i];
        }
    }
    return NULL;
}

void create_files(Directory *current, char *name);
void open_files(Directory *current, char *name);
void close_files(Directory *current, char *name);
void search_files(Directory *current, char *name);
int search_index(Directory *current, char *name);

int main(void) {

    printf("Simple File Management System\n");
    printf("-----------*---*---*---------\n");


    return 0;
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
    // TODO
}


void open_files(Directory *current, char *name){
    // TODO
    // check if file exits

}

void close_files(Directory *current, char *name) {

}

void search_files(Directory *current, char *name){

}

// This function looks at the position of a file in the array of files.
// It returns -1 if file already exist.
int search_index(Directory *current, char *name) {
    // TODO
}