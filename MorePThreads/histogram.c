#include <assert.h>
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables */
const int MAX_THREADS = 1024;
int thread_count;
pthread_mutex_t mutex;

int bin_count, local_bin_count, *bin_counts;
float min_meas, max_meas;
int data_count, local_data_count;
float* data;
float* bin_mins, * bin_maxes;
/*
        float bin_mins[], float bin_maxes[], int bin_count, 
        int bin_counts[], float min_meas, float max_meas);
*/

void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);
void Read_input(int *bin_count_p, int *local_bin_count_p, int *data_count_p,
        int *local_data_count_p, float *min_meas_p, float *max_meas_p);

void Generate_data(int data_count, float data[], float min_meas, float max_meas);
void Print_data(float data[], int data_count, char title[]);
void Set_subintervals(float bin_mins[], float bin_maxes[], float min_meas, 
        float max_meas, int bin_count);
void* Build_histogram(void* rank);
void Print_histogram(float bin_mins[], float bin_maxes[], int bin_count,
        int bin_counts[]);


int main(int argc, char* argv[])
{
    long thread;
    pthread_t* thread_handles;

    Get_args(argc, argv);
    Read_input(&bin_count, &local_bin_count, &data_count, &local_data_count,
            &min_meas, &max_meas);

    /* allocate data */
    data = malloc(data_count * sizeof(float));
    bin_mins = malloc(bin_count * sizeof(float));
    bin_maxes = malloc(bin_count * sizeof(float));
    bin_counts = malloc(bin_count * sizeof(int));

    Generate_data(data_count, data,  min_meas, max_meas);

    Set_subintervals(bin_mins, bin_maxes, min_meas, max_meas, bin_count);

    for (int i = 0; i < bin_count; i++)
    {
        bin_counts[i] = 0;
    }

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Build_histogram, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    Print_data(data, data_count, "histogram data");
    Print_histogram(bin_mins, bin_maxes, bin_count, bin_counts);

    /*
    local_data = malloc(local_data_count * sizeof(float));
    local_bin_count = malloc(bin_count * sizeof(float));
    */

    free(data);
    free(bin_mins);
    free(bin_maxes);
    free(bin_counts);
    free(thread_handles);
    return 0;
} /* main */


void Print_histogram(float bin_mins[], float bin_maxes[], int bin_count,
        int bin_counts[])
{
    int i;

    for (i = 0; i < bin_count; i++)
    {
        printf("bin %d (%f - %f] %d\n", i, bin_mins[i], bin_maxes[i], bin_counts[i]);
    }
}


void* Build_histogram(void* rank)
{
    long my_rank = (long) rank;
    int i;
    int j;

    int start_elem;
    int stop_elem;
    start_elem = (data_count / thread_count) * my_rank;
    stop_elem = start_elem + local_data_count;
    // go through each datum
    for (i = 0; i < local_data_count; i++)
    {
        // go through each bin
        for (j = 0; j < bin_count; j++)
        {
            if (bin_mins[j] < data[start_elem + i] && data[start_elem + i] <= bin_maxes[j])
            {
                pthread_mutex_lock(&mutex);
                bin_counts[j]++;
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    return NULL;
}


void Set_subintervals(float bin_mins[], float bin_maxes[], float min_meas, 
        float max_meas, int bin_count)
{
    float interval = (max_meas - min_meas) / (float) bin_count;

    int i;
    for (i = 0; i < bin_count; i++)
    {
        bin_mins[i] = min_meas + interval * (float) (i);
        bin_maxes[i] = min_meas + interval * (float) (i + 1);
    }
}


void Print_data(
        float data[], 
        int data_count, 
        char title[])
{
    int i;

    printf("%s\n", title);

    for (i = 0; i < data_count; i++)
    {
        printf("%f ", data[i]);
    }
    printf("\n");
}


void Generate_data(
        int data_count,
        float data[],
        float min_meas,
        float max_meas)
{
    int i;
    float rand_float;

    for (i = 0; i < data_count; i++)
    {
        rand_float = (float) rand() / (float) RAND_MAX;
        data[i] = min_meas + rand_float * (max_meas - min_meas);
    }
}


void Read_input(
        int *bin_count_p,           /* out */
        int *local_bin_count_p,     /* out */
        int *data_count_p,          /* out */
        int *local_data_count_p,    /* out */
        float *min_meas_p,          /* out */
        float *max_meas_p           /* out */)
{
    printf("Enter the min_meas, max_meas, bin_count, data_count\n");
    fflush(stdout);
    scanf("%f %f %d %d", min_meas_p, max_meas_p, bin_count_p, data_count_p);

    *local_bin_count_p = *bin_count_p / thread_count;
    *local_data_count_p = *data_count_p / thread_count;

    printf("\nThe sorting of the numbers will be done parallel\n");
    printf("This program will use %d threads\n", thread_count);
    printf("Each thread will sort %d numbers out of %d\n", *local_bin_count_p, *bin_count_p);
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
