#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


void Print_vector(  double      local_vec[]     /* in  */,
                    int         local_n         /* in  */,
                    int         n               /* in  */,
                    char        title[]         /* in  */,
                    int         my_rank         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Read_vector(   double      local_a[]       /* out */,
                    int         local_n         /* in  */,
                    int         n               /* in  */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Read_n(        int*        n_p             /* out */,
                    int*        local_n_p       /* out */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

int main(void)
{
    // initialize data
    int n;
    int local_n;
    double* local_x;
    double* local_y;

    // initialize mpi
    int comm_sz;
    int my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    // program begin
    Read_n(&n, &local_n, my_rank, comm_sz, comm);
    assert(n);
    assert(local_n == n / comm_sz);

    local_x = malloc(local_n * sizeof(double));
    local_y = malloc(local_n * sizeof(double));
    Read_vector(local_x, local_n, n, my_rank, comm_sz, comm);
    Read_vector(local_y, local_n, n, my_rank, comm_sz, comm);
    
    Print_vector(local_x, local_n, n, "Vector X", my_rank, comm);
    Print_vector(local_y, local_n, n, "Vector Y", my_rank, comm);

    // finalize mpi
    MPI_Finalize();
    return 0;
}


void Print_vector(
    double      local_vec[]     /* in  */,
    int         local_n         /* in  */,
    int         n               /* in  */,
    char        title[]         /* in  */,
    int         my_rank         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    double* vec = NULL;
    int i;

    if (my_rank == 0)
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
} /* Print_vector */


void Read_vector(
    double      local_a[]       /* out */,
    int         local_n         /* in  */,
    int         n               /* in  */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    double* a = NULL;
    int i;

    if (my_rank == 0)
    {
        a = malloc(n * sizeof(double));

        printf("The calculation will be done parallel by %d processes.\n", comm_sz);
        printf("Each process works with %d elements out of %d.\n", local_n, n);

        printf("Enter the vector\n");
        fflush(stdout);
        
        for (i = 0; i < n; i++)
        {
            scanf("%lf", &a[i]);
        }

        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);

        free(a);
    }
    else
    {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    }
} /* Read_vectors */


void Read_n(
    int*        n_p             /* out */,
    int*        local_n_p       /* out */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    // read size of vectors
    if (my_rank == 0)
    {
        printf("Enter the size fo the vectors\n");
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;
    }

    // broadcast size of vectors
    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
} /* Read_n */

