#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

double start_point;
double end_point;
int num_subints;


void Get_input(double* start_p, double* end_p, int* n_p);
double func(double x);


int main (void)
{
    Get_input(&start_point, &end_point, &num_subints);

    printf("start_point = %lf\n", start_point);
    printf("end_point = %lf\n", end_point);
    printf("num_subints = %d\n", num_subints);

    return 0;
}


double func(double x)
{
    return x * x;
}


void Get_input(double* start_p, double* end_p, int* n_p)
{
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", start_p, end_p, n_p);
}

