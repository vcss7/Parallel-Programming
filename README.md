# Parallel-Programming
There are multiple ways to Parallel Program. The ones that will be focued on
here are: Distributed Memory Programming with Message Passing Interface and
Shared Memory Programming with PThreads


## Distributed Memory Programming with Message Passing Interface
The Message Passing Interface (MPI) is an Application Program Interface (API)
that is purposed for communicating between cores that are using the distributed
memory paradigm.


### Install open-mpi

```
sudo pacman -S openmpi        # arch-based distro
```


### Compiling and Running MPI programs
`mpicc` is a wrapper for the gcc compiler that simplifies compiling MPI 
programs. It finds the appropriate MPI header files and handles linking object
files to MPI libraries.

```
# compiling
mpicc -g -Wall -o program program.c
```

`mpiexec` starts *n* number of instances of the program and handles the
communication between them. *n* is the number of processors specified to use.

```
# running
mpiexec -n 2 --mca opal_warn_on_missing_libcuda 0 ./program
```


## Shared Memory Programming with PThreads
A shared memory system is a system that uses multiple cores (or threads) that
all have access to the same memory locations.

Some things to consider are:
1. How to synchronize threads
1. Putting threads to sleep to wait for a condition
1. Critical sections
1. Acessing blocks of code to improve parallization
1. Negatives of using cache memory

Shared Memory programming can be implemented by using POSIX threads.


### Using Pthreads API
To create a thread, we use the pthread.h API
```
int pthread_create(
    pthread_t*              thread_p                /* out */,
    const pthread_attr_t*   attr_p                  /* in  */,
    void*                   (*start_routine)(void*) /* in  */,
    void*                   args_p                  /* in  */);
```


### Install Pthreads
Pthreads are available on most POSIX compliant Unix systems.


### Compiling and Running PThread Programs
To compile, run following command. Some systems automatically link the library
and `-lpthread` is not needed.
```
gcc -g -Wall -o program.out program.c -lpthread
```

To run
```
./program.out <args>
```


### Potential Problems with Shared Memory Programming

#### Global Variables
Global variables may cause a program to not have any errors if there is a
function that uses a local variable of the same name. If, for example, a
function changes the global variable when it intended to change the local
variable, this is a logical error, but this will not produce a compile or
runtime error.

In general, limit the number of global variables.


#### Critical Sections

