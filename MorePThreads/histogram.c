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

void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);
void Read_input(int *bin_count_p, int *local_bin_count_p, int *data_count_p,
        int *local_data_count_p, float *min_meas_p, float *max_meas_p);

void Generate_data(int data_count, float data[], float min_meas, float max_meas);
void Print_data(float data[], int data_count, char title[]);

/*
void Set_subintervals(float bin_mins[], float bin_maxes[], float min_meas, 
        float max_meas, int bin_count, int my_rank, MPI_Comm comm);
void Build_histogram(float local_data[], int local_data_count, float bin_mins[],
        float bin_maxes[], int bin_count, int bin_counts[], float min_meas,
        float max_meas);
void Print_histogram(float bin_mins[], float bin_maxes[], int bin_count,
        int bin_counts[], int my_rank, MPI_Comm comm);
*/


int main(int argc, char* argv[])
{
    int bin_count, local_bin_count, *bin_counts;
    float min_meas, max_meas;
    int data_count, local_data_count;
    float* data, *local_data;
    float* bin_mins, * bin_maxes;
    //int* bin_counts, local_bin_counts;

    /*
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    */

    /* Start of Program */

    Get_args(argc, argv);
    Read_input(&bin_count, &local_bin_count, &data_count, &local_data_count,
            &min_meas, &max_meas);

    data = malloc(data_count * sizeof(float));
    Generate_data(data_count, data,  min_meas, max_meas);
    Print_data(data, data_count, "init data");

    /*
    local_data = malloc(local_data_count * sizeof(float));
    bin_mins = malloc(bin_count * sizeof(float));
    bin_maxes = malloc(bin_count * sizeof(float));
    bin_counts = malloc(bin_count * sizeof(int));
    local_bin_count = malloc(bin_count * sizeof(float));

    for (int i = 0; i < bin_count; i++)
    {
        bin_counts[i] = 0;
    }


    Set_subintervals(bin_mins, bin_maxes, min_meas, max_meas, bin_count, my_rank, MPI_COMM_WORLD);
    Build_histogram(local_data, local_data_count, bin_mins, bin_maxes,
            bin_count, bin_counts, min_meas, max_meas);

    Print_data(local_data, local_data_count, data_count, "histogram data", my_rank, MPI_COMM_WORLD);
    Print_histogram(bin_mins, bin_maxes, bin_count, bin_counts, my_rank, MPI_COMM_WORLD);
    */


    //MPI_Finalize();

    free(data);
    return 0;
} /* main */


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

/*
void Print_histogram(float bin_mins[], float bin_maxes[], int bin_count,
        int bin_counts[], int my_rank, MPI_Comm comm)
{
    int i;

    int *histogram_bin = malloc(bin_count * sizeof(int));

    for (i = 0; i < bin_count; i++)
    {
        histogram_bin[i] = 0;
        MPI_Reduce(&bin_counts[i], &histogram_bin[i], 1, MPI_INT, MPI_SUM, 0, comm);
        if (my_rank == 0)
        {
            printf("bin %d (%f - %f] %d\n", i, bin_mins[i], bin_maxes[i], histogram_bin[i]);
        }
    }
}


void Build_histogram(float local_data[], int local_data_count, float bin_mins[],
        float bin_maxes[], int bin_count, int bin_counts[], float min_meas,
        float max_meas)
{
    int i;
    int j;

    // go through each datum
    for (i = 0; i < local_data_count; i++)
    {
        // go through each bin
        for (j = 0; j < bin_count; j++)
        {
            if (bin_mins[j] < local_data[i] && local_data[i] <= bin_maxes[j])
            {
                bin_counts[j]++;
            }
        }
    }

}


void Set_subintervals(float bin_mins[], float bin_maxes[], float min_meas, 
        float max_meas, int bin_count, int my_rank, MPI_Comm comm)
{
    float interval = (max_meas - min_meas) / (float) bin_count;

    int i;
    for (i = 0; i < bin_count; i++)
    {
        bin_mins[i] = min_meas + interval * (float) (i);
        bin_maxes[i] = min_meas + interval * (float) (i + 1);
    }
}
*/


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
