#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void)
{
    /* initialize data */

    /* initialize mpi */
    int comm_sz;
    int my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* preogram begin */

    /* finalize mpi */
    MPI_Finalize();
    return 0;
}

