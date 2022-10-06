#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

void Read_input(int *bin_count_p, int *local_bin_count_p, int *data_count_p,
        int *local_data_count_p, float *min_meas_p, float *max_meas_p,
        int my_rank, int comm_sz, MPI_Comm comm);
void Get_input(int *bin_count_p, float *min_meas_p, float *max_meas_p, int *data_count_p,
        int* local_data_count_p, int my_rank, int comm_sz, MPI_Comm comm);
void Generate_data(float local_data[], int local_data_count, int data_count,
        float min_meas, float max_meas, int my_rank, MPI_Comm comm);

void Print_data(float local_data[], int local_data_count, int data_count,
        char title[], int my_rank, MPI_Comm comm);

int main(void)
{
    int my_rank, comm_sz;

    int bin_count, local_bin_count;
    float min_meas, max_meas;
    int data_count, local_data_count;
    float* data, *local_data;
    //float* bin_maxes;
    //int* bin_counts, local_bin_counts;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Start of Program */

    Read_input(&bin_count, &local_bin_count, &data_count, &local_data_count,
            &min_meas, &max_meas, my_rank, comm_sz, MPI_COMM_WORLD);

    data = malloc(data_count * sizeof(float));
    local_data = malloc(local_data_count * sizeof(float));

    Generate_data(local_data, local_data_count, data_count, min_meas, max_meas, my_rank, MPI_COMM_WORLD);

    Print_data(local_data, local_data_count, data_count, "histogram data", my_rank, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
    }

    /* End of Program */

    MPI_Finalize();
    return 0;
} /* main */


void Read_input(
        int *bin_count_p,           /* out */
        int *local_bin_count_p,     /* out */
        int *data_count_p,          /* out */
        int *local_data_count_p,    /* out */
        float *min_meas_p,          /* out */
        float *max_meas_p,          /* out */
        int my_rank,
        int comm_sz,
        MPI_Comm comm)
{
    if (my_rank == 0)
    {
        printf("Enter the min_meas, max_meas, bin_count, data_count\n");
        fflush(stdout);
        scanf("%f %f %d %d", min_meas_p, max_meas_p, bin_count_p, data_count_p);

        *local_bin_count_p = *bin_count_p / comm_sz;
        *local_data_count_p = *data_count_p / comm_sz;

        printf("\nThe sorting of the numbers will be done parallel\n");
        printf("This program will use %d processes\n", comm_sz);
        printf("Each process will sort %d numbers out of %d\n", *local_bin_count_p, *bin_count_p);
    }

    MPI_Bcast(bin_count_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_bin_count_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(data_count_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(local_data_count_p, 1, MPI_INT, 0, comm);
    MPI_Bcast(min_meas_p, 1, MPI_FLOAT, 0, comm);
    MPI_Bcast(max_meas_p, 1, MPI_FLOAT, 0, comm);
}


void Generate_data(
        float local_data[],
        int local_data_count,
        int data_count,
        float min_meas,
        float max_meas,
        int my_rank,
        MPI_Comm comm)
{
    float *data;

    if (my_rank == 0)
    {
        data = malloc(data_count * sizeof(float));

        int i;
        float rand_float;

        for (i = 0; i < data_count; i++)
        {
            rand_float = (float) rand() / (float) RAND_MAX;
            data[i] = min_meas + rand_float * (max_meas - min_meas);
        }

        MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);

        free(data);
    }
    else
    {
        MPI_Scatter(data, local_data_count, MPI_FLOAT, local_data, local_data_count, MPI_FLOAT, 0, comm);
    }
}


void Print_data(float local_data[], int local_data_count, int data_count,
        char title[], int my_rank, MPI_Comm comm)
{
    float *a = NULL;
    int i;

    if (my_rank == 0)
    {
        a = malloc(data_count * sizeof(float));

        MPI_Gather(local_data, local_data_count, MPI_FLOAT, a, local_data_count,
                MPI_FLOAT, 0, comm);

        printf("%s\n", title);

        for (i = 0; i < data_count; i++)
        {
            printf("%f ", a[i]);
        }
        printf("\n");

        free(a);
    }
    else
    {
        MPI_Gather(local_data, local_data_count, MPI_FLOAT, a, local_data_count,
                MPI_FLOAT, 0, comm);
    }
}

