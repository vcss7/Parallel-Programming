#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, 
      MPI_Comm comm);
void Check_for_error(int local_ok, char fname[], char message[], 
      MPI_Comm comm);
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(double local_vec[], int local_n, int n, char title[], 
      int my_rank, MPI_Comm comm);
double Par_dot_product(double local_vec1[], double local_vec2[], 
      int local_n, MPI_Comm comm);
void Par_vector_scalar_mult(double local_vec[], double scalar, 
      double local_result[], int local_n);

int main(void) {
   int n, local_n;
   double *local_vec1, *local_vec2;
   double scalar;
   double *local_scalar_mult1, *local_scalar_mult2;
   double dot_product;
   int comm_sz, my_rank;
   MPI_Comm comm;
   
   
   /* Print input data */
   
   /* Print results */
   
   /* Compute and print dot product */
   
   /* Compute scalar multiplication and print out result */
   
   
   free(local_scalar_mult2);
   free(local_scalar_mult1);
   free(local_vec2);
   free(local_vec1);
   
   MPI_Finalize();
   return 0;
}

/*-------------------------------------------------------------------*/
void Check_for_error(
                int       local_ok   /* in */, 
                char      fname[]    /* in */,
                char      message[]  /* in */, 
                MPI_Comm  comm       /* in */) {
   int ok;
   
   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, 
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}  /* Check_for_error */


/* Get the input of n: size of the vectors, and then calculate local_n according to comm_sz and n */
/* where local_n is the number of elements each process obtains */
/*-------------------------------------------------------------------*/
void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, 
      MPI_Comm comm) {

}  /* Read_n */


/* local_vec1 and local_vec2 are the two local vectors of size local_n which the process pertains */
/* process 0 will take the input of the scalar, the two vectors a and b */
/* process 0 will scatter the two vectors a and b across all processes */
/*-------------------------------------------------------------------*/
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm) {
   double* a = NULL;
   int i;
   if (my_rank == 0){
      printf("What is the scalar?\n");
      scanf("%lf", scalar_p);
   }
   
   MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);
   
   if (my_rank == 0){
      a = malloc(local_n * comm_sz * sizeof(double));
      printf("Enter the first vector\n");
      for (i = 0; i < local_n * comm_sz; i++) 
         scanf("%lf", &a[i]);
   } else {
   }
}  /* Read_data */

/* The print_vector gathers the local vectors from all processes and print the gathered vector */
/*-------------------------------------------------------------------*/
void Print_vector(double local_vec[], int local_n, int n, char title[], 
      int my_rank, MPI_Comm comm) {
   double* a = NULL;
   int i;
   
   if (my_rank == 0) {
      a = malloc(n * sizeof(double));
      free(a);
   } else {
   }

}  /* Print_vector */


/* This function computes and returns the partial dot product of local_vec1 and local_vec2 */
/*-------------------------------------------------------------------*/
double Par_dot_product(double local_vec1[], double local_vec2[], 
      int local_n, MPI_Comm comm) {
}  /* Par_dot_product */


/* This function gets the vector which is the scalar times local_vec, and put the vector into local_result */
/*-------------------------------------------------------------------*/
void Par_vector_scalar_mult(double local_vec[], double scalar, 
      double local_result[], int local_n) {
}  /* Par_vector_scalar_mult */

