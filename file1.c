#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#define MAX_FILE_SIZE 209715200 // 200 MBytes for checking the max value of outputFile
#define MAX_NUM_FILES 32 // the maximum number of inputfiles will be checked    
#define BUFFER_SIZE 1024

int main(int argc, char * argv[]) {

    char c;
    int resulta;
    int resultb;
    int i;
    FILE * fp;
    FILE * fd;
    char * outputfileName = "a.sau";
    int totalSize;
    struct stat fs;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    
    
    if (argc > MAX_NUM_FILES + 4) {
        printf("You are not allowed to merge more than 32 files");
        exit(1);
    }

    int numberOfFile = argc - 4;

    resulta = strcmp(argv[1], "-a");
    resultb = strcmp(argv[1], "-b"); 

    outputfileName = argv[argc - 1];

    fd = fopen(outputfileName, "w");

    for (i = 2; i < argc - 2; i++) 
    {

        if (strstr(argv[i], ".txt") == NULL) {

            printf("The file %s is not a text file.\n", argv[i]);
            exit(1);
        }
    }

    if (!resulta) {

        printf("files are extracted\n");

        //void exractFiles();

    } else if (!resultb) {

        for (i = 2; i < argc - 2; i++) 
        {

            fp = fopen(argv[i], "r");

            if (fp == NULL) {
                fprintf(stderr, "Couldn't open the files %s!\n", argv[i]);
                return 2;

            } else if (fp != NULL) {

                printf("Opened files %s\n", argv[i]);

                char buffer[BUFFER_SIZE];
                size_t bytesRead;

                fseek(fp, 0, SEEK_END);

                long int sizeOfFile = ftell(fp);
                if (stat(argv[i], & fs) != 0) {
                    perror("Error getting file stat");
                    return -1;
                }

                fprintf(fd, "|%s %o %lu|", argv[i], fs.st_mode & 777, sizeOfFile - 1);

                fseek(fp, 0, SEEK_SET);

                fseek(fp, 0, SEEK_END);

                totalSize += ftell(fp);
                fclose(fp);
            }

        }

        for (i = 2; i < argc - 2; i++) 
        {

            fp = fopen(argv[i], "r");

            while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                fwrite(buffer, 1, bytesRead, fd);
            }
            fclose(fp);

        }

        printf("files are merged\n");
        printf("Total Size is %d\n", totalSize);
        if (totalSize > MAX_FILE_SIZE) {
            printf("Total Size is bigger than 200mb which is the top limit\n");
            exit(1);
        }
    }

    return 0;
}
