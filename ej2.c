//Jimena Hernández 21199

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5 //Numero de filosofos
#define PENSANDO 0 //Estado pensando
#define HAMBRIENTO 1 //Estado hambriento
#define COMIENDO 2 //Estado comiendo
#define IZQUIERDA (filosofo + 4) % NUM_FILOSOFOS //Numero de filosofo a la izquierda
#define DERECHA (filosofo + 1) % NUM_FILOSOFOS //Numero de filosofo a la derecha

sem_t mutex; //Semaforo para la exclusion mutua

sem_t s[NUM_FILOSOFOS]; //Semaforo para cada filosofo

void *filosofo(void *num);
void tomar_tenedores(int);
void dejar_tenedores(int);
void prueba(int);

int estado[NUM_FILOSOFOS]; //Estado de cada filosofo
int filosofos[NUM_FILOSOFOS] = {0, 1, 2, 3, 4}; //Numero de cada filosofo

void *filosofo(void *num) {
    while (1) {
        int *i = num;
        sleep(1);
        tomar_tenedores(*i);
        sleep(0);
        dejar_tenedores(*i);
    }
}

void tomar_tenedores(int filosofo) {
    //Funcion para tomar tenedores
    sem_wait(&mutex); //Espera a que el semaforo este disponible
    estado[filosofo] = HAMBRIENTO;  //Cambia el estado del filosofo a hambriento
    printf("Filosofo %d hambriento\n", filosofo + 1);
    prueba(filosofo); //Intenta tomar los tenedores
    sem_post(&mutex); 
    sem_wait(&s[filosofo]); 
    sleep(1);
}

void dejar_tenedores(int filosofo) {
    //Funcion para dejar tenedores
    sem_wait(&mutex);
    estado[filosofo] = PENSANDO; //Cambia el estado del filosofo a pensando
    printf("Filosofo %d dejando tenedores %d y %d\n", filosofo + 1, IZQUIERDA + 1, filosofo + 1);
    printf("Filosofo %d pensando\n", filosofo + 1);
    prueba(IZQUIERDA); //Intenta tomar el tenedor izquierdo
    prueba(DERECHA); //Intenta tomar el tenedor derecho
    sem_post(&mutex);
}

void prueba(int filosofo) {
    //Funcion para probar si el filosofo puede tomar los tenedores
    if (estado[filosofo] == HAMBRIENTO && estado[IZQUIERDA] != COMIENDO && estado[DERECHA] != COMIENDO) {
        estado[filosofo] = COMIENDO; //Cambia el estado del filosofo a comiendo
        sleep(2);
        printf("Filosofo %d toma tenedores %d y %d\n", filosofo + 1, IZQUIERDA + 1, filosofo + 1);
        printf("Filosofo %d comiendo\n", filosofo + 1);
        sem_post(&s[filosofo]);
    }
}

int main() {
    int i;
    pthread_t thread_id[NUM_FILOSOFOS]; //Hilos para cada filosofo
    sem_init(&mutex, 0, 1); //Inicializa el semaforo mutex
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&s[i], 0, 0);
    }
    for (i = 0; i < NUM_FILOSOFOS; i++) { //Crea los hilos para cada filosofo
        pthread_create(&thread_id[i], NULL, filosofo, &filosofos[i]);
        printf("Filosofo %d pensando\n", i + 1); //Empieza pensando
    }
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(thread_id[i], NULL);
    }
}



