#include <iostream>
#include <iterator>
#include <string>
#include <vector>

bool isValidP2(int p1, int p2, char c, std::string* pwd) {
  if (pwd->at(p1 - 1) == c && pwd->at(p2 - 1) != c) {
    return true;
  }
  if (pwd->at(p2 - 1) == c && pwd->at(p1 - 1) != c) {
    return true;
  }
  return false;
}

int main() {
//  std::vector<int> input{std::istream_iterator<int>(std::cin),{}};
  int valid_pwds1 = 0;
  int valid_pwds2 = 0;
  std::vector<std::string> pwds;
  std::string line;
  while (std::getline(std::cin, line)) {
    pwds.push_back(line);
  }
  for (auto& pwd : pwds) {
    std::string::size_type size;
    int min = std::stoi(pwd, &size);
    pwd = pwd.substr(size + 1);
    int max = std::stoi(pwd, &size);
    pwd = pwd.substr(size + 1);
    char check = pwd.at(0);
    pwd = pwd.substr(3);
    int check_count = 0;
    // Part 1
    for (char& c : pwd) {
      if (c == check) {
        ++check_count;
      }
    }
    if (check_count >= min && check_count <= max) {
      ++valid_pwds1;
    }
    // Part 2
    if (isValidP2(min, max, check, &pwd)) {
      ++valid_pwds2;
    }     
  }
  std::cout << valid_pwds1 << '\n';
  std::cout << valid_pwds2 << '\n';
  return 0;
}

