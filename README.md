# Parallel-Programming

## Install open-mpi

```
sudo pacman -S openmpi        # arch-based distro
```

## Compiling and Running an MPI program

```
# compiling
mpicc -g -Wall -o program program.c

# running
mpiexec -n 2 --mca opal_warn_on_missing_libcuda 0 ./program
```

