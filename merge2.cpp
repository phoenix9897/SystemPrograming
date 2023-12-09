#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file1, *file2,*file3,*file4,*outputFile,*file5,*file6;  // bunlar dosyalarýn pointerlarý
    char ch;										//bu dosyalarý okumak için kullanacaðýmýz char 
	int sizeOfFiles[]={0,0,0};						//her dosyanýn boyutunu int olarak tutmak için kullanacaðýmýz dizi 
   	int sizeCheck=0;								//toplam boyutu belirlemek için kullanýlacak int deðer 
	int points[]={0,0,0};
	
    
    file1 = fopen("C:/Users/90533/Desktop/file1.txt", "r");
    file2 = fopen("C:/Users/90533/Desktop/file2.txt", "r");
    file5 = fopen("C:/Users/90533/Desktop/file5.txt", "r");
   	outputFile = fopen("C:/Users/90533/Desktop/merge.txt", "w");


   
   	/*
    if (file1 == NULL || file2 == NULL) {
        printf("Error opening input files. Make sure the files exist.\n");
        return 1;
    }
   	*/	
    /*
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        fclose(file1);
        fclose(file2);
        return 1;
    }
	*/
	
		while ((ch = fgetc(file1)) != EOF) {
    	sizeOfFiles[0]++;
    	sizeCheck++;
        fputc(ch, outputFile);
        
    	}
			
		points[0]=points[0]+sizeOfFiles[0];
    	printf("this is first's size %d\n",sizeOfFiles[0]);
		printf("first point is %d\n",points[0]);
		
		
	
	
	
    	while ((ch = fgetc(file2)) != EOF) {
    	sizeOfFiles[1]++;
    	sizeCheck++;
        fputc(ch, outputFile);
        
    	}		

		points[1]=points[0]+sizeOfFiles[1];
		printf("this is second's size %d \n",sizeOfFiles[1]);
		printf("second point is %d\n",points[1]);
		
		
		
		while ((ch = fgetc(file5)) != EOF) {
    	sizeOfFiles[2]++;
    	sizeCheck++;
        fputc(ch, outputFile);
        
    	}		

		points[2]=points[1]+sizeOfFiles[2];
		printf("this is third's size %d \n",sizeOfFiles[2]);
		printf("third point is %d\n",points[2]);
		
		

	
    
	
	fclose(outputFile);
	
	outputFile = fopen("C:/Users/90533/Desktop/merge.txt", "r");
	file3 = fopen("C:/Users/90533/Desktop/file3.txt", "w");
    file4 = fopen("C:/Users/90533/Desktop/file4.txt", "w");
    file6 = fopen("C:/Users/90533/Desktop/file6.txt", "w");
    
	int i=0;
	
	
	while ((ch = fgetc(outputFile)) != EOF) {
		
        if(i<points[0]) {
            fputc(ch,file3);
            i++;
            printf("this is first file i %d\n",i);
            
            if(i==points[0]){
            	continue;
			}
        }
		if(points[0]<=i && i<points[1] ){
			
			fputc(ch,file4);
			i++;
			printf("this is second i %d\n",i);
			if(i==points[1]){
            	continue;
			}
		}     
		if(points[1]<=i && i<points[2]+1 ){
			
			fputc(ch,file6);
			i++;
			printf("this is third i %d\n",i);
			
		}  
       
    }
    
    
    
    // Close all files
    fclose(file1);
    fclose(file2);
    fclose(file4);
    fclose(outputFile);

    printf("Files merged and separated successfully!\n");

    return 0;
}
