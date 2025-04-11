#!/bin/bash 

make -C ./Problem_3/
cd ./Problem_3/ && mpirun -n 4 ./merge_sort_MPI 262144  ../../../test-data/vec1_2^18.csv ./result_4p_18.csv; cd -
cd ./Problem_3/ && mpirun -n 4 ./merge_sort_MPI 524288  ../../../test-data/vec1_2^19.csv ./result_4p_19.csv; cd -
cd ./Problem_3/ && mpirun -n 4 ./merge_sort_MPI 1048576 ../../../test-data/vec1_2^20.csv ./result_4p_20.csv; cd -




