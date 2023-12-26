#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#include <stdint.h>

#define MAX_FILE_SIZE 209715200 // 200 MBytes for checking the max value of outputFile.
#define MAX_NUM_FILES 32 // The maximum number of input files will be checked.
#define BUFFER_SIZE 1024 // Read files as a maximum of 1024 bytes.

char ch; // Will be used for reading and writing.
int resulta; // Will be used for calling the extract files part.
int resultb; // Will be used for calling the merge files part.
int i; // Will be used for loops.
FILE * fp; // This pointer will keep the input file's names.
FILE * fd; // This pointer will keep the output file's name.
char * outputfileName = "a.sau"; // Default output file name.
int totalSize; // This variable keeps the total size of input files.
struct stat fs; // Will be used to reach more data of files.
size_t bytesRead; // Reading and writing data.
int header = 0; // Length of the archive file (first 10 bytes).
long int sizeOfFile = 0; // This variable keeps the size of files temporarily.

void checkMaxFile(int argc) {
    // If argc is more than 36, which tells there are more than 32 input files.
    if (argc > MAX_NUM_FILES + 4) {
        printf("You are not allowed to merge more than 32 files\n");
        exit(1);
    }
}

void checkFilesTxtOrNot(int argc, char * argv[]) {
    // This function controls if input files include ".txt" so that we can control whether they are txt files or not.
    for (i = 2; i < argc - 2; i++) {
        if (strstr(argv[i], ".txt") == NULL) {
            printf("%s input file format is not compatible.\n", argv[i]);
            exit(1);
        }
    }
}

void writeArchive(FILE * fd, int header, FILE * fp, char * argv[], int argc, struct stat fs, long int sizeOfFile) {
    // This function writes to the output file archive part.
    fprintf(fd, "%010d", header); // The first ten bytes to keep the length of the organization (content) part.

    for (i = 2; i < argc - 2; i++) {
        fp = fopen(argv[i], "r");

        fseek(fp, 0, SEEK_END);
        sizeOfFile = ftell(fp);

        fprintf(fd, "|%s,%o,%lu|", argv[i], fs.st_mode & 777, sizeOfFile - 1);
    }
}

void writeToOutputFile(FILE * fp, char * argv[], int argc, char ch, FILE * fd) {
    // This function is used for reading input file's data and writing to the output file name.
    for (i = 2; i < argc - 2; i++) {
        fp = fopen(argv[i], "r");
        fseek(fp, 0, SEEK_SET);

        while ((ch = fgetc(fp)) != EOF) {
            fprintf(fd, "%c", ch);
        }
    }
    printf("Files are merged\n");
}

void checkMaxSize(int totalSize) {
    // This function controls if the total size of files is more than 200MB.
    printf("Size of files is %d\n", totalSize);
    if (totalSize > MAX_FILE_SIZE) {
        printf("Total Size is bigger than 200MB, which is the top limit\n");
        exit(1);
    }
}

