#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 20
#define PRODUCERS_count 2
#define CONSUMERS_count 2
#define ITERATIONS_count 30

int buffer[BUFFER_SIZE];
int input = 0;
int output = 0;
int p=0,c=0;
int SUM = 0;


pthread_mutex_t mutex;
sem_t filled,vacant;

void *Pfunc()
{
    for(p=0;p<ITERATIONS_count;p++)
    {
        sem_wait(&vacant);
        pthread_mutex_lock(&mutex);

        printf("%d\n",p);

        buffer[input] = p+1;
        input = (input + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&filled);

    }
    return 0;
}

void *Cfunc()
{
    int m;
    for(c=0;c<ITERATIONS_count;c++)
    {
        sem_wait(&filled);
        pthread_mutex_lock(&mutex);

        m = buffer[output];
        output = (output + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&vacant);

        printf("sum=%d\n",SUM);
        SUM=SUM+m;

    }
    return 0;
}

int main()
{
    int i;
    pthread_t thrdP0,thrdP1,thrdC0,thrdC1;

    pthread_mutex_init(&mutex,NULL);

    sem_init(&vacant,0,BUFFER_SIZE);
    sem_init(&filled, 0,0);

    pthread_create(&thrdP0,NULL,Pfunc,NULL);
    pthread_create(&thrdP1,NULL,Pfunc,NULL);
    pthread_create(&thrdC0,NULL,Cfunc,NULL);
    pthread_create(&thrdC1,NULL,Cfunc,NULL);

    /*for(i=0;i<CONSUMERS_count;i++)
    {
        pthread_create(&thrdP[i],NULL,Pfunc,NULL);
    }

    for(i=0;i<PRODUCERS_count;i++)
    {
        pthread_create(&thrdC[i],NULL,Pfunc,NULL);
    }*/

    pthread_join(&thrdP0,NULL);
    pthread_join(&thrdP1,NULL);
    pthread_join(&thrdC0,NULL);
    pthread_join(&thrdC1,NULL);
    /*for(i=0;i<CONSUMERS_count;i++)
    {
        pthread_join(&thrdP[i],NULL);
    }

    for(i=0;i<PRODUCERS_count;i++)
    {
        pthread_join(&thrdC[i],NULL);
    }*/

    printf("Sum value is %d",SUM);

}
