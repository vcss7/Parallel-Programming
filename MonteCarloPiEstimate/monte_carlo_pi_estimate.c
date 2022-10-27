#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void Read_n(        int*            n_p         /* out */,
                    int*            local_n_p   /* out */,
                    int             my_rank     /* in  */,
                    int             comm_sz     /* in  */,
                    MPI_Comm        comm        /* in  */);


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


void Read_n(
    int*            n_p         /* out */,
    int*            local_n_p   /* out */,
    int             my_rank     /* in  */,
    int             comm_sz     /* in  */,
    MPI_Comm        comm        /* in  */)
{
    if (my_rank == 0)
    {
        printf("Enter the number of points: ");
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
}
    
