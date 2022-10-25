#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Read_n(        int*            n_p             /* out */,
                    int*            local_n_p       /* out */,
                    int             my_rank         /* in  */,
                    int             comm_sz         /* in  */,
                    MPI_Comm        comm            /* in  */);

void Read_vector(   int             local_vec[]     /* out */,
                    int             n               /* in  */,
                    int             my_rank         /* in  */,
                    MPI_Datatype    MPI_VECTOR      /* in  */,
                    MPI_Comm        comm            /* in  */);

void Print_vector(  int             local_vec[]     /* in  */,
                    int             n               /* in  */,
                    int             my_rank         /* in  */,
                    MPI_Datatype    MPI_VECTOR      /* in  */,
                    MPI_Comm        comm            /* in  */);

void Read_matrix(   int             local_matrix[]  /* out */,
                    int             n               /* in  */,
                    int             my_rank         /* in  */,
                    MPI_Datatype    MPI_MATRIX      /* in  */,
                    MPI_Comm        comm            /* in  */);

void Print_matrix(  int             local_m[]       /* in  */,
                    int             n               /* in  */,
                    int             my_rank         /* in  */,
                    MPI_Datatype    MPI_MATRIX      /* in  */,
                    MPI_Comm        comm            /* in  */);

int main(void)
{
    /* initialize data */
    int n;
    int local_n;
    int* local_vec = NULL;
    int* local_matrix = NULL;

    /* initialize mpi */
    int comm_sz;
    int my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* preogram begin */
    if (my_rank == 0)
    {
        printf("Enter order of Vector and Matrix: ");
        fflush(stdout);
    }
    Read_n(&n, &local_n, my_rank, comm_sz, comm);
    local_vec = malloc(local_n * sizeof(int));
    local_matrix = malloc(local_n * local_n * sizeof(int));

    // custom mpi types
    MPI_Datatype MPI_VECTOR;
    MPI_Type_contiguous(local_n, MPI_INT, &MPI_VECTOR);
    MPI_Type_commit(&MPI_VECTOR);

    MPI_Datatype MPI_MATRIX;
    MPI_Type_contiguous(local_n * local_n, MPI_INT, &MPI_MATRIX);
    MPI_Type_commit(&MPI_MATRIX);

    // grab vector
    if (my_rank == 0) 
    {
        printf("Enter Vector V: ");
        fflush(stdout);
    }
    Read_vector(local_vec, n, my_rank, MPI_VECTOR, comm);

    //grab matrix
    if (my_rank == 0)
    {
        printf("Enter Matrix M\n");
    }
    Read_matrix(local_matrix, n, my_rank, MPI_MATRIX, comm);

    // print data to verify
    if (my_rank == 0)
    {
        printf("Vector V: ");
    }
    Print_vector(local_vec, n, my_rank, MPI_VECTOR, comm);

    if (my_rank == 0)
    {
        printf("Matrix M\n========\n");
    }
    Print_matrix(local_matrix, n, my_rank, MPI_MATRIX, comm);

    /* finalize mpi */
    free(local_vec);
    MPI_Finalize();
    return 0;
} /* main */


void Print_matrix(
    int             local_m[]       /* in  */,
    int             n               /* in  */,
    int             my_rank         /* in  */,
    MPI_Datatype    MPI_MATRIX      /* in  */,
    MPI_Comm        comm            /* in  */)
{
    int* m = NULL;
    int i;
    int j;

    if (my_rank == 0)
    {
        m = malloc(n * n * sizeof(int));

        MPI_Gather(local_m, 1, MPI_MATRIX, m, 1, MPI_MATRIX, 0, comm);

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", m[i * n + j]);
            }
            printf("\n");
        }
    }
    else
    {
        MPI_Gather(local_m, 1, MPI_MATRIX, m, 1, MPI_MATRIX, 0, comm);
    }
} /* Print_matrix */

void Read_matrix(
    int             local_m[]       /* out */,
    int             n               /* in  */,
    int             my_rank         /* in  */,
    MPI_Datatype    MPI_MATRIX      /* in  */,
    MPI_Comm        comm            /* in  */)
{
    int* m = NULL;
    int i;
    int j;

    if (my_rank == 0)
    {
        m = malloc(n * n * sizeof(int));
        
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                scanf("%d", &m[i * n + j]);
            }
        }

        MPI_Scatter(m, 1, MPI_MATRIX, local_m, 1, MPI_MATRIX, 0, comm);

        free(m);
    }
    else
    {
        MPI_Scatter(m, 1, MPI_MATRIX, local_m, 1, MPI_MATRIX, 0, comm);
    }
} /* Read_matrix */


void Print_vector(
    int             local_vec[]     /* in  */,
    int             n               /* in  */,
    int             my_rank         /* in  */,
    MPI_Datatype    MPI_VECTOR      /* in  */,
    MPI_Comm        comm            /* in  */)
{
    int* v = NULL;
    int i;

    if (my_rank ==0)
    {
        v = malloc(n * sizeof(int));
        MPI_Gather(local_vec, 1, MPI_VECTOR, v, 1, MPI_VECTOR, 0, comm);

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
    int             local_vec[]     /* out */,
    int             n               /* in  */,
    int             my_rank         /* in  */,
    MPI_Datatype    MPI_VECTOR      /* in  */,
    MPI_Comm        comm            /* in  */)
{
    int* v = NULL;
    int i;

    if (my_rank == 0)
    {
        v = malloc(n * sizeof(int));

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


void Read_n(
    int*            n_p             /* out */,
    int*            local_n_p       /* out */,
    int             my_rank         /* in  */,
    int             comm_sz         /* in  */,
    MPI_Comm        comm            /* in  */)
{
    if (my_rank == 0)
    {
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
} /* Read_n */

