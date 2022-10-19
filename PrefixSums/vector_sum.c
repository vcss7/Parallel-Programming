#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


int main(void)
{
    int comm_sz;
    int my_rank;

    MPI_Comm comm;

    // initialize mpi
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // program begin

    // finalize mpi
    MPI_Finalize();
    return 0;
}

