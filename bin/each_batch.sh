
#run all mode(FF_bfs, FF_dfs, PR, BK) with first file in each batch

#usage  
#       $1 input_dir
#       $2 golde_dir

mode=("FF_bfs" "FF_dfs") 

for ((i = 0; i < 4; ++i))
do

./main -i $1/BVZ-sawtooth0.max -g $2/BVZ-sawtooth0.sol -m ${mode[i]} >> ${mode[i]}.out
./main -i $1/BVZ-tsukuba0.max -g $2/BVZ-tsukuba0.sol -m ${mode[i]} >> ${mode[i]}.out
./main -i $1/BVZ-venus0.max* -g $1/BVZ-venus0.sol -m ${mode[i]} >> ${mode[i]}.out

./main -i $1/KZ2-sawtooth0.max -g $2/KZ2-sawtooth0.sol -m ${mode[i]} >> ${mode[i]}.out
./main -i $1/KZ2-tsukuba0.max -g $2/KZ2-tsukuba0.sol -m ${mode[i]} >> ${mode[i]}.out
./main -i $1/KZ2-venus0.max* -g $1/KZ2-venus0.sol -m ${mode[i]} >> ${mode[i]}.out

done

