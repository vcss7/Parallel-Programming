#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* thread variables */
const uint16_t MAX_THREADS = 1024;
uint16_t num_threads;

/* linked list variables */
uint16_t num_tasks;

/* function forward declarations */
void get_args(int argc, char* argv[]);
void usage(char* prog_name);


/* main program */
int main(int argc, char* argv[])
{
    get_args(argc, argv);

    return 0;
}


/* -----------------------------------------------------------------------------
 * Function:    get_args
 * Purpose:     assign stdin args to appropriate variables
 * In Args:     argc
 *              argv
 * Out Args:    num_tasks
 *              thread_count
 */
void get_args(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
    }

    num_threads = strtol(argv[1], NULL, 10);
    if (num_threads < 0 || num_threads > MAX_THREADS)
    {
        usage(argv[0]);
    }

    num_tasks = strtol(argv[2], NULL, 10);
    if (num_tasks < 0)
    {
        usage(argv[0]);
    }
}


/* -----------------------------------------------------------------------------
 * Function:    usage
 * Purpose:     print to stderr to show user how to use the program
 * In Args:     prog_name
 * Out Args:    None
 */
void usage(char* prog_name)
{
    fprintf(stderr, "usage: %s <num_threads> <num_tasks>\n", prog_name);
    fprintf(stderr, "   num_threads is the number of threads\n");
    fprintf(stderr, "   num_threads range: 0 < num_threads < 1024");
    fprintf(stderr, "   num_tasks is the number of to create\n");
    exit(0);
} /* usage */

