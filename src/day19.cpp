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

/* Returns the vector of all the possible ends of the match for a given rule */
std::vector<size_t> apply_rule(int rule_id, size_t& start, const std::string& str) {
  std::vector<size_t> ends = {};
  // reached end of string, can't try and parse further.
  if (start > str.size()) return ends;
  rule& r = cfg[rule_id];
  // this is a terminal rule
  if (r.subrules.size() == 0) {
    if (str[start] == r.terminal) {
      ends.push_back(start + 1);
    }
    return ends;
  } 
    
  std::vector<size_t> tmp_starts = {}, tmp_ends = {}, tmp = {};
  for (const auto& sr : r.subrules) {
    tmp_starts = {};
    tmp_starts.push_back(start);
    for (const auto& subrule_id : sr) {
      for (auto& i : tmp_starts) {
        tmp = apply_rule(subrule_id, i, str);
        tmp_ends.insert(tmp_ends.begin(), tmp.begin(), tmp.end());
      }
      tmp_starts = tmp_ends;
      tmp_ends = {};
      tmp = {};
    }
    ends.insert(ends.end(), tmp_starts.begin(), tmp_starts.end());
  }
  return ends;
}

int main() {
  std::string line;
  while (std::getline(std::cin, line) && line.length() > 0) {
    parse_rule(line);
  }
  
  std::vector<std::string> input;
  int count = 0;
  while (std::getline(std::cin, line)) {
    input.push_back(line);
  }
  for (const auto& msg : input) {
    size_t i = 0;
    std::vector<size_t> rule_matches = apply_rule(0, i, msg);
    if (std::find(rule_matches.begin(), rule_matches.end(), msg.size()) != rule_matches.end()) {
      ++count;
    }
  }
  std::cout << count << '\n';
  std::vector<int> subrule1;
  subrule1.push_back(42);
  subrule1.push_back(8);
  rule& r = cfg[8];
  r.subrules.push_back(std::move(subrule1));

  std::vector<int> subrule2;
  subrule2.push_back(42);
  subrule2.push_back(11);
  subrule2.push_back(31);
  rule& r2 = cfg[11];
  r2.subrules.push_back(std::move(subrule2));
  count = 0;
  for (const auto& msg : input) {
    size_t i = 0;
    std::vector<size_t> rule_matches = apply_rule(0, i, msg);
    if (std::find(rule_matches.begin(), rule_matches.end(), msg.size()) != rule_matches.end()) {
      ++count;
    }
  }
  std::cout << count << '\n';
}
