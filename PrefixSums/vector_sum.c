#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


void Read_n(        int* n_p,
                    int* local_n_p,
                    int my_rank,
                    int comm_sz,
                    MPI_Comm comm);

int main(void)
{
    // initialize data
    int n;
    int local_n;

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

    // finalize mpi
    MPI_Finalize();
    return 0;
}


void Read_n(
    int* n_p,               /* out */
    int* local_n_p,         /* out */
    int my_rank,            /* in */
    int comm_sz,            /* in */
    MPI_Comm comm           /* in */)
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


