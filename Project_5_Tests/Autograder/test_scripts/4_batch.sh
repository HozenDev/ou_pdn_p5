#!/bin/bash 

make -C ./Problem_4/
cd ./Problem_4/ && mpirun -n 2  ./pi_MPI ./result_2p.csv  ./time_2p.csv;  cd -
cd ./Problem_4/ && mpirun -n 4  ./pi_MPI ./result_4p.csv  ./time_4p.csv;  cd -
cd ./Problem_4/ && mpirun -n 8  ./pi_MPI ./result_8p.csv  ./time_8p.csv;  cd -
cd ./Problem_4/ && mpirun -n 16 ./pi_MPI ./result_16p.csv ./time_16p.csv; cd -

