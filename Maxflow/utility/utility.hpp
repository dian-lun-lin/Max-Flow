#pragma once
#include <experimental/filesystem>
#include <fstream>
#include <Maxflow/utility/graph.hpp>

namespace std {
  namespace fs = experimental::filesystem;
}

void check_correct(const std::fs::path& golden_path, int score);

void read_file(const std::fs::path& input_path, int& src, int& tgt, Graph& graph);

Edge* find_edge(std::vector<Edge>& from, const int to);


//-----------------------------------------------------------------------------
//Definition of utility function
//-----------------------------------------------------------------------------

void check_correct(const std::fs::path& golden_path, int score) {
  int ans;
  using namespace std::literals::string_literals;
  std::stringstream sstream;
  {
    std::ifstream f{ golden_path };

    if(!f) {
      throw std::runtime_error("cannot open the file"s + golden_path.c_str());
    }
    sstream << f.rdbuf();
  }

  std::string line;
  while(std::getline(sstream, line)) {
    if(line[0] == 's') {
      std::istringstream stream(line);
      std::string skip, s_ans;
      std::getline(stream, skip, ' ');
      std::getline(stream, s_ans, ' ');
      ans = std::stoi(s_ans);
      break;
    }
    line.clear();
  }
  if(ans == score) {
    std::cout << "Correct answer!\n";
  }
  else{
    std::cout << "Wrong answe!r\n";
  }
}

void read_file(const std::fs::path& input_path, int& src, int& tgt, Graph& graph) {

  using namespace std::literals::string_literals;
  std::stringstream sstream;
  {
    std::ifstream f{ input_path };

    if(!f) {
      throw std::runtime_error("cannot open the file"s + input_path.c_str());
    }
    sstream << f.rdbuf();
  }

  std::string line;
  while(std::getline(sstream, line)) {
    if(line[0] == 'p') {
      std::istringstream define_stream(line);
      std::string skip, max_nodes, max_arcs;
      std::getline(define_stream, skip, ' ');
      std::getline(define_stream, skip, ' ');
      std::getline(define_stream, max_nodes, ' ');
      std::getline(define_stream, max_arcs, ' ');
      graph.resize(std::stoi(max_nodes));
    }
    else if(line[0] == 'n') {
      std::istringstream end_point_stream(line);
      std::string skip, node, where;
      std::getline(end_point_stream, skip, ' ');
      std::getline(end_point_stream, node, ' ');
      std::getline(end_point_stream, where, ' ');

      if(where[0] == 's') {
        src = std::stoi(node) - 1;
      }
      else if(where[0] == 't') {
        tgt = std::stoi(node) - 1;
      }
      else{
        throw std::runtime_error("invalid file format");
      }

      graph[src].reserve(graph.size());
    }
    else if(line[0] == 'a') {
      std::istringstream flow_stream(line);
      std::string a, u, v, w;
      std::getline(flow_stream, a, ' ');
      std::getline(flow_stream, u, ' ');
      std::getline(flow_stream, v, ' ');
      std::getline(flow_stream, w, ' ');
      graph[std::stoi(u)- 1].emplace_back(std::stoi(u) - 1, std::stoi(v) - 1, std::stoi(w));
    }

    line.clear();
  }

  std::cout << "Finishing Reading.....\n";
}

Edge* find_edge(std::vector<Edge>& from, const int to) {
  for(auto& edge : from) {
    if(edge.v == to) {
      return &edge;
    }
  }
  return nullptr;
}
