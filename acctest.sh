#!/bin/bash
#
#SBATCH --job-name=sort
#SBATCH --nodes=1
#SBATCH --partition=gpu
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=4
#SBATCH --mem-per-cpu=8G
#SBATCH --output=sort_acc-%j.out 
#SBATCH --time=01:00:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=sjhou@scu.edu #
module load PGI/19.10-GCC-8.3.0-2.32 
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=sort-$current_time.log
file_path=/WAVE/users/unix/sjhou/pr3

for size in 10000 100000 1000000 10000000 100000000 1000000000; do
	./acc $size
done