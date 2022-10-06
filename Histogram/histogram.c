#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

void Read_input(int *bin_count_p, int *local_bin_count_p, float *min_meas_p,
        float *max_meas_p, int my_rank, int comm_sz, MPI_Comm comm);
void Get_input(int *bin_count_p, float *min_meas_p, float *max_meas_p, int *data_count_p,
        int* local_data_count_p, int my_rank, int comm_sz, MPI_Comm comm);
void Generate_data(float local_data[], int local_data_count, int data_count,
        float min_meas, float max_meas, int my_rank, MPI_Comm comm);

int main(void)
{
    int my_rank, comm_sz;

    int bin_count, local_bin_count;
    float min_meas, max_meas;
    //float* bin_maxes;
    //int* bin_counts, local_bin_counts;
    //int data_count, local_data_count;
    //float* data, local_data;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Start of Program */

    Read_input(&bin_count, &local_bin_count, &min_meas, &max_meas, my_rank, comm_sz, MPI_COMM_WORLD);

    /* End of Program */

    MPI_Finalize();
    return 0;
} /* main */


void Read_input(
        int *bin_count_p,
        int *local_bin_count_p,
        float *min_meas_p,
        float *max_meas_p,
        int my_rank,
        int comm_sz,
        MPI_Comm comm)
{
    /*  Input
     *  lower_bound
     *  upper_bound
     *  num_bins
     */
    if (my_rank == 0)
    {
        printf("Enter the min_meas, max_meas, bin_count\n");
        fflush(stdout);
        scanf("%f %f %d", min_meas_p, max_meas_p, bin_count_p);

        *local_bin_count_p = *bin_count_p / comm_sz;

        printf("\nThe sorting of the numbers will be done parallel\n");
        printf("This program will use %d processes\n", comm_sz);
        printf("Each process will sort %d numbers out of %d\n", *local_bin_count_p, *bin_count_p);
    }

    MPI_Bcast(bin_count_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_bin_count_p, 1, MPI_INT, 0, comm);
}
//
//void Generate_data(float local_data[], int local_data_count, int data_count,
//        float min_meas, float max_meas, int my_rank, MPI_Comm comm)
//{
//    float *data;
//
//    if (my_rank == 0)
//    {
//        data = malloc(data_count * sizeof(float));
//
//        int i;
//        float rand_float;
//        for (i = 0; i < data_count; i++)
//        {
//            rand_float = rand() / (float) RAND_MAX;
//            data[i] = min_meas + rand_float * (max_meas - min_meas);
//        }
//
//        // check pointers
//        MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);
//
//        free(data);
//    }
//    else
//    {
//        MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);
//    }
//}

