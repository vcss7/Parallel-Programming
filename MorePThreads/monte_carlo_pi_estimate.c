#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX_THREADS = 1024;

int thread_count;
int num_tosses;
int local_num_tosses;
int num_in_circle;
pthread_mutex_t mutex;

void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);
void Read_n();
void* Pth_toss(void* my_rank);


int main(int argc, char* argv[])
{
    /* initizalize data */
    num_in_circle = 0;
    long        thread;
    pthread_t*  thread_handles;
    double pi_estimate;

    /* program */
    Get_args(argc, argv);
    Read_n();
    local_num_tosses = num_tosses / thread_count;

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Pth_toss, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pi_estimate = 4 * num_in_circle / ((double) num_tosses);

    printf("pi estimate is %lf\n", pi_estimate);

    free(thread_handles);
    return 0;
} /* main */

void* Pth_toss(void* rank)
{
    long my_rank = (long) rank;

    srand(1234321 + my_rank);

    int i;
    double range = 2;
    double div = RAND_MAX / range;
    double x, y;
    double dist_sq;

    int local_count = 0;

    for (i = 0; i < local_num_tosses; i++)
    {
        x = -1 + (rand() / div);
        y = -1 + (rand() / div);

        dist_sq = sqrt(x * x + y * y);
        
        if (dist_sq <= 1)
        {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    num_in_circle += local_count;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


void Read_n()
{
    printf("Enter the number of tosses: ");
    fflush(stdout);

    scanf("%d", &num_tosses);
}
    
void Get_args(int argc, char* argv[]) {
     if (argc != 2) Usage(argv[0]);
     thread_count = strtol(argv[1], NULL, 10);
     if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
 }  /* Get_args */
 
 
 void Usage(char* prog_name)
 {
     fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
     exit(0);
} /* Usage */
