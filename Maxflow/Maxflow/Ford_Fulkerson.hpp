#pragma once
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cstring>
#include <Maxflow/utility/utility.hpp>
#include <Maxflow/utility/graph.hpp>
#include <map>

namespace std {
  namespace fs = experimental::filesystem;
}

int ff_bfs(std::fs::path& input_path);

int ff_dfs(std::fs::path& input_path);

int ff_method(const int src, const int tgt, const Graph& g, const bool is_bfs = true);


bool find_path_bfs(
  const int src,
  const int tgt,
  const std::vector<std::map<int, int> >& residual,
  const int MAX_N,
  std::vector<int>& parent
);

bool find_path_dfs(
  const int src,
  const int tgt,
  const std::vector<std::map<int, int> >& residual,
  const int MAX_N,
  std::vector<int>& parent
);

//-----------------------------------------------------------------------------
//Definition of Boykov_Kolmogorov function
//-----------------------------------------------------------------------------

int ff_bfs(std::fs::path& input_path) {
  int s, t;
  Graph graph;
  read_file(input_path, s, t, graph);
  return ff_method(s, t, graph, true);
}

int ff_dfs(std::fs::path& input_path) {
  int s, t;
  Graph graph;
  read_file(input_path, s, t, graph);
  return ff_method(s, t, graph, false);
}

int ff_method(const int src, const int tgt, const Graph& g, const bool is_bfs) {

  int max_n = g.size();
  int MAX_FLOW{0};

  std::vector<std::map<int, int> > residual(max_n);

  for(const auto& node : g) {
    for(const auto& edge : node) {
      residual[edge.u][edge.v] = edge.w;
      if(residual[edge.v].find(edge.u) == residual[edge.v].end()) {
        residual[edge.v][edge.u] = 0;
      }
    }
  }

  std::function<bool(const int, const int, const std::vector<std::map<int, int> >&, const int, std::vector<int>&)> find_path;
  if(is_bfs) {
    find_path = find_path_bfs;
  }
  else{
    find_path = find_path_dfs;
  }

  std::vector<int> p(max_n, -1);

  while(find_path(src, tgt, residual, max_n, p)) {
    int minf = INT_MAX;
    for(int i = tgt; i != src; i = p[i]) {
        minf = std::min(minf, residual[p[i]][i]);
    }

    for(int i = tgt; i != src; i = p[i]) {
      residual[p[i]][i] -= minf;
      residual[i][p[i]] += minf;
    }

    MAX_FLOW += minf;
    std::fill_n(p.begin(), max_n, -1);
    std::cerr << MAX_FLOW << " ";
  }

  return MAX_FLOW;
}

bool find_path_bfs(
  const int src,
  const int tgt,
  const std::vector<std::map<int, int> >& residual,
  const int max_n,
  std::vector<int>& parent
) {
  std::vector<bool> is_checked(max_n, false);
  std::queue<int> que;
  que.push(src);
  is_checked[src] = true;
  while(!que.empty()) {
    int node{que.front()};
    que.pop();
    for(const auto& [to, cap] : residual[node]) {
      if(is_checked[to]) {
        continue;
      }
      if(cap > 0) {
        parent[to] = node;
        que.push(to);
        is_checked[to] = true;
      }
    }
  }
  return is_checked[tgt];
}

bool find_path_dfs(
  const int src,
  const int tgt,
  const std::vector<std::map<int, int> >& residual,
  const int max_n,
  std::vector<int>& parent
) {
  std::vector<bool> is_checked(max_n, false);
  std::stack<int> stk;
  stk.push(src);
  is_checked[src] = true;
  while(!stk.empty()) {
    int node{stk.top()};
    stk.pop();
    for(const auto& [to, cap] : residual[node]) {
      if(is_checked[to]) {
        continue;
      }
      if(cap > 0) {
        parent[to] = node;
        stk.push(to);
        is_checked[to] = true;
      }
    }
  }
  return is_checked[tgt];
}

