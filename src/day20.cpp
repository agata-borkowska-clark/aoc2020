#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
  std::string line;
  std::map<std::string, std::vector<int>> edges_to_ids = {};
  int id;
  while (std::getline(std::cin, line)) {
    id = std::stoi(line.substr(5));
    std::vector<std::string> tile = {};
    while (std::getline(std::cin, line) && line.length() > 0) {
      tile.push_back(line);
    }
    
    {
      // top edge
      std::string top_edge = tile.front();
      edges_to_ids[top_edge].push_back(id);
      std::reverse(top_edge.begin(), top_edge.end());
      edges_to_ids[top_edge].push_back(id);
    }

    {
      // bottom edge
      std::string bottom_edge = tile.back();
      edges_to_ids[bottom_edge].push_back(id);
      std::reverse(bottom_edge.begin(), bottom_edge.end());
      edges_to_ids[bottom_edge].push_back(id);
    }

    {
      // left edge
      char edge_arr[10];
      for (int i = 0; i < tile.size(); ++i) {
        edge_arr[i] = tile[i][0];
      }
      std::string left_edge(edge_arr, 10);
      edges_to_ids[left_edge].push_back(id);
      std::reverse(left_edge.begin(), left_edge.end());
      edges_to_ids[left_edge].push_back(id);
    }

    {
      // right edge
      char edge_arr[10];
      size_t edge_length = tile[0].length();
      for (int i = 0; i < 10; ++i) {
        edge_arr[i] = tile[i][edge_length - 1];
      }
      std::string right_edge(edge_arr, 10);
      edges_to_ids[right_edge].push_back(id);
      std::reverse(right_edge.begin(), right_edge.end());
      edges_to_ids[right_edge].push_back(id);
    }
  }
  std::map<int, int> tiles_with_unmatched_edges;
  unsigned long long mult = 1;
  for (const auto& [edge, tiles] : edges_to_ids) {
    if (tiles.size() == 1) {
      int tile = tiles.front();
      if (tiles_with_unmatched_edges[tile] == 3) {
        mult *= tile;
      } else {
        tiles_with_unmatched_edges[tile]++;
      }
    }
  }
  std::cout << mult << '\n';
}

