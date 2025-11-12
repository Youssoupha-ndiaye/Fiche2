#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>

int main(int argc, char*argv[]){

    if(argc < 2){
        printf("error nbr argument i,nsuffisante\n");
        exit(1);
    }

    pid_t pid_fils;
    int tube[2],status;
    char buffer_read[80];//,buffer_write;
    int b,i=0;

    //tube[0] pour lire des donnees
    //tube[1] pour ecrire des donnees

    puts("creation des tubes");
    if(pipe(tube)!=0){
        fprintf(stderr,"erreur dans pipe");
        exit (1);
    }

    pid_fils=fork();
    if(pid_fils==-1){
        fprintf(stderr,"ereur lors du forckage");
        exit (1);
    }

    if(pid_fils==0){
        printf("\nfermeture des entrer dans le fils  %d\n",getpid());
        close(tube [1]);
        while(read(tube[0],argv[1],1) > 0){
            buffer_read[i]=argv[1];
            i++;
        }
        printf("le fils (%d) a lu : %s\n", getpid(),buffer_read);
        exit(1);
    }
    else{
        printf("fermeture des sorties dans le pere (pid = %d)\n",getpid());
        close(tube[0]);
        while(read(0,argv[1],1)){
            //pour verifier que les lettres sont que des minuscules
            if(!isupper(b)){
                write(tube[1],&b,1);
            }
        }
        close(tube[1]);
        //write(tube[1],buffer_write,80);
        pid_fils=wait(&status);
        if(WIFEXITED(status)){
            printf("le fils %d termine et son code de retour est %d \n",pid_fils,WEXITSTATUS(status));
            return EXIT_SUCCESS;
        }
    }

    return 0;
}