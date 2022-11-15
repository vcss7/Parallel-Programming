# Parallel-Programming
There are multiple ways to Parallel Program. The ones that will be focued on
here are: Distributed Memory Programming with Message Passing Interface Shared
Memory Programming with PThreads

## Distributed Memory Programming with Message Passing Interface
The Message Passing Interface (MPI) is an Application Program Interface (API)
that is purposed for communicating between cores that are using the distributed
memory paradigm.


### Install open-mpi

```
sudo pacman -S openmpi        # arch-based distro
```


### Compiling and Running an MPI program
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

