#!/bin/bash
#SBATCH --partition=oucspdn_cpu
#SBATCH --ntasks-per-node=8
#SBATCH --ntasks=8
#
#SBATCH --time=00:15:00
#
# --------------------- Change the items below here! ---------------------
#
#SBATCH --mail-user=Enzo.B.Durel-1@ou.edu
#SBATCH --mail-type=ALL
#
#SBATCH --job-name=hw5prob2
#
#SBATCH --chdir=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_2
#SBATCH --output=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_2/results/dot_%J_stdout.txt
#SBATCH --error=/home/oucspdn025/ou_pdn_p5/code/Durel_Enzo_Project_5/Problem_2/results/dot_%J_stderr.txt
#
#################################################

echo "Working directory:"
pwd
echo ""

VEC_1_18_INPUT="../../test_data/Vectors/vec1_2^18.csv"
VEC_1_19_INPUT="../../test_data/Vectors/vec1_2^19.csv"
VEC_1_20_INPUT="../../test_data/Vectors/vec1_2^20.csv"
VEC_2_18_INPUT="../../test_data/Vectors/vec2_2^18.csv"
VEC_2_19_INPUT="../../test_data/Vectors/vec2_2^19.csv"
VEC_2_20_INPUT="../../test_data/Vectors/vec2_2^20.csv"

module load intel/2022.2
module load OpenMPI/4.1.4-GCC-11.3.0

make

mpirun -n 8 ./dot_product_MPI 262144 $VEC_1_18_INPUT $VEC_2_18_INPUT result_8p_18.csv time_8p_18.csv
mpirun -n 8 ./dot_product_MPI 524288 $VEC_1_19_INPUT $VEC_2_19_INPUT result_8p_19.csv time_8p_19.csv
mpirun -n 8 ./dot_product_MPI 1048576 $VEC_1_20_INPUT $VEC_2_20_INPUT result_8p_20.csv time_8p_20.csv
