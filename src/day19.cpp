#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct rule {
  int id;
  std::vector<std::vector<int>> subrules;
  char terminal;
};

std::map<int, rule> cfg;
std::vector<int> terminal_list;

void parse_rule(std::string& line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, {}};
  rule r;
  int id = std::stoi(tokens[0]);
  r.id = id;
  if (tokens[1][0] == '"') {
    r.terminal = tokens[1][1];
    terminal_list.push_back(id);
  } else {
    std::vector<std::vector<int>> subrules;
    size_t i = 1;
    std::vector<int> subrule1;
    while (i < tokens.size() && tokens[i] != "|") {
      subrule1.push_back(std::stoi(tokens[i]));
      ++i;
    }
    subrules.push_back(std::move(subrule1));
    if (i < tokens.size() && tokens[i] == "|") {
      ++i;
      std::vector<int> subrule2;
      while (i < tokens.size()) {
        subrule2.push_back(std::stoi(tokens[i]));
        ++i;
      }
      subrules.push_back(std::move(subrule2));
    }
    r.subrules = std::move(subrules);
  }
  cfg.emplace(id, r);
}

/* Checks if the beginning of the str matches the given rule and updates start to point at the end of match */
bool confirm_match(int rule_id, size_t& start, const std::string& str) {
  if (cfg[rule_id].subrules.size() == 0) {
    if (str[start] == cfg[rule_id].terminal) {
      ++start;
      return true;
    } else { 
      return false;
    }
  } else {
    rule& r = cfg[rule_id];
    for (const auto& v : r.subrules) {
      size_t i = start;
      bool sequence_match = true;
      for (const auto& sr : v) {
        sequence_match &= confirm_match(sr, i, str);
        if (!sequence_match){
          break;
        }
      }
      if (sequence_match) {
        start = i;
        return true;
      }
    }
  }
  return false;
}

int main() {
  std::string line;
  while (std::getline(std::cin, line) && line.length() > 0) {
    parse_rule(line);
  }
  // populate_parents();
  
  std::vector<std::string> input;
  int count = 0;
  while (std::getline(std::cin, line)) {
    input.push_back(line);
  }
  for (const auto& msg : input) {
    size_t i = 0;
    if (confirm_match(0, i, msg) && i == msg.length()) ++count;
  }
  std::cout << count << '\n';

}
