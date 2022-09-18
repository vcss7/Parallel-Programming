# Parallel-Programming
The Message Passing Interface (MPI) is an Application Program Interface (API)
that is purposed for communicating between cores that are using the distributed
memory paradigm.

## Install open-mpi

```
sudo pacman -S openmpi        # arch-based distro
```

## Compiling and Running an MPI program
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

## Writing an MPI Program 

#### Initialization
`MPI_Init()` takes care of the program's setup (sets up message buffers, process
ranking). It is the first function that should be called from the MPI API.

```
# MPI_Init forward declaration
int MPI_Init(
    int*    argc_p  /* in/out */
    char*** arvc_p  /* in/out */
);
```

#### Communication
`MPI_Comm` is a special MPI type called a **communicator**. It handles the
communication between processes.


`MPI_Comm_size()` updates the second variable with the number of processes using
the communicator. 

```
int MPI_Comm_size(
    MPI_Comm    comm        /* in  */
    int*        comm_sz_p   /* out */
);
```

`MPI_Comm_rank` updates the second variable passed with the rank of the process
that is calling the communicator.

```
int MPI_Comm_rank(
    MPI_Comm    comm        /* in  */
    int*        comm_rank_p /* out */
);
```

#### Finalization
`MPI_Finalize()` frees up any resources used by MPI and indicates that the
program is done using the MPI API. It is the last function that should be called
from the MPI API.

```
# MPI_Finalize forward declaration
int MPI_Finalize(void);
```

