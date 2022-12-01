#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int MAX_THREADS = 1024;

long        thread_count;
int         n;
int*        vector = NULL;
int*        matrix = NULL;
int*        result = NULL;


void Usage(char* prog_name);
void Get_args(int argc, char* argv[]);
void Read_n();
void Read_vector();
void Print_vector(int v[]);
void Read_matrix();
void Print_matrix();
void* Pth_mat_vect(void* rank);

int main(int argc, char* argv[])
{
    long thread;
    pthread_t* thread_handles;
    
    Get_args(argc, argv);

    // read input n
    Read_n();

    // memory allocation
    vector = malloc(n * sizeof(int));
    matrix = malloc(n * n * sizeof(int));
    result = malloc(n * sizeof(int));

    // read input vector and matrix
    Read_vector();
    Read_matrix();

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    Print_vector(result);

    free(thread_handles);
    free(vector);
    free(matrix);
    return 0;
}


/*------------------------------------------------------------------            
* Function:    Pth_mat_vect
* Purpose:     Calculate matrix x vector multiplication with pthreads
* In args:     my_rank
* Out args:    NULL
*/ 
void* Pth_mat_vect(void* rank)
{
    long my_rank = (long) rank;
    int i;
    int j;
    int local_m = n / thread_count;
    int my_first_row = my_rank * local_m;
    int my_last_row = (my_rank + 1) * local_m - 1;

    int local_i;
    int* local_result = NULL;
    local_result = malloc(local_m * sizeof(int));

    local_i = my_last_row - my_first_row;
    for (i = my_first_row; i <= my_last_row; i++, local_i++)
    {
        local_result[local_i] = 0.0;
        for (j = 0; j < n; j++)
        {
            local_result[local_i] += matrix[i * n + j] * vector[j];
        }
    }

    for (i = my_first_row; i <= my_last_row; i++)
    {
        for (local_i = 0; local_i <= my_last_row - my_first_row; local_i++)
        {
            result[i] = local_result[local_i];
        }
    }

    return NULL;
}


/*------------------------------------------------------------------            
* Function:    Print_vector
* Purpose:     Print the Matrix to stdin
* In args:     *matrix
* Out args:    *matrix
*/ 
void Print_matrix()
{
    int i;
    int j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}


/*------------------------------------------------------------------            
* Function:    Read_matrix
* Purpose:     Get the Matrix from stdout
* In args:     *matrix
* Out args:    *matrix
*/ 
void Read_matrix()
{
    int i;
    int j;

    printf("Enter Matrix M\n");
    fflush(stdout);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &matrix[i * n + j]);
        }
    }
}


/*------------------------------------------------------------------            
* Function:    Print_vector
* Purpose:     Print vector to stdin
* In args:     vector
*/ 
void Print_vector(int v[])
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
} /* Print_vector */


/*------------------------------------------------------------------            
* Function:    Read_vector
* Purpose:     Get the Vector from stdout
* In args:     None
*/ 
void Read_vector()
{
    int i;

    printf("Enter Vector V: ");
    fflush(stdout);

    for (i = 0; i < n; i++)
    {
        scanf("%d", &vector[i]);
    }
} /* Read_vector */


/*------------------------------------------------------------------            
* Function:    Read_n
* Purpose:     Get the order of the Vector and Matrix from stdout
* In args:     None               
* Globals out: n
*/ 
void Read_n()
{
    printf("Enter the order of the Vector and Matrix: ");
    fflush(stdout);

    scanf("%d", &n);
} /* Read_n */


/*------------------------------------------------------------------            
* Function:    Get_args
* Purpose:     Get the command line args
* In args:     argc, argv
* Globals out: thread_count, n
*/ 
void Get_args(int argc, char* argv[])
{
    if (argc != 3) 
    {
        Usage(argv[0]);
    }
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) 
    {
        Usage(argv[0]);
    }
    n = strtol(argv[2], NULL, 10);
    if (n <= 0) 
    {
        Usage(argv[0]);
    }
} /* Get_args */


/*------------------------------------------------------------------
* Function:  Usage
* Purpose:   Print a message explaining how to run the program
* In arg:    prog_name
*/
void Usage(char* prog_name)
{
    fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
    fprintf(stderr, "   n is the number of terms and should be >= 1\n");
    fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
    exit(0);

} /* Usage */

