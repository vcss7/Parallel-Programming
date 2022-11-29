#include <assert.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void Read_n(        int*            n_p         /* out */,
                    int*            local_n_p   /* out */,
                    int             my_rank     /* in  */,
                    int             comm_sz     /* in  */,
                    MPI_Comm        comm        /* in  */);

void Toss(          int             local_n     /* in  */,
                    int*            local_count /* out */,
                    int             my_rank     /* in  */,
                    int             comm_sz     /* in  */,
                    MPI_Comm        comm        /* in  */);


int main(void)
{
    /* initizalize data */
    int n;
    int local_n;
    int local_count;
    int count;
    double pi_estimate;

    /* initialize mpi */
    int my_rank;
    int comm_sz;

    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    /* program */
    Read_n(&n, &local_n, my_rank, comm_sz, comm);
    
    Toss(local_n, &count, my_rank, comm_sz, comm);

    if (my_rank == 0)
    {
        printf("count is %d\n", count);

        pi_estimate = 4 * count / ((double) n); 

        printf("pi_estimate is %lf\n", pi_estimate);
    }
    /* free allocated data */

    /* finalize mpi */
    MPI_Finalize();

    return 0;
} /* main */


void Toss(
    int             local_n     /* in  */,
    int*            count_p     /* out */,
    int             my_rank     /* in  */,
    int             comm_sz     /* in  */,
    MPI_Comm        comm        /* in  */)
{
    srand(1234321 + my_rank);

    int i;
    double range = 2;
    double div = RAND_MAX / range;
    double x, y;
    double dist_sq;

    int local_count = 0;

    for (i = 0; i < local_n; i++)
    {
        x = -1 + (rand() / div);
        y = -1 + (rand() / div);

        dist_sq = sqrt(x * x + y * y);
        
        if (dist_sq <= 1)
        {
            local_count++;
        }
    }

    MPI_Reduce(&local_count, count_p, 1, MPI_INT, MPI_SUM, 0, comm);
}


void Read_n(
    int*            n_p         /* out */,
    int*            local_n_p   /* out */,
    int             my_rank     /* in  */,
    int             comm_sz     /* in  */,
    MPI_Comm        comm        /* in  */)
{
    if (my_rank == 0)
    {
        printf("Enter the number of tosses: ");
        fflush(stdout);
        scanf("%d", n_p);

        *local_n_p = *n_p / comm_sz;
    }

    MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_n_p, 1, MPI_INT, 0, comm);
}
    
