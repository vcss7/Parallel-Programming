#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


void Read_input(    int*        n_p             /* out */,
                    int*        local_n_p       /* out */,
                    int*        min_val         /* out */,
                    int*        max_val         /* out */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Print_vector(  int         local_vec[]     /* in  */,
                    int         local_n         /* in  */,
                    int         n               /* in  */,
                    char        title[]         /* in  */,
                    int         my_rank         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Generate_data( int         local_x[]       /* out */,
                    int         n               /* in  */,
                    int         local_n         /* in  */,
                    int         min_val         /* in  */,
                    int         max_val         /* in  */,
                    int         my_rank         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Prefix_sum(    int         x[]             /* out */,
                    int         local_x[]       /* in  */,
                    int         n               /* in  */,
                    int         local_n         /* in  */,
                    int         my_rank         /* in  */,
                    MPI_Comm    comm            /* in  */);

int main(void)
{
    /* initialize data */
    int n;
    int local_n;
    int* local_x;
    int* x;
    int min_val;
    int max_val;

    /* initialize mpi */
    int comm_sz;
    int my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* program begin */
    Read_input(&n, &local_n, &min_val, &max_val, my_rank, comm_sz, comm);
    assert(n);
    assert(local_n == n / comm_sz);

    local_x = malloc(local_n * sizeof(int));
    Generate_data(local_x, n, local_n, min_val, max_val, my_rank, comm);
    Print_vector(local_x, local_n, n, "Vector X", my_rank, comm);

    x = malloc(n * sizeof(int));
    Prefix_sum(x, local_x, n, local_n, my_rank, comm);
    Print_vector(x, local_n, n, "Prefix Sum of Vector X", my_rank, comm);

    /* finalize mpi */
    free(x);
    free(local_x);
    MPI_Finalize();
    return 0;
}


void Prefix_sum(
    int         x[]             /* out */,
    int         local_x[]       /* in  */,
    int         n               /* in  */,
    int         local_n         /* in  */,
    int         my_rank         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    int i;

    if (my_rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            x[i] = 0;
        }
    }

    MPI_Scan(local_x, x, local_n, MPI_INT, MPI_SUM, comm);
}


void Generate_data(
    int         local_x[]       /* out */,
    int         n               /* in  */,
    int         local_n         /* in  */,
    int         min_val         /* in  */,
    int         max_val         /* in  */,
    int         my_rank         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    int* data = NULL;
    int i;

    MPI_Datatype MPI_VECTOR;
    MPI_Type_contiguous(local_n, MPI_INT, &MPI_VECTOR);
    MPI_Type_commit(&MPI_VECTOR);

    if (my_rank == 0)
    {
        data = malloc(n * sizeof(int));

        for (i = 0; i < n; i++)
        {
            data[i] = min_val + rand() % (max_val - min_val);
        }

        MPI_Scatter(data, 1, MPI_VECTOR, local_x, 1, MPI_VECTOR, 0, comm);

        free(data);
    }
    else
    {
        MPI_Scatter(data, 1, MPI_VECTOR, local_x, 1, MPI_VECTOR, 0, comm);
    }
} /* Generate_data */


void Print_vector(
    int         local_vec[]     /* in  */,
    int         local_n         /* in  */,
    int         n               /* in  */,
    char        title[]         /* in  */,
    int         my_rank         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    int* vec = NULL;
    int i;

    MPI_Datatype MPI_VECTOR;
    MPI_Type_contiguous(local_n, MPI_INT, &MPI_VECTOR);
    MPI_Type_commit(&MPI_VECTOR);

    if (my_rank == 0)
    {
        vec = malloc(n * sizeof(int));

        MPI_Gather(local_vec, 1, MPI_VECTOR, vec, 1, MPI_VECTOR, 0, comm);

        printf("%s\n", title);

        for (i = 0; i < n; i++)
        {
            printf("%d ", vec[i]);
        }
        printf("\n");

        free(vec);
    }
    else
    {
        MPI_Gather(local_vec, 1, MPI_VECTOR, vec, 1, MPI_VECTOR, 0, comm);
    }
} /* Print_vector */


void Read_input(
    int*        n_p             /* out */,
    int*        local_n_p       /* out */,
    int*        min_val         /* out */,
    int*        max_val         /* out */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    // read size of vectors
    if (my_rank == 0)
    {
        printf("Enter the size of the vectors\n");
        fflush(stdout);
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;

        printf("Enter the min_val and max_val\n");
        fflush(stdout);
        scanf("%d %d", min_val, max_val);
    }

    // broadcast input
    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(min_val, 1, MPI_INT, 0, comm);
    MPI_Bcast(max_val, 1, MPI_INT, 0, comm);
} /* Read_n */

