#include <iostream>
#include <string>
#include <vector>


int main() {
  int tree_counts[] = {0, 0, 0, 0, 0};
  std::vector<std::string> map;
  std::string line;
  while (std::getline(std::cin, line)) {
    map.push_back(line);
  }
  int x_diffs[] = {1, 3, 5, 7, 1};
  size_t x[] = {0, 0, 0, 0, 0};
  int y = 0;
  size_t map_width = map[y].length();
  while (y < map.size()) {
    for (int i = 0; i < 4; ++i) {
      if (map[y].at(x[i]) == '#') {
        ++tree_counts[i];
      }
      x[i] += x_diffs[i];
      x[i] %= map_width;
    }
   if (y % 2 == 0) {
      if (map[y].at(x[4]) == '#') {
        ++tree_counts[4];
      } 
      x[4] = (x[4] + 1) % map_width;
    }
    ++y;
  }
  // Part 2
  unsigned long tree_product = 1;
  for (int i = 0; i < 5; ++i) {
    tree_product *= tree_counts[i];
  }
  std::cout << tree_counts[1] << '\n';
  std::cout << tree_product << '\n';
  return 0;
}

