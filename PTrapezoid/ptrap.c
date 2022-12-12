#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* Thread values */
const int MAX_THREADS = 1024;
long thread_count;
pthread_mutex_t mutex;

/* global variables */
double height;
double start_point;
double end_point;
int num_subints;
int local_num_subints;
double total_integral;


/* functions */
void* Trap(void* rank);
double Func(double x);
void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);


int main (int argc, char* argv[])
{
    Get_args(argc, argv);

    total_integral = 0;

    height = (end_point - start_point) / num_subints;
    local_num_subints = num_subints / thread_count;

    long thread;
    pthread_t* thread_handles;
    
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Trap, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    printf("Trapeziod Rule Area Estimateion: %lf\n", total_integral);

    free(thread_handles);

    return 0;
}


/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use as a working function for a thread to calculate a section of
 *              the area under its section of a function
 * In args:     rank
 * Globals out: 
 */
void* Trap(void* rank)
{
    long my_rank = (long) rank;
    double estimate, x;
    int i;

    double local_start_point;
    double local_end_point;

    local_start_point = start_point + my_rank * local_num_subints * height;
    local_end_point = local_start_point + local_num_subints * height;
    
    printf("Th %ld > start: %lf | end: %lf\n", my_rank, local_start_point, local_end_point);

    estimate = 0;
    estimate = (Func(local_end_point) + Func(local_start_point)) / 2.0;
    for (i = 1; i < local_num_subints; i++)
    {
        x = local_start_point + i * height;
        estimate += Func(x);
    }
    estimate *= height;

    printf("Th %ld > estimate: %lf\n", my_rank, estimate);

    pthread_mutex_lock(&mutex);
    total_integral += estimate;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}


/*------------------------------------------------------------------
 * Function:    Func
 * Purpose:     Get some input and return the output of the defined function
 * In args:     x
 * Globals out:
 */
double Func(double x)
{
    return x * x;
}


/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, num_subints, start_point, end_point
 */
void Get_args(int argc, char* argv[]) {
    if (argc != 5)
        Usage(argv[0]);

    thread_count = strtol(argv[1], NULL, 10);  
    if (thread_count <= 0 || thread_count > MAX_THREADS)
        Usage(argv[0]);

    sscanf(argv[2], "%d", &num_subints);
    if (num_subints <= 0)
        Usage(argv[0]);

    sscanf(argv[3], "%lf", &start_point);
    sscanf(argv[4], "%lf", &end_point);
}  /* Get_args */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <num_threads> <num_subints> <start_point> <end_point> \n", prog_name);
    fprintf(stderr, "   num_threads is the number of threads\n");
    fprintf(stderr, "   num_subints is the number of subintervals\n");
    fprintf(stderr, "   num_subints should be evenly divisible by the number of threads\n");
    fprintf(stderr, "   start_point is the start of the interval\n");
    fprintf(stderr, "   end_point is the end of the interval\n");
    exit(0);
}  /* Usage */

