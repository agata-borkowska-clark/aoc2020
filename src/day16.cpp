#include <array>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

auto const regex_range = std::regex("[0-9]+-[0-9]+");

void ints_string_to_vector(std::string& str, std::vector<int>* v) {
  std::stringstream ss(str);
  while (ss.good()) {
    std::string val_str;
    std::getline(ss, val_str, ',');
    int val = std::stoi(val_str);
    v->push_back(val);
  }
}

/* Returns the number of sets with 1 element */
unsigned int check_eliminated(const std::vector<std::set<std::string>>& fields_to_names) {
  unsigned int count = 0;
  for (auto const& s : fields_to_names) {
    if (s.size() == 1)  ++count;
  }
  return count;
}

bool check_in_range(int val, const std::vector<std::pair<int, int>>& ranges) {
  for (auto const& range : ranges) {
    if (val >= range.first && val <= range.second) {
      return true;
    }
  }
  return false;
}

void eliminate(std::vector<std::set<std::string>>* fields_to_names,
               const std::map<std::string, std::vector<std::pair<int, int>>>& fields_to_ranges, 
               const std::vector<std::vector<int>>& tickets) {
  unsigned int check = check_eliminated(*fields_to_names);
  while(check != fields_to_names->size()) {
    // if we have a set containing 1 item, then remove that item from all other sets
    if (check > 0) {
      for (size_t i = 0; i < fields_to_names->size(); ++i) {
        if ((*fields_to_names)[i].size() == 1) {
          const std::string& key = *((*fields_to_names)[i].begin());
          for (size_t j = 0; j < fields_to_names->size(); ++j) {
            if (j != i) {
              (*fields_to_names)[j].erase(key);
            }
          }
        }
      }
    }
    // for fields without matched keys, try to eliminate some of the keys
    for (size_t i = 0; i < fields_to_names->size(); ++i) {
      auto& s = (*fields_to_names)[i];
      if (s.size() == 1) {
        continue;
      }
      // for each field in this set, check that the values on all valid tickets are in range
      std::vector<std::string> keys_to_delete;
      for (auto const& key : s) {
        // Ranges for this key
        std::vector<std::pair<int, int>> ranges = fields_to_ranges.at(key);
        // Checking if ith element in each ticket is valid for this key. If not, remove it.
        for (size_t j = 0; j < tickets.size(); ++j) {
          bool all_good = true;
          for (auto const& t : tickets) {
            all_good = check_in_range(t[i], ranges);
            if (!all_good) break;
          }
          if (!all_good) {
            // the value in ith field are invalid for key
            keys_to_delete.push_back(key);
            break;
          }
        }
      }
      for (auto const& key : keys_to_delete) {
        s.erase(key);
      }
    }
    check = check_eliminated(*fields_to_names);
  }
}


int main() {
  std::array<bool, 1000> valid_numbers = {};  // For Part 1, to check if a number is in any range
  std::map<std::string, std::vector<std::pair<int, int>>> fields_to_ranges;
  std::string line;
  while (std::getline(std::cin, line) && line.length() > 0) {
    std::smatch match;
    std::string field = line.substr(0, line.find(':'));
    std::vector<std::pair<int, int>> ranges;
    while (std::regex_search(line, match, regex_range)) {
      std::string match_str = match[0].str();
      int begin = std::stoi(match_str);
      std::string end_str = match_str.substr(match_str.find('-') + 1);
      int end = std::stoi(end_str);
      // Mark numbers in this range as valid for Part 1
      for (int i = begin; i <= end; ++i) {
        valid_numbers[i] = true;
      }
      // Add the range to the vector
      ranges.push_back(std::pair<int, int>(begin, end));
      line = match.suffix();
    }
    // Add the field-ranges pair to the map
    if (field.length() > 0) fields_to_ranges.emplace(field, ranges);
  }

  while (std::getline(std::cin, line) && line != "your ticket:") {
    // skip to your ticket
  }
  std::vector<int> your_ticket;
  std::getline(std::cin, line);
  ints_string_to_vector(line, &your_ticket);

  while (std::getline(std::cin, line) && line != "nearby tickets:") {
    // skip to nearby tickets
  }

  std::vector<std::vector<int>> tickets;
  int sum = 0;
  while (std::getline(std::cin, line)) {
    std::vector<int> ticket;
    ints_string_to_vector(line, &ticket);
    bool invalid = false;
    for (auto val : ticket) {
      if (!valid_numbers[val]) {
        invalid = true;
        sum += val;
      }
    }
    if (!invalid) tickets.push_back(ticket);
  }
  std::cout << sum << '\n';

  fields_to_ranges.erase("");
  // Match values to fields by elimination.
  // First, assign all keys to all fields.
  std::vector<std::set<std::string>> fields_to_names;
  std::set<std::string> keys;
  for (auto const& [key, value] : fields_to_ranges) {
    keys.insert(key);
  }
  for (size_t i = 0; i < your_ticket.size(); ++i) {
    fields_to_names.push_back(keys); 
  }
  eliminate(&fields_to_names, fields_to_ranges, tickets);
  long product = 1;
  for (size_t i = 0; i < your_ticket.size(); ++i) {
    std::string key = *fields_to_names[i].begin();
    if (key.length() >= 9 && key.substr(0, 9) == "departure") {
      product *= your_ticket[i];
    }
  } 
  std::cout << product << '\n';
}

