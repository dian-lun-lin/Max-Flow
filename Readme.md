# Maxflow

# How to Compile

```bash
~$ mkdir build
~$ cd build
~$ make
```

You will see the executable under `bin/`.

#Usage
./main -i input_pah -g golden_path -m algorithm(FF_bfs FF_dfs PR(push-relabel) BK(Boykov Kolmogorov))

./main -h for more details

#Using script
./each_batch.sh -i input_dir -g golden_dir

This script will run first file in each benchmark. For example, ./run_batch.sh -i ./dataset -g ./dataset will run ./dataset/BVZ-sawtooth0.max , ./dataset/BVZ-tsukuba1.max ...
