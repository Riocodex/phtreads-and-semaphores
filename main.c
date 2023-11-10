#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>



//2)Using busy waiting 
// char *messages[3] = {NULL, NULL, NULL};
// int turn = 0;
// void *messenger(void *p)
// {
//     long tid = (long)p;
//     char tmpbuf[100];

//     for (int i = 0; i < 10; i++)
//     {
//         while (turn != tid)
//         {
//             // Busy-wait until it's this thread's turn
//             usleep(1000);
//         }

//         /* Sending a message */
//         long int dest = (tid + 1) % 3;
//         sprintf(tmpbuf, "Hello from Thread %ld!", tid);
//         char *msg = strdup(tmpbuf);
//         messages[dest] = msg;
//         printf("Thread %ld sent the message to Thread %ld\n", tid, dest);

//         turn = (turn + 1) % 3; // Set the turn for the next thread

//         /* Receiving a message */
//         while (turn != tid)
//         {
//             // Busy-wait until it's this thread's turn
//             usleep(1000);
//         }

//         printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
//         free(messages[tid]);
//         messages[tid] = NULL;

//         turn = (turn + 1) % 3; // Set the turn for the next thread
//     }
//     return NULL;
// }

//3)using pmuttex
// char *messages[3] = {NULL, NULL, NULL};
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// int turn = 0;

// void *messenger(void *p)
// {
//     long tid = (long)p;
//     char tmpbuf[100];

//     for (int i = 0; i < 10; i++)
//     {
//         pthread_mutex_lock(&mutex);

//         /* Sending a message */
//         long int dest = (tid + 1) % 3;
//         sprintf(tmpbuf, "Hello from Thread %ld!", tid);
//         char *msg = strdup(tmpbuf);
//         messages[dest] = msg;
//         printf("Thread %ld sent the message to Thread %ld\n", tid, dest);

//         pthread_mutex_unlock(&mutex);

//         /* Receiving a message */
//         pthread_mutex_lock(&mutex);

//         printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
//         free(messages[tid]);
//         messages[tid] = NULL;

//         pthread_mutex_unlock(&mutex);
//     }
//     return NULL;
// }



//4)Using semaphores

char *messages[3] = {NULL, NULL, NULL};
sem_t semaphores[3];

void *messenger(void *p)
{
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++)
    {
        /* Sending a message */
        long int dest = (tid + 1) % 3;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        sem_wait(&semaphores[dest]);
        messages[dest] = msg;
        printf("Thread %ld sent the message to Thread %ld\n", tid, dest);
        sem_post(&semaphores[dest]);

        /* Receiving a message */
        sem_wait(&semaphores[tid]);
        printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
        free(messages[tid]);
        messages[tid] = NULL;
        sem_post(&semaphores[tid]);
    }
    return NULL;
}



int main()
{

    // 1) Ths program has a race conditions and  may not work as expected. The issue lies in the order of execution of threads, and the program does not enforce the desired sequence of message passing.

    // 2)using busy waiting to correct the program
    // pthread_t thrID1, thrID2, thrID3;

    // pthread_create(&thrID1, NULL, messenger, (void *)0);
    // pthread_create(&thrID2, NULL, messenger, (void *)1);
    // pthread_create(&thrID3, NULL, messenger, (void *)2);
    // pthread_join(thrID1, NULL);
    // pthread_join(thrID2, NULL);
    // pthread_join(thrID3, NULL);

    //3) Using mutex
    // pthread_t thrID1, thrID2, thrID3;

    // pthread_create(&thrID1, NULL, messenger, (void *)0);
    // pthread_create(&thrID2, NULL, messenger, (void *)1);
    // pthread_create(&thrID3, NULL, messenger, (void *)2);

    // pthread_join(thrID1, NULL);
    // pthread_join(thrID2, NULL);
    // pthread_join(thrID3, NULL);

    // pthread_mutex_destroy(&mutex);


    //4)Using semaphores
     pthread_t thrID1, thrID2, thrID3;

    // Initialize semaphores
    for (int i = 0; i < 3; ++i)
    {
        sem_init(&semaphores[i], 0, 1); // 1 is the initial value of the semaphore
    }

    pthread_create(&thrID1, NULL, messenger, (void *)0);
    pthread_create(&thrID2, NULL, messenger, (void *)1);
    pthread_create(&thrID3, NULL, messenger, (void *)2);

    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);
    pthread_join(thrID3, NULL);

    // Destroy semaphores
    for (int i = 0; i < 3; ++i)
    {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}