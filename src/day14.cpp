#include <cassert>
#include <iostream>
#include <bitset>
#include <map>
#include <set>
#include <string>
#include <vector>

std::string apply_bitmask(const long value, std::string& mask) {
  std::string bit_value = std::bitset<36>(value).to_string();
  for (int i = 0; i < 36; ++i) {
    if (mask.at(i) == '1') {
      bit_value.at(i) = '1';
    } else if (mask.at(i) == '0') {
      bit_value.at(i) = '0';
    }
  }
  return bit_value;
}

void apply_bitmask_of_ones(std::string* addr, std::string& mask) {
  for (int i = 0; i < 36; ++i) {
    if (mask.at(i) == '1') {
      addr->at(i) = '1';
    }
  }
}

void get_addrs_to_update(std::string addr, std::string& mask, int start_index, 
                        std::set<std::string>* addr_list) {
  bool floats_present = false;
  for (int i = start_index; i < 36; ++i) {
    if (mask.at(i) == 'X') {
      floats_present = true;
      addr.at(i) = '0';
      get_addrs_to_update(addr, mask, i + 1, addr_list);
      addr.at(i) = '1';
      get_addrs_to_update(addr, mask, i + 1, addr_list);
    }
  }
  if (!floats_present) {
    addr_list->insert(addr);
  }
}

void update_addrs(std::set<std::string>& addr_list, unsigned long value,
                  std::map<unsigned long long, unsigned long long>* mem) {
  for (auto const& addr : addr_list) {
    mem->insert_or_assign(std::bitset<36>(addr).to_ullong(), value);
  }
}

int main() {
  std::vector<std::string> instructions;
  std::string line;
  while (std::getline(std::cin, line)) {
    instructions.push_back(line);
  }
  std::map<unsigned long long, std::string> memory_p1;
  std::map<unsigned long long, unsigned long long> memory_p2;
  std::string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::string what_do;
  std::set<std::string> addr_list;
  for (auto const& instr : instructions) {
    what_do = instr.substr(0, 4);
    if (what_do == "mask") {
      mask = instr.substr(7);
    } else {
      unsigned long mem_addr = std::stoi(instr.substr(4));
      long value = std::stoi(instr.substr(instr.find("=") + 1));
      // Part 1
      std::string bit_value = apply_bitmask(value, mask);
      memory_p1.insert_or_assign(mem_addr, bit_value);
      // Part 2
      std::string addr = std::bitset<36>(mem_addr).to_string();
      apply_bitmask_of_ones(&addr, mask);
      get_addrs_to_update(addr, mask, 0, &addr_list);
      update_addrs(addr_list, value, &memory_p2);
      addr_list.clear();
    }
  }

  // Part 1
  unsigned long long sum = 0;
  for (auto const& [key, value] : memory_p1) {
    sum += std::bitset<36>(value).to_ullong();
  }
  std::cout << sum << '\n';

  // Part 2
  sum = 0;
  for (auto const& [key, value] : memory_p2) {
    sum += value;
  }
  std::cout << sum << '\n';

  return 0;
}

