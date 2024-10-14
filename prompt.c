#include"prompt.h"

void prompt(char dir[],char home_dir[]){
    // fetching the system name
    char sys[4096];
    gethostname(sys,sizeof(sys));
    // printf("%s\n",sys);
    char username[4096];
    getlogin_r(username,sizeof(username));
    // char cur[4096];
    // getcwd(cur,sizeof(cur));
    // printf("%s\n",cur);
    int a_len = strlen(home_dir);
    int r_len = strlen(dir);
    // printf("%s\n",username);
    // printf("<%s@%s:~>\n",username,sys);

    char*token1;
    char*token2;
    char*tokens1[4096];
    char*tokens2[4096];
    int i = 0;
    char h_copy[4096];
    strncpy(h_copy,home_dir,sizeof(h_copy));
    h_copy[sizeof(h_copy) - 1] = '\0';
    token1 = strtok(h_copy, "/"); // current working dir
    while (token1 != NULL) {
        tokens1[i++] = token1;
        token1 = strtok(NULL, "/");
    }
    char dir_copy[4096];
    strncpy(dir_copy, dir, sizeof(dir_copy));
    
    dir_copy[sizeof(dir_copy) - 1] = '\0';
    token2 = strtok(dir_copy, "/"); // the one we are passing
    int j = 0;
    while (token2 != NULL) {
        tokens2[j++] = token2;
        token2 = strtok(NULL, "/");
    }
    int flag = 1;
    int a = 0,b = 0;
    while(a<i && b<j){
        if(strcmp(tokens1[a],tokens2[b]) == 0){
            a++;
            b++;
        }
        else{
            flag = 0;
            break;
        }
    }
    if(flag){
        if(i<j){
            // we are inside home dir
            if(elapse){                
                printf("<%s@%s:%s %s:%ds>",username,sys,dir+a_len,cmd,elapsed_time);
                elapse = 0;
            }
            else{
                printf("<%s@%s:%s>",username,sys,dir+a_len);                
            }            
            
        }
        else if(i == j){
            if(elapse){
                printf("<%s@%s:~ %s:%ds>",username,sys,cmd,elapsed_time);
                elapse = 0;
            }
            else{
                printf("<%s@%s:~>",username,sys);
            }
             
        }
        else{
            // printf("%s\n",dir);
            if(elapse){
                printf("<%s@%s:%s %s:%ds>",username,sys,dir,cmd,elapsed_time);
                elapse = 0;
            }
            else{
                printf("<%s@%s:%s>",username,sys,dir);
            }
            
        }
    }
    else{
        // printf("%s\n",dir);
        if(elapse){
            printf("<%s@%s:%s %s:%ds>",username,sys,dir,cmd,elapsed_time);
            elapse = 0;
        }
        else{
            printf("<%s@%s:%s>",username,sys,dir);
        }
        

    }


}

// int main(){
//     char pr[4096] = "/home/rahul/Desktop/SEM 3-1";
//     prompt(pr);
    
// }

// int main(){
//      char home_dir[4096];
//     getcwd(home_dir, sizeof(home_dir));  
    
//     // Example usage of the prompt function
//     prompt(home_dir);

// }

