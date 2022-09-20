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

        printf("local_n_p = ", local_n_p);
        
        /* todo: send local_n to each process */
        /* todo: implement via broadcast or send */
        MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(local_n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
}





