#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>

int main(){

    pid_t pid_fils;
    int tube[2],status;
    char buffer_read[80],buffer_write[80];

    //le messsage que le fils dois ecrire.
    sprintf(buffer_write,"messagere du pere au fils");
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
    // si le forkage c bien passe
    if(pid_fils==0){
        printf("fermeture des entrer dans le fils  %d\n",getpid());
        close(tube [1]);
        //je lis la chaine de caractere 
        read(tube[0],buffer_read,80);
        printf("----- la lecture avant la mise en majuscule du caracteres -----\n ");
        printf("le fils (%d) a lu : %s\n", getpid(),buffer_read);
        printf("\n");
        for(int i=0;i < 80;i++){
            buffer_read[i]=toupper(buffer_read[i]);
        }
        printf("\n");
        printf("----- la lecture apres la mise en majuscule du caracteres -----\n ");
        printf("le fils (%d) a lu : %s\n", getpid(),buffer_read);
        exit(1);
    }

    
    else{
        printf("fermeture des sorties dans le pere (pid = %d)\n",getpid());
        close(tube[0]);
        write(tube[1],buffer_write,80);
        pid_fils=wait(&status);
        if(WIFEXITED(status)){
            printf("\n");
            printf("le fils %d termine et son code de retour est %d \n",pid_fils,WEXITSTATUS(status));
            return EXIT_SUCCESS;
        }
    }

    return 0;
}