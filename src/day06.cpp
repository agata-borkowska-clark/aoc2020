#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

int add_group(std::array<int, 26>& seen) {
  return std::count_if(seen.begin(), seen.end(), [](int x){return x > 0;});
}

int add_group_if_all(std::array<int, 26>& seen, int group_size) {
  return std::count(seen.begin(), seen.end(), group_size);
}

int main() {
  std::array<int, 26> seen = {};  
  std::string line;
  int group_size = 0;
  int sum = 0;
  int sum_all = 0;
  while (std::getline(std::cin, line)) {
    if (line.length() == 0) {
      sum += add_group(seen);
      sum_all += add_group_if_all(seen, group_size);
      seen = {};
      group_size = 0;
    } else {
      for (char c : line) {
        ++seen[c - 'a'];
      }
      ++group_size;
    }
  }
  sum += add_group(seen);
  sum_all += add_group_if_all(seen, group_size);
  std::cout << sum << '\n';
  std::cout << sum_all << '\n';
  return 0;
}

