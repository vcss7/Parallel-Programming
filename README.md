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
`MPI_Init(...)` takes care of the program's setup (sets up message buffers,
process ranking). It is the first function that should be called from the MPI
API.

```
# MPI_Init forward declaration
int MPI_Init(
    int*    argc_p  /* in/out */
    char*** arvc_p  /* in/out */
);
```

#### Communication

`MPI_Comm` is a special MPI type called a **communicator**. It handles the
communication between processes. Effectively, a communicator is a collection of
processes that can communicate with eachother.


`MPI_Comm_size(...)` updates the second variable with the number of processes
using the communicator. 

```
int MPI_Comm_size(
    MPI_Comm    comm        /* in  */
    int*        comm_sz_p   /* out */
);
```

`MPI_Comm_rank(...)` updates the second variable passed with the rank of the
process that is calling the communicator.

```
int MPI_Comm_rank(
    MPI_Comm    comm        /* in  */
    int*        comm_rank_p /* out */
);
```

##### Point-to-Point Communication
Point-to-Point communication is communication between two processes.

`MPI_Send(...)` sends messages between processes.

```
int MPI_Send(
    void            msg_buf_p       /* in; a pointer to the msg to send */
    int             msg_size        /* in; the size of the msg */
    MPI_Datatype    msg_type        /* in; the msg type to send */
    int             dest            /* in; what process to send the msg to */
    int             tag             /* in; used to distinguish data */
    MPI_Comm        communicator    /* in; the communicator being used */
);
```

`MPI_Recv(...)` receives messages from a process.

```
int MPI_Recv(
    void*           msg_buf_p       /* out; pointer to the msg being received */
    int             buf_size        /* in;  num objects the msg can store */
    MPI_Datatype    buf_type        /* in;  the msg type received */
    int             source          /* in;  what process sent the msg */
    int             tag             /* in;  used to distinguish data */
    MPI_Comm        communicator    /* in;  the communicator being used */
    MPI_Status      status_p        /* out; */
```

##### Collective Communication
Collective communication is communication between a collection of processes.
MPI collective communicaiton functions are optimized for their respective
operation to offload the optimization from the programmer.

`MPI_Reduce(...)` collects data from all processes in a communicator and runs a
user-specified operator to converge the data into one process.

```
MPI_Reduce(
    void*           input_data_p    /* in; input data memory location */
    void*           output_data_p   /* out; output data memory location */
    int             count           /* in; num data values */
    MPI_Datatype    datatype        /* in; datatype of data values */
    MPI_Op          operator        /* in; operation to do */
    int             dest_process    /* in; destination process */
    MPI_Comm        communicator    /* in; communicator to use */
```

`MPI_Bcast(...)` gets data from one process and send it to all the processes
inside of communicator.

```
MPI_Bcast(
    void*           data_p          /* in/out; output data memory location */
    int             count           /* in; num data values */
    MPI_Datatype    datatype        /* in; datatype of data value(s) */
    int             source_proc     /* in; process sending data */
    MPI_Comm        communicator    /* in; communcator to use */
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

