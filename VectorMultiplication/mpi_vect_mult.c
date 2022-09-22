#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);

int main(void)
{
    int n_elem = 0;
    int local_n_elem = 0;
    int my_rank = -1;
    int comm_sz = -1;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    Read_n(&n_elem, &local_n_elem, my_rank, comm_sz, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}

/* Reads user input for the size of the vectors */
void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm)
{
    if (my_rank == 0)
    {
        /* grab size of vectors */
        assert(my_rank == 0);
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


void Print_vector(double local_vec[], int local_n, int n, char title[],
        int my_rank, MPI_Comm comm)
{
    double *vec = NULL;
    
    if(my_rank == 0)
    {
        vec = malloc(n * sizeof(double));
        MPI_Gather(local_vec, local_n, MPI_DOUBLE, vec, local_n, MPI_DOUBLE, 0, comm);

        printf("%s\n", title);
        for (int i = 0; i < n; i++)
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


