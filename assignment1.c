#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Declaring the Mutex and conditional variables
pthread_mutex_t M;
pthread_cond_t P_cond, C_cond;

//initialize of shared variable
int Buffer_Size[20];
int input = 0;
int output = 0;
int SUMMATION = 0;

// Function to implement Producer
void *P_Fun(void *arg) {

    for (int k = 1; k <= 30; k++) {
        
        //ENTRY-SECTION 
        pthread_mutex_lock(&M);

        // Wait until Buffer_Size is full 
        while (((input + 1) % 20) == output) {
            pthread_cond_wait(&P_cond, &M);
        }

        //CRITICAL SECTION
        Buffer_Size[input] = k;                // Addtion to Buffer_Size by producing item
        input = (++input) % 20;
        pthread_cond_signal(&C_cond);         // Signaling consumer threads


        pthread_mutex_unlock(&M);
    }
    //EXIT SECTION
    pthread_exit(NULL);
}

// Function to implement Producer
void *C_Fun(void *arg) {

    
    for (int k = 1; k <= 30; k++) {
        //ENTRY SECTION
        pthread_mutex_lock(&M);

        //Wait until Buffer_Size is empty 
        while (input == output) {
            pthread_cond_wait(&C_cond, &M);
        }

         //CRITICAL SECTION
        SUMMATION = SUMMATION +Buffer_Size[output];         //Addition to SUMMATION by consuming items
        output = (++output ) % 20;
        pthread_cond_signal(&P_cond);                      // Signaling producer threads

        pthread_mutex_unlock(&M);
    }
    //EXIT SECTION
    pthread_exit(NULL);
}

int main() {

    //Declaring thread variables
    pthread_t Pro[2];
    pthread_t Cons[2];

    // Initialization of mutexes and conditional variables required
    pthread_mutex_init(&M, NULL);
    pthread_cond_init(&P_cond, NULL);
    pthread_cond_init(&C_cond, NULL);
    
    //Producer threads creation
    for (int k = 0; k < 2; k++) {
        pthread_create(&Pro[k], NULL, P_Fun, NULL);
    }

    // Consumer threads creation
    for (int k = 0; k < 2; k++) {
        pthread_create(&Cons[k], NULL, C_Fun, NULL);
    }

    // wait for the producer thread to finish  
    for (int k = 0; k < 2; k++) {
        pthread_join(Pro[k], NULL);
    }

    // wait for the consumer thread to finish  
    for (int k = 0; k < 2; k++) {
        pthread_join(Cons[k], NULL);
    }

    printf("SUMMATION = %d\n", SUMMATION);

    // Eliminate mutex and conditional variables created
    pthread_mutex_destroy(&M);
    pthread_cond_destroy(&P_cond);
    pthread_cond_destroy(&C_cond);

    return 0;
}