#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void Check_for_error(int local_ok, char fname[], char message[], MPI_Comm comm);
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
        int local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(double local_vec[], int local_n, int n, char title[],
        int my_rank, MPI_Comm comm);
double Par_dot_product(double local_vec1[], double local_vec2[], int local_n,
        MPI_Comm comm);
void Par_vector_scalar_mult(double local_vec[], double scalar,
        double local_result[], int local_n);

int main(void)
{
    int n, local_n;
    double *local_vec1, *local_vec2;
    double scalar;
    double *local_scalar_mult1, *local_scalar_mult2;
    double dot_product;
    double partial_dot_product;
    int comm_sz, my_rank;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Read_n(&n, &local_n, my_rank, comm_sz, MPI_COMM_WORLD);
    Read_data(local_vec1, local_vec2, &scalar, local_n, my_rank, comm_sz, MPI_COMM_WORLD);
    Print_vector(local_vec1, local_n, n, "vector1", my_rank, MPI_COMM_WORLD);

    /* Compute and print dot proiduct */
    partial_dot_product = Par_dot_product(local_vec1, local_vec2, local_n, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        MPI_Reduce(&partial_dot_product, &dot_product, local_n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        Print_vector(local_vec1, local_n, n ,"Dot Product", my_rank, MPI_COMM_WORLD);
    }

    /* Compute and print scalar multiplication */
    double *scalar_mult1 = NULL;
    double *scalar_mult2 = NULL;

    if(my_rank == 0)
    {
        MPI_Gather(local_scalar_mult1, local_n, MPI_DOUBLE, scalar_mult1, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        Print_vector(scalar_mult1, local_n, n, "First Scalar Mult", 0, MPI_COMM_WORLD);

        MPI_Gather(local_scalar_mult2, local_n, MPI_DOUBLE, scalar_mult2, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        Print_vector(scalar_mult2, local_n, n, "Second Scalar Mult", 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Gather(local_scalar_mult1, local_n, MPI_DOUBLE, scalar_mult1, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(local_scalar_mult2, local_n, MPI_DOUBLE, scalar_mult2, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

/* Reads user input for the size of the vectors */
void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm)
{
    if (my_rank == 0)
    {
        /* grab size of vectors */
        printf("Enter the size of the Vectors: ");
        fflush(stdout);
        scanf("%d", n_p);

        /* calculate local_n_elem using comm_sz and n */
        *local_n_p = *n_p / comm_sz;
        assert(*local_n_p == (*n_p / comm_sz));
    }
 
    /* Proc = 0: send n and local_n to each process */
    /* Proc > 0: receive n and local_n */
    MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}


/*-------------------------------------------------------------------*/
void Check_for_error(int local_ok, char fname[], char message[], MPI_Comm comm)
{
    int okay = -1;

    MPI_Allreduce(&local_ok, &okay, 1, MPI_INT, MPI_MIN, comm);
    
    if (okay == 0)
    {
        int my_rank = -1;
        MPI_Comm_rank(comm, &my_rank);

        if(my_rank == 0)
        {
            fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, message);
            fflush(stderr);
        }

        MPI_Finalize();
        exit(-1);
    }
}


/* local_vec1 and local_vec2 are the two local vectors of size local_n which
 * the process pertains */
/* process 0 will take the input of the scalar, the two vectors a and b */
/* process 0 will scatter the two vectors a and b across all processes */
/*-------------------------------------------------------------------*/
void Read_data(
        double local_vec1[],
        double local_vec2[],
        double* scalar_p,
        int local_n,
        int my_rank,
        int comm_sz,
        MPI_Comm comm)
{
    // read and broadcast scalar
    if(my_rank == 0)
    {
        printf("Enter the scalar number: ");
        fflush(stdout);
        scanf("%lf", scalar_p);
    }
    MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);

    // read vector 1
    double *arr1 = NULL;
    int i;
    
    if(my_rank == 0)
    {
        arr1 = malloc(local_n * comm_sz  * sizeof(double));

        printf("Enter the first Vector of size %d: ", (local_n * comm_sz));
        fflush(stdout);
        for(i = 0; i < (local_n * comm_sz); i++)
        {
            fflush(stdout);
            scanf("%lf", &arr1[i]);
        }
        MPI_Scatter(arr1, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
    }
    else
    {
        MPI_Scatter(arr1, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
    }

    free(arr1);

    // read vector 2
    double *arr2 = NULL;
    int j;

    if(my_rank == 0)
    {
        arr2 = malloc(local_n * comm_sz * sizeof(double));

        printf("Enter the second Vector of size %d: ", (local_n * comm_sz));
        fflush(stdout);
        for(j = 0; j < (local_n * comm_sz); j++)
        {
            fflush(stdout);
            scanf("%lf", &arr2[j]);
        }
        MPI_Scatter(arr2, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
    }
    else
    {
        MPI_Scatter(arr2, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
    }

    free(arr2);
}


/* The print_vector gathers the local vectors from all processes and print the
 * gathered vector */
/*-------------------------------------------------------------------*/
void Print_vector(double local_vec[], int local_n, int n, char title[],
        int my_rank, MPI_Comm comm)
{
    double *vec = NULL;
    int i;
    
    if(my_rank == 0)
    {
        vec = malloc(n * sizeof(double));
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);

        printf("%s\n", title);
        for (i = 0; i < n; i++)
        {
            printf("%f ", vec[i]);
        }
        printf("\n");
        free(vec);
    }
    else
    {
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);
    }
}


/* This function computes and returns the partial dot product of local_vec1 and
 * local_vec2 */
/*-------------------------------------------------------------------*/
double Par_dot_product(double local_vec1[], double local_vec2[], int local_n,
        MPI_Comm comm)
{
    int i;
    double my_sum = 0;

    for(i = 0; i < local_n; i++)
    {
        my_sum = local_vec1[i] * local_vec2[i];
    }

    return my_sum;
}


/* This function gets the vector which is the scalar times local_vec, and puts
 * the vector into local_result */
/*-------------------------------------------------------------------*/
void Par_vector_scalar_mult(double local_vec[], double scalar,
        double local_result[], int local_n)
{
    int i;

    for(i = 0; i < local_n; i++)
    {
        local_result[i] = scalar * local_vec[i];
    }
}

