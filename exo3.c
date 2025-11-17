#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid_fils;
    int tube[2], tube2[2],tube3[2] , tube4[2] , status, i1 = 0, i2 = 0, j1 = 0, j2 = 0;
    int bufferR[80], bufferR2[80];
    int b, c, d, sum1 = 0, sum2 = 0;

    puts("Creation du premier tube");
    if (pipe(tube) != 0) {
        fprintf(stderr, "Erreur dans pipe\n");
        exit(1);
    }

    if (pipe(tube4) != 0) {
        fprintf(stderr, "Erreur dans pipe\n");
        exit(1);
    }

    if (pipe(tube3) != 0) {
        fprintf(stderr, "Erreur dans pipe\n");
        exit(1);
    }

    puts("Creation du second tube");
    if (pipe(tube2) != 0) {
        fprintf(stderr, "Erreur dans pipe\n");
        exit(1);
    }

    pid_fils = fork();

    if (pid_fils == -1) {
        fprintf(stderr, "Erreur dans fork\n");
        exit(1);
    }

    if (pid_fils == 0) {
        printf("Fermeture sortie dans le fils de gauche (pid = %d)\n", getpid());
        close(tube[1]);
        close(tube2[1]);
        while (read(tube[0], &c, sizeof(int)) > 0) {
            printf("lecture de :%d\n", c);
            bufferR[i1] = c;
            i1++;
            sum1 += c;
        }

         printf("Somme des nombres du fils de gauche : %d\n", sum1);

        printf("Le fils de gauche (%d) a lu : ", getpid());
        while (j1 < i1) {
            printf("(%d)", bufferR[j1]);
            j1++;
        }
        printf("\n");

        close(tube[0]);

        // Envoyer la somme au pere
        close(tube3[0]);
        close(tube4[0]);
        write(tube3[1], &sum1, sizeof(int));
        close(tube3[1]);

        exit(0);
    } else {
        pid_t pid_fils2 = fork();

        if (pid_fils2 == 0) {
            printf("Fermeture sortie dans le fils de droite (pid = %d)\n", getpid());
            close(tube2[1]);
            close(tube[1]);
            close(tube[0]);
            while (read(tube2[0], &d, sizeof(int)) > 0) {
                bufferR2[i2] = d;
                i2++;
                sum2 += d;
            }

            printf("Le fils de droite (%d) a lu : ", getpid());
            while (j2 < i2) {
                printf("(%d)", bufferR2[j2]);
                j2++;
            }
            printf("\n");

            close(tube2[0]);
            close(tube4[0]);
            close(tube3[0]);

            // Envoyer la somme au pere
            write(tube4[1], &sum2, sizeof(int));
            close(tube4[1]);

            exit(0);
        }

        printf("Fermeture entree dans le pere (pid = %d)\n", getpid());
        close(tube[0]);
        close(tube2[0]);

        do {
            printf("Entrer un nombre : ");
            scanf("%d", &b);

                if (b < 0) {
                    printf("Le pere ecrit dans le tube 1 : (%d)\n", b);
                    write(tube[1], &b, sizeof(int));
                } else if (b > 0) {
                    printf("Le pere ecrit dans le tube 2 : (%d)\n", b);
                    write(tube2[1], &b, sizeof(int));
                }
                else 
                	break;
        } while (b != 0);

        close(tube[1]);
        close(tube2[1]);

        wait(NULL);
        wait(NULL);
		printf("Les fils ont termine.\n");
        

        int sum_left, sum_right;

        // Lecture des sommes envoyees par les fils
        close(tube3[1]);
        close(tube4[1]);

        read(tube3[0], &sum_left, sizeof(int));
        read(tube4[0], &sum_right, sizeof(int));

        printf("Somme des nombres du fils de gauche : %d\n", sum_left);
        printf("Somme des nombres du fils de droite : %d\n", sum_right);
		close(tube3[0]);
        close(tube4[0]);
        
       
    }return 0;
}
