
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_LEN 100

struct fun_desc {
  char *name;
  void (*fun)(void);
};
char* dataPtr; 
int size=1;
char* filename=NULL;
char buf[BUF_LEN];
char* buffer;
void* data_pointer=NULL;

char *copy_namefile(char *s){
     int i=0;
     while(s[i]!='\n'){
         buf[i]=s[i];
         i++;
     }
     buf[i]='\0';
     return buf;
}

void SetFileName(){
    char newFilename[BUF_LEN];
    printf("Insert the file name please:\n");
    fgets(newFilename,BUF_LEN,stdin);  
    filename=copy_namefile(newFilename);  
}


void SetUnitSize(){
    char new_size[BUF_LEN];
    printf("Insert the unit size please:\n");
    fgets(new_size,BUF_LEN,stdin);
    if(strcmp(new_size,"1\n")==0||strcmp(new_size,"2\n")==0||strcmp(new_size,"4\n")==0){
        size=new_size[0]-'0';
    }else{
       printf("ERROR!!\n"); 
    }
}

void Quit(){
  
    exit(0);
}
 void PrintDec( int ln) {
        printf("\nDecimal Representation:\n");
	char* s_arr=malloc(BUF_LEN+1);
    char *unity;
    for(int i=0;i<ln*size;i=i+size){
        for(int j=size-1;j>=0; j--){
            sprintf(s_arr,"%02hhx",buffer[i+j]);
            *s_arr++;
            *s_arr++;
        }
        for(int k=0; k<(size*2); k++){
            *s_arr--;
        }
        int decNumber=(int)strtol(s_arr,&unity,16);
        printf("%d ",decNumber);
    }
    free(s_arr);
    printf("\n");
}

void printHexOrDec(int length, int location){
 int i;
    int j ;
    printf("\nHexadecimal Representation:\n");
    for(i=0; i < length * size; i = i + size){
        for(j = size - 1; j >= 0 ; j--){
            printf("%02hhx" , buffer[i+j]);
        }
        printf(" ");
    }
}



void FileModify()
{
    int location, val;
    char location_val[BUF_LEN];
    if(filename==NULL){
        printf("file not exsits\n");
        return;
    }
    FILE* file=fopen(filename,"r+");
    if(file==NULL){
        printf("fail to open file\n");
        return;
    }
    printf("Please enter <location> <val> : \n");
    fgets(location_val, sizeof(location_val),stdin);
    sscanf(location_val, "%x %x", &location, &val);
    fseek(file,location,SEEK_SET);
    fwrite((char*)&val,1,size,file);
    fclose(file);

}

void FileDisplay(){
    int location,length;
    char location_length[BUF_LEN];
    if(filename==NULL){
        printf("file not exsits\n");
        return;
    }
    FILE* file=fopen(filename,"rb+");
    if(file==NULL){
        printf("fail to open file\n");
        return;
    }
    printf("Please enter <location> <length> : \n");
    fgets(location_length,BUF_LEN,stdin);
    sscanf(location_length, "%x %d", &location, &length);

    buffer=(void*)malloc(size*length);
    fseek(file,location,SEEK_SET);
    fread(buffer,size,length,file);
    fclose(file);
    printHexOrDec(length,location);
    PrintDec(length);
    free(buffer);
}

void CopyFromFile(){
    FILE *dst;
    FILE *src;
    char src_file[1000];
    char input[BUF_LEN];
    int length;
    int src_offset;
    int dst_offset;
    void* buf;

    printf("Please enter <src_file> <src_offset> <dst_offset> <length>\n");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%s %x %x %d", src_file, &src_offset, &dst_offset, &length);

    if(src_file==NULL){
        printf("file not exsits\n");
        return;
    }
    if( filename==NULL){
   printf("file not exsits\n");
        return;
    }
    if(!(src = fopen(src_file,"r"))){
        printf("fail to open file\n");
        return;
    }
    if(!(dst = fopen(filename,"r+"))){
        printf("fail to open file\n");
        return;
    }
        src_file[ strlen(src_file)] = '\0';

    buf = malloc(size*length);
    fseek(src, src_offset, SEEK_SET);
    fread(buf, length, size, src);
    printf("load %d bytes into %p from %s to %s\n",length,buffer ,src_file, filename);
    fseek(dst, dst_offset, SEEK_SET);
    fwrite(buf,1, size, dst);
    fclose(dst);
    fclose(src);
    free(buf);
}

int main(int argc, char **argv){
    int num,i;
    char in;
    struct fun_desc menu[] = {
                               { "1-Set File Name", SetFileName }, 
                               { "2-Set Unit Size", SetUnitSize }, 
                               { "3-File Display", FileDisplay },
                               {"4-File Modify", FileModify}, 
                               {"5-CopyFromFile",CopyFromFile},
                               { "6-Quit", Quit }
                             };
    while(1){
        printf("%s\n","Choose action:");
        for(i=0;i<6;i++){
            printf("%s\n",(menu[i]).name);
        }
        in=fgetc(stdin);
        fgetc(stdin);
        num=in-'1';
        (*(menu[num].fun))();
    }
} 
