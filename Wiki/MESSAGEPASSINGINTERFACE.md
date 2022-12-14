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

### Initialization
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

### Communication

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

#### Point-to-Point Communication
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
);
```

#### Collective Communication
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
);
```

`MPI_Bcast(...)` gets data from one process and send it to all the processes
inside of communicator.

```
MPI_Bcast(
    void*           data_p          /* in/out; output data memory location */
    int             count           /* in; num data values */
    MPI_Datatype    datatype        /* in; datatype of data value(s) */
    int             source_proc     /* in; process sending data */
    MPI_Comm        communicator    /* in; communicator to use */
);
```

`MPI_Scatter(...)` divides the data referenced by the number of processes inside
the communicator and sends a part of that data to the processes.

```
MPI_Scatter(
    void*           send_buf_p      /* in; output data memory location */
    int             send_count      /* in; num data sending to each process */
    MPI_Datatype    send_type       /* in; type of data sending */
    void*           recv_buf_p      /* out; where to save data received */
    int             recv_count      /* in; num data each process receives */
    MPI_Datatype    recv_type       /* in; type of data receiving */
    int             src_proc        /* in; the processes sending the data */
    MPI_Comm        comm            /* in; the communicator to use */
);
```

`MPI_Gather(...)` gathers the data referenced and sends it to a destination process
inside the communicator.

```
MPI_Gather(
    void*           send_buf_p      /* in; memory location of data being sent */
    int             send_count      /* in; num data being sent by each process */
    MPI_Datatype    send_type       /* in; type of data sending */
    void*           recv_buf_p      /* out; memory location of gathered data */
    int             recv_count      /* in; num data from each process */
    MPI_Datatype    recv_type       /* in; type of data recv */
    int             dest_proc       /* in; destination process */
    MPI_Comm        comm            /* in; the communicator to use */
);
```

`MPI_Scan(...)` performs a prefix reduction across all MPI processes inside the
communicator.

```
MPI_Scan(
    void*           send_buf_p      /* in; memory location of data being sent */
    void*           recv_buf_p      /* out; memory location of data recved */
    int             count           /* in; num data in array */
    MPI_Datatype    datatype        /* in; datatype in array */
    MPI_Op          op              /* in; operation to perform on array */
    MPI_Comm        comm            /* in; the communicator to use */
);
```

### Derived Datatypes
Passing messages between processes can be expensive. It is cheaper to send a
single large message than it is to send multiple messages with the same amount
of data.

MPI derived datatypes are used to represent a collection of data and their data
types for the purpose of being sent between processes and hence make
communication more effecient.

`MPI_Type_contiguous(...)` can be used to build a derived datatypes of
contiguous elements in an array.

```
MPI_Type_contiguous(
    int             count           /* in; num data in the array */
    MPI_Datatype    old_mpi_t       /* in; old MPI datatype */
    MPI_Datatype    new_mpi_t_p     /* out; pointer to new mpi type */
)
```

### Finalization
`MPI_Finalize()` frees up any resources used by MPI and indicates that the
program is done using the MPI API. It is the last function that should be called
from the MPI API.

```
# MPI_Finalize forward declaration
int MPI_Finalize(void);
```

