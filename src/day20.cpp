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
    std::cout << "parsing tile " << id << '\n';
    std::vector<std::string> tile = {};
    while (std::getline(std::cin, line) && line.length() > 0) {
      tile.push_back(line);
    }
    
    {
      // top edge
      std::string top_edge = tile.front();
      edges_to_ids[top_edge].push_back(id);
      std::cout << "top edge " << top_edge << '\n';
    }

    {
      // bottom edge
      std::string bottom_edge = tile.back();
      std::reverse(bottom_edge.begin(), bottom_edge.end());
      edges_to_ids[bottom_edge].push_back(id);
      std::cout << "bottom edge " << bottom_edge << '\n';
    }

    {
      // left edge
      char edge_arr[10];
      for (int i = 0; i < tile.size(); ++i) {
        edge_arr[i] = tile[i][0];
      }
      std::string left_edge(edge_arr, 10);
      std::reverse(left_edge.begin(), left_edge.end());
      edges_to_ids[left_edge].push_back(id);
      std::cout << "left edge " << left_edge << '\n';
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
      std::cout << "right edge " << right_edge << '\n';
    }
  }
  std::set<int> tiles_with_unmatched_edges;
  unsigned long long mult = 1;
  for (const auto& [edge, tiles] : edges_to_ids) {
    if (tiles.size() == 1) {
      int tile = tiles.front();
      std::cout << "considering edge tile " << tile << '\n';
      if (tiles_with_unmatched_edges.find(tile) != tiles_with_unmatched_edges.end()) {
        mult *= tile;
        std::cout << "corner tile\n";
      } else {
        std::cout << "adding to set\n"; 
        tiles_with_unmatched_edges.insert(tile);
      }
    }
  }
  std::cout << mult << '\n';
  

}
