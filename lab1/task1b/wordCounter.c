
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  
int c;
int i;
FILE *fp=stdin;
int words=0;
int bool_words=0;
int bool_ch=0;
int numberOfCharacter=0;
int bool_longestword=0;
int counter=0;
int max=0;
int numberlines=1;
int bool_numberlines=0;
int bool_line_space_tab=0;
int def=0;

for(i=1;i<argc;i++){
    if(strncmp(argv[i],"-n",2)==0){
        bool_numberlines=1;
        def=1;}
    if(strncmp(argv[i],"-w",2)==0){
        bool_words=1;
        }
    if(strncmp(argv[i],"-l",2)==0){
        bool_longestword=1;
        def=1;
        
    }
    if(strncmp(argv[i],"-c",2)==0){
        bool_ch=1;
        def=1;
    }
}
while((c=fgetc(fp))!=EOF){
    if((c==' ' )||(c=='\n')||(c=='\t')){
        bool_line_space_tab=0;
    }
    if((c>32)&& (c!='\t')&& (c!='\n')){
        if(bool_line_space_tab==0){
            words=words+1;
            bool_line_space_tab=1;
    }}
    if(bool_ch==1 && c!=32 && c!='\t'&& c!='\n'){
    numberOfCharacter=numberOfCharacter+1;
        
    }
    if(bool_longestword==1){
        if(c==32 || c=='\n'||c=='\t') 
            counter=0;
        else{
            counter=counter+1;
        }
    if(max<counter){
        max=counter;
    }
}
    if(bool_numberlines==1){
        if(c=='\n'){
            numberlines=numberlines+1;
        }
    }  
}
    
    if(def==0||bool_words) printf("%d\n", words);
    if(bool_ch) printf("%d\n",numberOfCharacter);
    if(bool_longestword) printf("%d\n",max);
    if(bool_numberlines) printf("%d\n",numberlines);


fclose(fp);
    return 0;
} 
