#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* thread variables */
const uint16_t MAX_THREADS = 1024;
uint16_t num_threads;

/* linked list data types */
struct list_node_s {
    int data;
    struct list_node_s* next;
};

/* linked list variables */
const uint16_t NUM_ITEMS_INIT = 50;
uint16_t num_tasks;
struct list_node_s* head_p;

/* function forward declarations */
int insert(int value, struct list_node_s** head_pp);
void print_list();
void gen_rand_sorted_linked_list();
void get_args(int argc, char* argv[]);
void usage(char* prog_name);


/* main program */
int main(int argc, char* argv[])
{
    get_args(argc, argv);

    gen_rand_sorted_linked_list();

    print_list();

    return 0;
} /* main */


/* -----------------------------------------------------------------------------
 * Function:    insert
 * Purpose:     insert a linked list node into a linked list
 * In Args:     value
 *              head_pp
 * Out Args:    a linked list node
 */
int insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL) /* New first node */
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/* -----------------------------------------------------------------------------
 * Function:    print_list
 * Purpose:     print the linked list to stdout
 * In Args:     none
 * Out Args:    none
 */
void print_list()
{
    struct list_node_s* curr_p = head_p;

    while (curr_p != NULL)
    {
        printf("%d ", curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}


/* -----------------------------------------------------------------------------
 * Function:    gen_rand_sorted_linked_list
 * Purpose:     generate an initial sorted linked list with random values
 * In Args:     None
 * Out Args:    linked_list
 */
void gen_rand_sorted_linked_list()
{
    int new_value;
#pragma unroll
    for (int i = 0; i < NUM_ITEMS_INIT; i++)
    {
        new_value = rand() % 100;
        insert(new_value, &head_p);
    }
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
} /* get_args */


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

