#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

const int MAX_THREADS = 1024;

long thread_count;

double start_point;
double end_point;
int num_subints;
int local_num_subints;
double total;



void Get_input(double* start_p, double* end_p, int* n_p);
double Func(double x);
void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);


int main (int argc, char* argv[])
{
    double height;

    Get_args(argc, argv);

    height = (end_point - start_point) / num_subints;
    local_num_subints = num_subints / thread_count;
    

    return 0;
}


void* Trap(void* rank)
{
    long my_rank = (long) rank;
    double estimate, x;
    int i;

    double local_start_point;
    double local_end_point;


    return NULL;
}


double Func(double x)
{
    return x * x;
}


void Get_input(double* start_p, double* end_p, int* n_p)
{
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", start_p, end_p, n_p);
}

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
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

