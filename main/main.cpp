#include <iostream>
#include <experimental/filesystem>
#include <Maxflow/Maxflow.hpp>
#include <Maxflow/utility/utility.hpp>
#include <chrono>
#include <CLI11/CLI11.hpp>

int main(int argc, char* argv[]) {

  CLI::App app{"Maxflow"};

  std::fs::path input_path;
  app.add_option("-i, --input", input_path, "input path");

  std::fs::path golden_path;
  app.add_option("-g, --golden", golden_path, "golden path");

  std::string method("FF_bfs");
  app.add_option("-m, --method", method, "Algorithm to solve maxflow problem(FF_bfs, FF_dfs, PR(Push-Relabel), BK(Boykov-Kolmogorov)). Default is FF_bfs");

  CLI11_PARSE(app, argc, argv);

  int ans;
  auto beg = std::chrono::steady_clock::now();
  if(method == "FF_bfs") {
    ans = ff_bfs(input_path);
  }
  else if(method == "FF_dfs") {
    ans = ff_dfs(input_path);
  }
  else if(method == "PR") {
    ans = pr(input_path);
  }
  else if(method == "BK") {
    BK bk(input_path);
    ans = bk.solve();
  }
  auto end = std::chrono::steady_clock::now();

  std::cout << "Answer : " << ans << std::endl;
  check_correct(golden_path, ans);

  std::cout << "Mode : " << method << std::endl
            << "Ececution time : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end-beg).count() 
            << " ms"
            << std::endl;

}
