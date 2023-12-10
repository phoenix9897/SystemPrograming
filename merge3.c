#include <stdio.h>

#include <string.h>

#define BUFFER_SIZE 1024
int i = 0;
void replaceForwardSlashes(char * str) {
    while ( * str != '\0') {
        if ( * str == '/') {
            * str = '\\';
        }
        str++;
    }
}

void setArrayToZero(int arr[], int size) {
    for (i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int main() {
    int numberOfFiles;
    int sizeOfFiles[numberOfFiles];
    int points[numberOfFiles];
    FILE * outputFile;
    setArrayToZero(sizeOfFiles, numberOfFiles);
    setArrayToZero(points, numberOfFiles);
    char fileNames[numberOfFiles][50];
    char ch;
    size_t bytesRead;
    char buffer[BUFFER_SIZE];
    long fileSize;
    outputFile = fopen("C:/Users/90533/Desktop/merge.txt", "w");

    numberOfFilesError:
        printf("Please enter the number of files that you want to merge");
    scanf("%d", & numberOfFiles);

    FILE * files[numberOfFiles];
    char filePaths[numberOfFiles][100];

    if (numberOfFiles > 32) {
        printf("You are not allowed to merge more than 32 files\n");
        goto numberOfFilesError;
    }

    fileNamesError:
        for (i = 0; i < numberOfFiles; i++) {
            printf("Please enter the %d .files names that you want to merge ", i);
            scanf("%s", & fileNames[i]);

            if (strstr(fileNames[i], ".txt") == NULL) {
                printf("You are not allowed to merge rather than .txt files!!!\n");
                goto fileNamesError;
            }

        }

    for (i = 0; i < numberOfFiles; i++) {
        strcpy(filePaths[i], "C:/Users/90533/Desktop/");
        strcat(filePaths[i], fileNames[i]);
        replaceForwardSlashes(filePaths[i]);
        printf("\n");
        files[i] = fopen(filePaths[i], "r");

        if (files[i] == NULL) {
            printf("System can not open the files enter the names of files again");
            goto fileNamesError;
        } 
        
        while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, files[i])) > 0) {
            fwrite(buffer, 1, bytesRead, outputFile);
            fseek(files[i], 0, SEEK_END);
            sizeOfFiles[i] = ftell(files[i]);
            printf("%d.file size: %d bytes\n", i+1,sizeOfFiles[i]);
			points[i] = points[i-1]+sizeOfFiles[i];
			printf("%d.point is:%d\n",i+1,points[i]);
        }

    }

    return 0;
}
