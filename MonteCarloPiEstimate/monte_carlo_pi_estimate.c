#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    /* initizalize data */

    /* initialize mpi */
    int my_rank;
    int comm_sz;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* program */

    /* free allocated data */

    /* finalize mpi */
    MPI_Finalize();

    return 0;
} /* main */

