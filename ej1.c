//Jimena Hernández 21199
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

void bloquear(int fd, const char* nombreArchivo) {
    if (flock(fd, LOCK_EX) == -1) { //flock bloquea el archivo 
        perror("Error al adquirir bloqueo");
        printf("Archivo: %s\n", nombreArchivo);
        exit(EXIT_FAILURE);
    } else {
        printf("Bloqueo adquirido en %s\n", nombreArchivo);
    }
}

void liberar(int fd, const char* nombreArchivo) {
    if (flock(fd, LOCK_UN) == -1) { //flock desbloquea el archivo
        perror("Error al liberar bloqueo");
        exit(EXIT_FAILURE);
    } else {
        printf("Bloqueo liberado en %s\n", nombreArchivo);
    }
}

int main() {
    int fd1 = open("archivo1.txt", O_RDWR | O_CREAT, 0666);
    int fd2 = open("archivo2.txt", O_RDWR | O_CREAT, 0666);

    if (fd1 == -1 || fd2 == -1) {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear proceso");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Proceso hijo
        while (1) {
            bloquear(fd1, "archivo1.txt");
            sleep(1);  // Simula un trabajo con el archivo
            bloquear(fd2, "archivo2.txt");
            liberar(fd2, "archivo2.txt");
            liberar(fd1, "archivo1.txt");
        }
    } else { // Proceso padre (orden inverso)
        while (1) {
            bloquear(fd2, "archivo2.txt");
            sleep(1);  // Simula un trabajo con el archivo
            bloquear(fd1, "archivo1.txt");
            liberar(fd1, "archivo1.txt");
            liberar(fd2, "archivo2.txt");
        }
    }

    close(fd1);
    close(fd2);

    return 0;
}
