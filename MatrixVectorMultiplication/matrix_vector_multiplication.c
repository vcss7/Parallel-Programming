#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Read_input(    int*        n_p             /* out */,
                    int*        local_n_p       /* out */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Read_vector(   int         local_vec[]     /* out */,
                    int         local_n         /* in  */,
                    int         n               /* in  */,
                    char        title[]         /* in  */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

void Print_vector(  int         local_vec[]     /* in  */,
                    int         local_n         /* in  */,
                    int         n               /* in  */,
                    char        title[]         /* in  */,
                    int         my_rank         /* in  */,
                    int         comm_sz         /* in  */,
                    MPI_Comm    comm            /* in  */);

int main(void)
{
    /* initialize data */
    int n;
    int local_n;
    int* local_vec = NULL;

    /* initialize mpi */
    int comm_sz;
    int my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* preogram begin */
    Read_input(&n, &local_n, my_rank, comm_sz, comm);
    local_vec = malloc(local_n * sizeof(int));

    Read_vector(local_vec, local_n, n, "Enter vector", my_rank, comm_sz, comm);
    Print_vector(local_vec, local_n, n, "Vector V", my_rank, comm_sz, comm);

    /* finalize mpi */
    free(local_vec);
    MPI_Finalize();
    return 0;
}

void Print_vector(
    int         local_vec[]     /* in  */,
    int         local_n         /* in  */,
    int         n               /* in  */,
    char        title[]         /* in  */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    int* v = NULL;
    int i;

    MPI_Datatype MPI_VECTOR;
    MPI_Type_contiguous(local_n, MPI_INT, &MPI_VECTOR);
    MPI_Type_commit(&MPI_VECTOR);

    if (my_rank ==0)
    {
        v = malloc(n * sizeof(int));
        MPI_Gather(local_vec, 1, MPI_VECTOR, v, 1, MPI_VECTOR, 0, comm);

        printf("%s\n", title);

        for (i = 0; i < n; i++)
        {
            printf("%d ", v[i]);
        }
        printf("\n");

        free(v);
    }
    else
    {
        MPI_Gather(local_vec, 1, MPI_VECTOR, v, 1, MPI_VECTOR, 0, comm);
    }
} /* Print_vector */


void Read_vector(
    int         local_vec[]     /* out */,
    int         local_n         /* in  */,
    int         n               /* in  */,
    char        title[]         /* in  */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    int* v = NULL;
    int i;

    MPI_Datatype MPI_VECTOR;
    MPI_Type_contiguous(local_n, MPI_INT, &MPI_VECTOR);
    MPI_Type_commit(&MPI_VECTOR);

    if (my_rank == 0)
    {
        v = malloc(n * sizeof(int));

        printf("%s\n", title);
        fflush(stdout);

        for (i = 0; i < n; i++)
        {
            scanf("%d", &v[i]);
        }
        
        MPI_Scatter(v, 1, MPI_VECTOR, local_vec, 1, MPI_VECTOR, 0, comm);

        free(v);
    }
    else
    {
        MPI_Scatter(v, 1, MPI_VECTOR, local_vec, 1, MPI_VECTOR, 0, comm);
    }
} /* Read_vector */


void Read_input(
    int*        n_p             /* out */,
    int*        local_n_p       /* out */,
    int         my_rank         /* in  */,
    int         comm_sz         /* in  */,
    MPI_Comm    comm            /* in  */)
{
    if (my_rank == 0)
    {
        printf("Enter the order of the square matrix and vector\n");
        fflush(stdout);
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
} /* Read_inpit */

