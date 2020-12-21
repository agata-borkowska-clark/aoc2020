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
bool confirm_match(int rule_id, size_t& start, const std::string& str, int next_rule = 0) {
  if (rule_id == 8 || rule_id == 11) {
    std::cout << "looping rule " << rule_id << " with " << cfg[rule_id].subrules.size() << " subrules\n";
   // keep applying 42 until it no longer matches
    size_t i = start;
    int count = 0;
    bool match = true;
    do {
      match = confirm_match(42, i, str);
      if (match) ++count;
    } while (i < str.length() && match);
    std::cout << "matched 42 " << count << " times\n";
    if (i >= str.length()) {
      if (rule_id == 8) return match;
      std::cout << "i exceeded str length\n";
      return false;
    }
    if (!match) return false;
    if (rule_id == 8) {
      std::cout << "updating start to " << i << '\n';
      start = i;
    } else {
      // check that we have an appropriate number of 31s
      while (count >= 0 && i < str.length() && match) {
        match = confirm_match(31, i, str);
      }
      if (!match) {
        return false;
      }
      if (count == 0 && match) {
        start = i;
      }
    }
    return match;
  }
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
      for (size_t sr = 0; sr < v.size(); ++sr) {
        if (v[sr] == 8 && sr < v.size() - 1 && cfg[8].subrules.size() == 2) {
          sequence_match &= confirm_match(v[sr], i, str, v[sr + 1]);
        } else {
          sequence_match &= confirm_match(v[sr], i, str);
        }
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
  std::cout << "PART 2\n";
  for (const auto& msg : input) {
    std::cout << "parsing " << msg << '\n';
    size_t i = 0;
    if (confirm_match(0, i, msg) && i == msg.length()) ++count;
  }
  std::cout << count << '\n';
}
