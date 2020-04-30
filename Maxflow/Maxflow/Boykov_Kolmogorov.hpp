#pragma once
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <queue>
#include <vector>
#include <climits>
#include <tuple>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <Maxflow/utility/utility.hpp>
#include <Maxflow/utility/graph.hpp>


namespace std {
  namespace fs = experimental::filesystem;
}

//int bk(const std::fs::path& input_path);

//int bk_method(const int src, const int tgt, const Graph& g);

//std::vector<BKedge*> grow(const int src, std::vector<std::vector<BKnode*> >& neighbor_vec, std::vector<BKnode>& node_vec, std::deque<BKnode*>& A, BKgraph& residual);

//int augment(const int src, const int tgt, std::vector<BKnode>& node_vec, std::vector<BKedge*>& augment_path, BKgraph& residual, std::vector<std::vector<BKnode*> >& neighbor_vec, std::queue<BKnode*>& O);

//void adopt(const int src, const int tgt, const std::vector<BKnode>& node_vec, std::queue<BKnode*>& O, std::deque<BKnode*>& A, BKgraph& residual, std::vector<std::vector<BKnode*> >& neighbor_vec);

//BKedge* find_edge(std::vector<BKedge>& res, BKnode& node);

//bool check_origin(BKnode q, const std::vector<BKnode>& node_vec);

class BK {

  private:

    int _src, _tgt;
    int _max_n;
    int _max_flow{0};
    std::set<int> _active, _orphan;
    std::vector<int> _tree;
    std::vector<int> _parent;
    std::vector<std::map<int, int> > _res;
    
    std::deque<int> _grow();

    void _augment(std::deque<int>& augment_path);

    void _adopt();

    bool _check_origin(int node);
    
  public:

    BK(const std::fs::path& input_path);

    ~BK();
    
    int solve();

};

//-----------------------------------------------------------------------------
//Definition of Boykov_Kolmogorov function
//-----------------------------------------------------------------------------

BK::BK(const std::fs::path& input_path) {
  Graph g;
  read_file(input_path, _src, _tgt, g);
  
  _max_n = g.size();
  _res.resize(_max_n);

  //avoid add edges during solve()
  //for(const auto& edge : g[_src]) {
    //_res[edge.v][_src] = 0;
  //}

  //initialize residual
  for(const auto& node : g) {
    for(const auto& edge : node) {
      _res[edge.u][edge.v] = edge.w;
      if(_res[edge.v].find(edge.u) == _res[edge.v].end()) {
        _res[edge.v][edge.u] = 0;
      }
    }
  }

  //-1 src, tgt
  _tree.resize(_max_n, -1);
  _tree[_src] = _src;
  _tree[_tgt] = _tgt;

  _parent.resize(_max_n, -1);

  _active.emplace(_src);
  _active.emplace(_tgt);
  
}

BK::~BK() {
}

int BK::solve() {

  while(true) {
    auto augment_path = _grow();
    if(augment_path.empty()) {
      return _max_flow;
    }
    _augment(augment_path);
    _adopt();
  }
}

std::deque<int> BK::_grow() {
  std::deque<int> augment_path;

  while(!_active.empty()) {
    int node = *(_active.begin());
    if(_tree[node] == _src) {
      for(auto& [to, cap] : _res[node]) {
        if(cap > 0) {
          if(_tree[to] == -1) {
            _tree[to] = _tree[node];
            _parent[to] = node;
            _active.emplace(to);
          }
          else{
            if(_tree[to] != _tree[node]) {
              //to src
              for(int i = node; i != _tree[node]; i = _parent[i]) {
                augment_path.emplace_front(i);
              }
              augment_path.emplace_front(_src);

              //to tgt
              for(int i = to; i != _tree[to]; i = _parent[i]) {
                augment_path.emplace_back(i);
              }
              augment_path.emplace_back(_tgt);
              return augment_path;
            }
          }
        }
      }
    }
    else if (_tree[node] == _tgt) {
      for(auto& [to, _] : _res[node]) {
        int cap = _res[to][node];
        if(cap > 0) {
          if(_tree[to] == -1) {
            _tree[to] = _tree[node];
            _parent[to] = node;
            _active.emplace(to);
          }
          else{
            if(_tree[to] != _tree[node]) {
              //to src
              for(int i = to; i != _tree[to]; i = _parent[i]) {
                augment_path.emplace_front(i);
              }
              augment_path.emplace_front(_src);
              //to tgt
              for(int i = node; i != _tree[node]; i = _parent[i]) {
                augment_path.emplace_back(i);
              }
              augment_path.emplace_back(_tgt);
              return augment_path;

            }
          }
        }
      }
    }
    else {
      throw std::runtime_error("grow stage wrong");
    }
    _active.erase(node);
  }
  return augment_path;
}

void BK::_augment(std::deque<int>& augment_path) {
  int minf = INT_MAX;
  for(size_t i = 0; i < augment_path.size() - 1; ++i) {
    int p = augment_path[i];
    int q = augment_path[i + 1];
    minf = std::min(minf, _res[p][q]);
  }
  
  for(size_t i = 0; i < augment_path.size() - 1; ++i) {
    int p = augment_path[i];
    int q = augment_path[i + 1];
    _res[p][q] -= minf;
    _res[q][p] += minf;
  }

  for(size_t i = 0; i < augment_path.size() - 1; ++i) {
    int p = augment_path[i];
    int q = augment_path[i + 1];
    if(_res[p][q] == 0) {
      if(_tree[p] == _tree[q]) {
        if(_tree[p] == _src) {
          _parent[q] = -1;
          _orphan.emplace(q);
        }
        else if(_tree[p] == _tgt) {
          _parent[p] = -1;
          _orphan.emplace(p);
        }
        else{
          throw std::runtime_error("augment stage wrong");
        }
      }
    }
  }

  _max_flow += minf;  
}

void BK::_adopt() {
  while(!_orphan.empty()) {
    int node = *(_orphan.begin());
    _orphan.erase(node);
    if(_tree[node] == _src) {
      for(auto& [to, _] : _res[node]) {
        int cap = _res[to][node];
        //find valid parent
        if(_tree[node] == _tree[to] && cap > 0 && _check_origin(to)) {
          _parent[node] = to;
          break;
        }
      }
      if(_parent[node] == -1) {
        for(auto& [to, _] : _res[node]) {
          int cap = _res[to][node];
          if(_tree[node] == _tree[to]) {
            if(cap > 0) {
              _active.emplace(to);
            }
            if(_parent[to] == node) {
              _orphan.emplace(to);
              _parent[to] = -1;
            }
          }
        }
        _tree[node] = -1;
        _active.erase(node);
      }
      
    }
    else if(_tree[node] == _tgt) {
      for(auto& [to, cap] : _res[node]) {
        if(_tree[node] == _tree[to] && cap > 0 && _check_origin(to)) {
          _parent[node] = to;
          break;
        }
      }
      if(_parent[node] == -1) {
        for(auto& [to, cap] : _res[node]) {
          if(_tree[node] == _tree[to]) {
            if(cap > 0) {
              _active.emplace(to);
            }
            if(_parent[to] == node) {
              _orphan.emplace(to);
              _parent[to] = -1;
            }
          }
        }
        _tree[node] = -1;
        _active.erase(node);
      }
    }
    else {
      throw std::runtime_error("augment stage wrong");
    }
  }
}

bool BK::_check_origin(int node) {
  for(int i = node; i != _tree[node]; i = _parent[i]) {
    if(_parent[i] == -1) {
      return false;
    }
  }
  return true;
}

