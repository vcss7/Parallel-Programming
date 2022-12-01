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
The `thread_p` is the address of the pthread_t object we will use. The
`start_routine` is a pointer to the funtion the thread will run. The `args_p` is
a pointer to the arguments for the thread function (i.e. `start_routine`) that
the thread will execute.

Its possible to pass multiple arguments by creating a struct type with the
argument variables and then passing the struct as the argument.

Threads created do not all need to run the same program, but throughout these
projects, the "single program, multiple data" style parallelism will be used.

To stop a node when it is done executing we use the `pthread_join(...)` function
from the pthread.h API
```
int pthread_join(
    pthread_t   thread      /* in  */,
    void**      ret_val_p   /* out */);
```

The first argument, `thread`, is the thread to join back to the main branch and
the second argument, `ret_val_p` is the location in which to store the return
value of the thread (can be `NULL`).

It is important to remember to join the threads back to the main branch because
each thread uses up some amounts of resources. If left unchecked, the thread
will hold on to those resources and perhaps prevent the creation of other
threads.


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


## OpenMP
OpenMP(Open Multiprocessing) is an API for shared-memory MIMD(Multiple
Instruction, Multiple Data). It is a system that allows each process (or thread)
access to all available memory.

OpenMP is designed to allow a programmer to easily parallelize an already
existing serial program.


### Compiling and Running an OpenMP Program
OpenMP uses preprocessor directives to allow behaviour that is not part of the C
programming language.

```
# compiling
gcc -g -Wall -fopenmp -o program.out program.c
```

How a program behaves is compiler dependent and, if, a parallel functionality is
not supported, then the program *should* run as a serial program.
```
# running 
./program.out <args>
```

### Using the OpenMP API
OpenMP provides preprocessor directives and macros that can be accessed by
including the `omp.h` header file.

