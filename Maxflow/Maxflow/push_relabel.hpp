#pragma once
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <queue>
#include <vector>
#include <unordered_map>
#include <climits>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <Maxflow/utility/utility.hpp>
#include <Maxflow/utility/graph.hpp>


namespace std {
  namespace fs = experimental::filesystem;
}

int pr(const std::fs::path& input_path);

int pr_method(int src, int tgt, const Graph& g);

bool push(
  const int node,
  const int src,
  const int tgt,
  const std::vector<int>& h,
  std::vector<std::map<int, int> >& residual,
  std::vector<int>& exec,
  std::queue<int>& exec_vertices,
  std::vector<bool>& active
);

void enque(int v, std::queue<int>& exec_vertices, std::vector<bool>& active, std::vector<int>& exec);

//-----------------------------------------------------------------------------
//Definition of Push_Relabel function
//-----------------------------------------------------------------------------

int pr(const std::fs::path& input_path) {
  int s, t;
  Graph graph;
  read_file(input_path, s, t, graph);
  return pr_method(s, t, graph);
}

int pr_method(int src, int tgt, const Graph& g) {

  int max_n = g.size();

  std::vector<std::map<int, int> > residual(max_n);

  for(const auto& node : g) {
    for(const auto& edge : node) {
      residual[edge.u][edge.v] = edge.w;
      if(residual[edge.v].find(edge.u) == residual[edge.v].end()) {
        residual[edge.v][edge.u] = 0;
      }
    }
  }

  std::vector<int> h(max_n, 0);
  h[src] = max_n;

  //gap
  std::vector<int> h_counter(2 * max_n, 0);
  h_counter[max_n] = 1;
  h_counter[0] = max_n - 1;

  std::vector<int> exec(max_n, 0);
  std::queue<int> exec_vertices;
  std::vector<bool> active(max_n, false);

  for(auto& [to, cap] : residual[src]) {
    if(cap != 0) {
      exec[to] += cap;
      residual[to][src] += cap;
      enque(to, exec_vertices, active, exec);
    }
    cap = 0;
  }
  
  while(!exec_vertices.empty()) {
    int u = exec_vertices.front();
    exec_vertices.pop();
    active[u] = false;
    if(u != tgt) {
      if(!push(u, src, tgt, h, residual, exec, exec_vertices, active)) {
        if(h_counter[h[u]] == 1) {
          int gap_v = h[u];
          for(int i = 0; i < max_n; ++i) {
            if(h[i] < gap_v || i == tgt || i == src) {
              continue;
            }
            --h_counter[h[i]];
            h[i] = std::max(h[i], max_n + 1);
            ++h_counter[h[i]];
            enque(i, exec_vertices, active, exec);
          }
        }
        else{
          --h_counter[h[u]];
          ++h[u];
          ++h_counter[h[u]];
          enque(u, exec_vertices, active, exec);
        }
      }
    }
    std::cerr << exec[tgt] << " ";
  }
  return exec[tgt];
}

bool push(
  const int node,
  const int src,
  const int tgt,
  const std::vector<int>& h,
  std::vector<std::map<int, int> >& residual,
  std::vector<int>& exec,
  std::queue<int>& exec_vertices,
  std::vector<bool>& active
) {
  size_t seen{0};
  bool is_push = false;
  for(auto& [to, cap] : residual[node]) {
    if(h[node] > h[to]) {
      if(cap == 0) {
        ++seen;
        continue;
      }

      int min_flow = std::min(exec[node], cap);

      cap -= min_flow;
      residual[to][node] += min_flow;

      exec[node] -= min_flow;
      exec[to] += min_flow;
      if(to != src && to != tgt) {
        enque(to, exec_vertices, active, exec);
      }
      is_push = true;
    }
  }
  enque(node, exec_vertices, active, exec);
  if(seen == residual[node].size()) {
    is_push = true;
  }
  return is_push;
}

void enque(int v, std::queue<int>& exec_vertices, std::vector<bool>& active, std::vector<int>& exec) {
  if(!active[v] && exec[v] > 0) { 
    active[v] = true;
    exec_vertices.push(v);
  }
}
