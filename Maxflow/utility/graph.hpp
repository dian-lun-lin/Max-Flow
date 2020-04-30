#pragma once
#include <vector>

struct Edge {
  int u, v, w;
  Edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {};
};

//root = -1 represetn free node
struct BKnode {
  int node;
  int parent;
  int root;
  int dist_to_src;
  BKnode() : node(-1), parent(-1), root(-1), dist_to_src(-1) {};
  BKnode(int n, int p, int r, int d) : node(n), parent(p), root(r), dist_to_src(d) {};
};

struct BKedge {
  BKnode* u;
  BKnode* v;
  int w;
  BKedge(BKnode* _u, BKnode* _v, int _w) : u(_u), v(_v), w(_w) {};
};

using Graph = std::vector<std::vector<Edge> >;
using BKgraph = std::vector<std::vector<BKedge> >;
using AdjMatrix = std::vector<std::vector<int> >;

