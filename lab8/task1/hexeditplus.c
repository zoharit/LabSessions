#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

struct stat fd_stat;
struct fun_desc {
  char *name;
  void (*fun)(void);
};
char* filename=NULL;
int file_des=-1;
void *map;
char* get_type(Elf64_Word type)
{
    switch(type)
    {
        case 0:return "NULL";
        case 1:return "PROGBITS";
        case 2:return "SYMTAB";
        case 3:return "STRTAB";
        case 4:return "RELA";
        case 5:return "HASH";
        case 6:return "DYNAMIC";
        case 7:return "NOTE";
        case 8:return "NOBITS";
        case 9:return "REL";
        case 10:return "SHLIB";
        case 11:return "DYNSYM";
        case 14:return "INIT_ARRAY";
        case 15:return "FINI_ARRAY";
        case 16:return "PREINIT_ARRAY";
        case 17:return "GROUP";
        case 18:return "SYMTAB_SHNDX";
        case 19:return "NUM";
        }
    return NULL;
}

void Quit(){
  
    exit(0);
}

void PrintSectionNames(){
      int i=0;
  char filename[100];
    printf("please enter file name\n");
    fgets(filename, sizeof(filename),stdin);
    filename[strlen(filename)-1]='\0';
    if(file_des==-1){
        close(file_des);
    }
        if((file_des = open(filename, O_RDWR)) < 0 ){
        perror("ERROR : CANT OPEN FILE");
    }
   if( fstat(file_des, &fd_stat) != 0 ) {
      perror("EROOR :STAT FAIL");
      exit(-1);
   }
 if ( (map = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, file_des, 0)) == MAP_FAILED ) {
      perror("EROOR :MMAP FAIL");
      close(file_des);
      file_des=-1;
      exit(-4);
   }
    Elf64_Ehdr *header=(Elf64_Ehdr *)map;   
    Elf64_Shdr* section_table=(Elf64_Shdr *)( map+header->e_shoff);
    char* str_table=map + section_table[header->e_shstrndx].sh_offset;
    for(i=0;i<header->e_shnum;i++){
    char* section_name=(char*)(str_table+section_table[i].sh_name);
    printf("[%d] %-20s \t %08x  %05x \t %05x \t %-20s \t \n",i,section_name,(unsigned)section_table[i].sh_addr,
     (unsigned) section_table[i].sh_offset,(unsigned)section_table[i].sh_size,get_type((int)section_table[i].sh_type));
    }
}

void ExamineELFFile(){
    char filename[100];
    printf("please enter file name\n");
    fgets(filename, sizeof(filename),stdin);
    filename[strlen(filename)-1]='\0';
    if(file_des==-1){
        close(file_des);
    }
        if((file_des = open(filename, O_RDWR)) < 0 ){
        perror("ERROR : CANT OPEN FILE");
    }
      if( fstat(file_des, &fd_stat) != 0 ) {
      perror("EROOR :STAT FAIL");
      exit(-1);
   }
 if ( (map = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, file_des, 0)) == MAP_FAILED ) {
      perror("EROOR :MMAP FAIL");
      close(file_des);
      file_des=-1;
      exit(-4);
   }
       Elf64_Ehdr *header=(Elf64_Ehdr *)map;

      printf("Magic: %x %x %x\n",header->e_ident[EI_MAG1],header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
   if (header->e_ident[EI_MAG1]!='E' || header->e_ident[EI_MAG2]!='L' || header->e_ident[EI_MAG3]!='F') {
        printf("ERROR : not an ELF header !\n");
        file_des=-1;
        exit(-1);
   }
    printf("data encoding :\t\t\t\t%x\n",(unsigned) header->e_ident[EI_DATA]);
   printf("entry point : \t\t\t\t%x\n",(unsigned)header->e_entry);
   printf("section header offset : \t\t%d\n",(unsigned)header->e_shoff); 
   printf("number of section header : \t\t%d\n",header->e_shnum);
   printf("section header entry size : \t\t%d\n",header->e_shentsize); 
   printf("program header offset : \t\t%d\n",(unsigned)header->e_phoff);
   printf("number of program header : \t\t%d\n",header->e_phnum);
   printf("size of program header entries : \t%d\n",header->e_phentsize);


}

int main(int argc, char **argv){
    int num,i;
    char in;
    struct fun_desc menu[] = {
                               {"1-Examine ELF File", ExamineELFFile }, 
                               {"2-Print Section Names",PrintSectionNames},
                               { "3-Quit", Quit }
                             };
    while(1){
        printf("%s\n","Choose action:");
        for(i=0;i<3;i++){
            printf("%s\n",(menu[i]).name);
        }
        in=fgetc(stdin);
        fgetc(stdin);
        num=in-'1';
        (*(menu[num].fun))();
    }
} 
