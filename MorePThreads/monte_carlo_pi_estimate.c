#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX_THREADS = 1024;

int num_tosses;
int num_in_circle;
double pi_estimate;

void Read_n();
void Pth_toss();


int main(void)
{
    /* initizalize data */

    /* program */
    Read_n();

    return 0;
} /* main */

void Pth_toss(void my_rank)
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
*/


void Read_n()
{
    printf("Enter the number of tosses: ");
    fflush(stdout);

    scanf("%d", &num_tosses);
}
    