int main(int argc, char * argv[]) {
    resulta = strcmp(argv[1], "-a"); // If -a parameter has entered, then extract files.
    resultb = strcmp(argv[1], "-b"); // If -b parameter has entered, then merge files.

    if (!resulta) { // Extract output file part.
        if (argc != 4) {
            printf("Invalid number of arguments.\n");
            exit(1);
        }

        const char * archiveFileName = argv[2]; //This part takes the archive file (a.sau).
        const char * directoryPath = argv[3]; //This part takes the directory of files will be opened.

        // Check if the archive file name is appropriate
        if (strstr(archiveFileName, ".sau") == NULL) {
            printf("Archive file is inappropriate or corrupt.\n");
            exit(1);
        }

        // Open archive file for reading
        FILE * archive = fopen(archiveFileName, "r");
        if (archive == NULL) {
            perror("Error opening archive file for reading");
            exit(1);
        }

        // Check if the directory exists
        struct stat st;
        if (stat(directoryPath, & st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                //printf("Directory exists.\n");
            } else {
                printf("A file with the same name exists.\n");
            }
        } else {
            // Directory does not exist
            printf("Directory does not exist. Creating it...\n");

            // Create the directory
            if (mkdir(directoryPath, 0777) == 0) {
                printf("Directory created successfully.\n");
            } else {
                perror("Error creating directory");
                return 1;
            }
        }

        size_t bytes_to_read = 10; //This is for reading the header in archived file. 
        unsigned char buffer[bytes_to_read];

        // Read the header of archived file.
        size_t bytesRead = fread(buffer, 1, bytes_to_read, archive);
        if (bytesRead != bytes_to_read) {
            if (feof(archive)) {
                printf("End of file reached. Read %zu bytes.\n", bytesRead);
            } else if (ferror(archive)) {
                perror("Error reading from file");
            }
        } else {

            int baslik = 0;
            
            char buffer[11]; // 10 karakter + 1 null karakter
            int number;
            fseek(archive, 0, SEEK_SET);
            if (fread(buffer, 1, 10, archive) != 10) {
                perror("Okuma hatası");
                fclose(archive);
                return 1;
            }

           
            buffer[10] = '\0';
            printf("%s", buffer);
            number = atoi(buffer);

            printf("Okunan sayı: %d\n", number);
            baslik =number;

            int sayi;
            char character;

            if (fseek(archive, baslik, SEEK_SET) != 0) {
                perror("fseek error");
                fclose(archive);
                return 1;
            }

            fseek(archive, 0, SEEK_SET);
            int karakter; //Burayı kaç tane dosya olduğunu bulmak için | sayısı 
            int pipeSayisi = 0; //yarıya böldük.	

            while ((karakter = fgetc(archive)) != EOF) {
                if (karakter == '|') {
                    pipeSayisi++;
                }
            }

            int numberOfInputFiles = pipeSayisi / 2;

            fseek(archive, 0, SEEK_SET);

            if (fseek(archive, baslik, SEEK_SET) != 0) {
                perror("Error using fseek");
                fclose(archive);
                return 1;
            }

            fseek(archive, 0, SEEK_SET);

            char tampon[1024];
            size_t okunanByteSayisi = fread(tampon, 1, sizeof(tampon), archive);
            if (okunanByteSayisi == 0) {
                perror("Dosya okuma hatası");
                fclose(archive);
                return 1;
            }

            const char * delimiter = "|"; // Ayraç olarak kullanılacak karakter

            char * dosyalar[numberOfInputFiles]; // .txt uzantılı dosya adlarını tutacak dizi
            int dosyaSayisi = 0;

            // Metni parçala
            char * token = strtok(tampon, delimiter);
            while (token != NULL) {
                // Eğer ".txt" içeriyorsa dosya adını diziye kopyala
                if (strstr(token, ".txt") != NULL) {
                    dosyalar[dosyaSayisi] = strdup(token);
                    dosyaSayisi++;
                }
                // Sonraki parçayı al
                token = strtok(NULL, delimiter);
            }

            char list2[32][50] = {};

            for (int i = 0; i < sizeof(dosyalar) / sizeof(dosyalar[0]); i++) {
                // Satırdaki bilgileri parçala
                char * dosya_adı = strtok(dosyalar[i], ",");

                strcat(list2[i], directoryPath);
                strcat(list2[i], "/");
                strcat(list2[i], dosyalar[i]);
                fopen(list2[i], "w");

            }

            //printf("baslik%d\n",baslik);
            fseek(archive, baslik, SEEK_SET);

            for (int k = 0; k <= numberOfInputFiles; k++) {
                FILE * fk = fopen(list2[k], "w");
                while ((character = fgetc(archive)) != EOF) {

                    fputc(character, fk);
                    if (character == '\n') {
                        break;
                    }

                }

            }

        }

    } else if (!resultb) { // Merge files part.
        checkMaxFile(argc); // This function checks the maximum number of files.

        int numberOfFile = argc - 4; // 4 comes from "tarsau" "-a/-b" "-o" "a.sau"
        // other parameters give us the number of files.

        for (int i = 2; i < argc - 2; i++) {
            // This is used to add all file names as length.
            header += strlen(argv[i]);
        }

        header += 7 * numberOfFile; // This is used to add || and permissions and the " "
        header += 10;
        outputfileName = argv[argc - 1]; // This takes the output file name as argv.

        fd = fopen(outputfileName, "w"); // Open the output file.

        checkFilesTxtOrNot(argc, argv); // This function controls if input files are txt files.

        for (i = 2; i < argc - 2; i++) {
            fp = fopen(argv[i], "r"); // Open input files.

            if (fp == NULL) { // If input files cannot be opened, give an error.
                fprintf(stderr, "Couldn't open the file %s!\n", argv[i]);
                return 2;
            } else if (fp != NULL) { // If input files opened successfully...

                char buffer[BUFFER_SIZE];
                size_t bytesRead;

                fseek(fp, 0, SEEK_END);

                sizeOfFile = ftell(fp); // Gives the size of the input file.

                if (stat(argv[i], & fs) != 0) {
                    perror("Error getting file stat\n");
                    return -1;
                }

                // Add each input file's size length to the header variable.
                header += snprintf(NULL, 0, "%lu", sizeOfFile - 1);
                fseek(fp, 0, SEEK_SET);

                fseek(fp, 0, SEEK_END);

                totalSize += ftell(fp); // Add all sizes of input files.

                fclose(fp);
            }
        }

        totalSize -= numberOfFile; // Each file includes EOF bytes, so we have to subtract
        // 1 byte for each file. For example, if a file includes
        // 6 bytes of data, actually this file includes 5 bytes,
        // 1 comes from EOF.
        /*(We hope we made an accurate decision...)*/

        writeArchive(fd, header, fp, argv, argc, fs, sizeOfFile);

        writeToOutputFile(fp, argv, argc, ch, fd);

        checkMaxSize(totalSize);

    }

    return 0;
}
