#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#define MAX_FILE_SIZE 209715200 		// 200 MBytes for checking the max value of outputFile.
#define MAX_NUM_FILES 32 			// The maximum number of inputfiles will be checked.  
#define BUFFER_SIZE 1024			// Read files as maximum 1024 byte .

char ch;                         		//Will be used for reading and writing .
int resulta;		         	 	//Will be used for calling extract files part.
int resultb;					//Will be used for calling merge files part.
int i;						//Will be used for loops.
FILE * fp;					//This pointer will keep the input file's names .
FILE * fd;					//This pointer will keep the output file's name.
char * outputfileName = "a.sau";		//Default output file name.
int totalSize;					//This variable keep the total size of input files.
struct stat fs;					//Will be used reaching more data of files.
char buffer[BUFFER_SIZE];			//Reading and writting data.
size_t bytesRead;				//Reading and writting data .
int header = 0;					//Lenght of the archive file (first 10 byte).
long int sizeOfFile = 0;			//This variable keep the size of files temporary.

void checkMaxFile(int argc) {	//If argc more than 36 which tell there are more than 32 inputfiles.
    if (argc > MAX_NUM_FILES + 4) {      	
        printf("You are not allowed to merge more than 32 files\n");
        exit(1);
    }

}

void checkFilesTxtOrNot(int argc, char * argv[]) {  /* This function controls if inputfiles include
						       ".txt" so that we can control they are txt file  
						       or not.  */
    for (i = 2; i < argc - 2; i++) {

        if (strstr(argv[i], ".txt") == NULL) {

            printf("The file %s is not a text file.\n", argv[i]);
            exit(1);
        }
    }

}

void writeArchive(FILE * fd, int header, FILE * fp, char * argv[], int argc, struct stat fs, long int sizeOfFile) {
						//This function writes to output file archive part
    fprintf(fd, "%010d", header);		//first ten byte in order to keep lenth of 
						//organization(content) part.
    for (i = 2; i < argc - 2; i++) {

        fp = fopen(argv[i], "r");

        fseek(fp, 0, SEEK_END);

        sizeOfFile = ftell(fp);

        fprintf(fd, "|%s %o %lu|", argv[i], fs.st_mode & 777, sizeOfFile - 1);

    }
}

void writeToOutputFile(FILE * fp, char * argv[], int argc, char ch, FILE * fd) {

    for (i = 2; i < argc - 2; i++) {		//This function is used for reading input file's data 
        fp = fopen(argv[i], "r");		//and writting to outputfile name.
        fseek(fp, 0, SEEK_SET);

        while ((ch = fgetc(fp)) != EOF) {
            //fputc(ch, fd);
            fprintf(fd, "%c", ch);
        }

    }
    printf("files are merged\n");
}

void checkMaxSize(int totalSize) {	//This function controls if total size of files more than 
					//more than 200mb .
    printf("Size of files is%d\n", totalSize);
    if (totalSize > MAX_FILE_SIZE) {
        printf("Total Size is bigger than 200mb which is the top limit\n");
        exit(1);
    }

}

int main(int argc, char * argv[]) {

    checkMaxFile(argc);          //This function checks maximum number of files.

    int numberOfFile = argc - 4; // 4 comes from "tarsau" "-a/-b" "-o" "a.sau"
			         // other parameter give us to number of files.
			         
    for (int i = 2; i < argc - 2; i++) { //This is used for add all files names as lenght.
        header += strlen(argv[i]);       
    }

    header += 7 * numberOfFile; //This is used for add || and permissions and the " "  
    header += 10;               //for 10 bytes at the beginnig.

    resulta = strcmp(argv[1], "-a");   //If -a parameter has entered than extract files.
    resultb = strcmp(argv[1], "-b");   //If -b parameter has entered than merge files.

    outputfileName = argv[argc - 1];   //This takes the outputfile name as argv.

    fd = fopen(outputfileName, "w");   //Open the outputfile.

    checkFilesTxtOrNot(argc, argv);    //This function controls if inputfiles are txt files.

    if (!resulta) {		       //Extract output file part.

        printf("files are extracted\n");

        //void exractFiles();

    } else if (!resultb) {     			//Merge files part.

        for (i = 2; i < argc - 2; i++) {

            fp = fopen(argv[i], "r");		//Open input files.

            if (fp == NULL) {			//If input files cannot opened gives error.
                fprintf(stderr, "Couldn't open the files %s!\n", argv[i]);
                return 2;

            } else if (fp != NULL) {		//If input files opened succesfully...

                char buffer[BUFFER_SIZE];
                size_t bytesRead;

                fseek(fp, 0, SEEK_END);

                sizeOfFile = ftell(fp);		//Gives the size of input file.

                if (stat(argv[i], & fs) != 0) {
                    perror("Error getting file stat\n");
                    return -1;
                }

                header += snprintf(NULL, 0, "%lu", sizeOfFile - 1); //Add each input file's size length 
							            //to header variable.
                fseek(fp, 0, SEEK_SET);

                fseek(fp, 0, SEEK_END);

                totalSize += ftell(fp);		//Add all size of input files.

                fclose(fp);
            }

        }

        totalSize -= numberOfFile;		//Each file includes EOF bytes so we have to substract
        					//1 byte for each file.For example if a file includes 
        					//6 byte of data actually this file includes 5 byte 
        					//1 comes from EOF.
        					/*(We hope we made a accurate decision...)*/
        					
        writeArchive(fd, header, fp, argv, argc, fs, sizeOfFile);
        
        writeToOutputFile(fp, argv, argc, ch, fd);
        
        checkMaxSize(totalSize);

    }

    return 0;
}

//fseek(file,10,SEEK_SET);
