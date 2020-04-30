# Maxflow

# How to Compile

```bash
~$ mkdir build
~$ cd build
~$ make
```

You will see the executable under `bin/`.

#Usage
```bash
./main -i input_pah -g golden_path -m algorithm(FF_bfs FF_dfs PR(push-relabel) BK(Boykov Kolmogorov))
```

```./main -h``` for more details

#Using script
```bash
./each_batch.sh -i input_dir -g golden_dir
```

This script will run first file in each benchmark. For example, ```./each_batch.sh -i ./dataset -g ./dataset``` will run ```./dataset/BVZ-sawtooth0.max , ./dataset/BVZ-tsukuba1.max ...```

#Results(ms)

| Benchmark    | BFS  | DFS | Push-Relabel | Boykov-Kolmogorov |
| :-:          | :-: | :-: | :-: | :-: |
| BVZ-tsukuba  |x|x| 69625  | 1595  |
| BVZ-sawtooth |x|x| 50418  | 3774  |
| BVZ-venus    |x|x| 75612  | 2850  |
| KZ2-tsukuba  |x|x| 133038 | 4865  |
| KZ2-sawtooth |x|x| 219088 | 61076 |
| KZ2-venus    |x|x| 178744 | 32021 |
