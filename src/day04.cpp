#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

auto const regex_hair = std::regex("#[0-9a-f]{6}");
auto const regex_pid = std::regex("[0-9]{9}");
std::string colours[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

bool compare_int_bounds(std::string& value, int min, int max) {
  try {
    int year = std::stoi(value);
    return (year >= min && year <= max);
  } catch (const std::exception& e) {
    return false;
  }
} 

bool validate(int field, std::string& value) {
  switch (field) {
    case 0:
      return compare_int_bounds(value, 1920, 2002);
    case 1:
      return compare_int_bounds(value, 2010, 2020);
    case 2:
      return compare_int_bounds(value, 2020, 2030);
    case 3:
      if (value.length() < 3) {
        return false;
      } 
      {
        std::string unit = value.substr(value.length() - 2);
        if (unit == "cm") {
          return compare_int_bounds(value, 150, 193);
        } else if (unit == "in") {
          return compare_int_bounds(value, 59, 76);
        }
        return false;
      }
    case 4:
      try {
        return std::regex_match(value, regex_hair);
      } catch (const std::exception& e) {
        return false;
      }
    case 5:
      if (value.length() != 3) {
        return false;
      } 
      for (int i = 0; i < 7; ++i) {
        if (value == colours[i]) {
          return true;
        }
      }
      return false;
    case 6:
      try {
        return std::regex_match(value, regex_pid);
      } catch (const std::exception& e) {
        return false;
      }
    default: 
      return false;
  }
  return false;
}

bool check_all_there(std::array<bool,8>* seen) {
  bool all_there = true;
  for (int i = 0; i < 7; ++i) {
    all_there &= seen->at(i);
  }
  return all_there;
}

int main() {
  std::string keys[] = {"byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:", "cid:"};
  std::array<bool, 8> seen = {};
  std::array<bool, 8> valid = {};
  int field_count = 8;
  int passport_count = 0;
  int valid_count = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.length() == 0) {
      if (check_all_there(&seen)) {
        ++passport_count;
      }
      seen = {};
      if (check_all_there(&valid)) {
        ++valid_count;
      } 
      valid = {};
    } else {
      for (int i = 0; i < field_count - 1; ++i) {
        if (!seen[i]) {  // don't look for fields we've already found
          auto key_pos = line.find(keys[i]);
          if (key_pos != std::string::npos) {
            seen[i] = true;
            int substr_begin = key_pos + 4;
            int substr_len = line.find(' ', substr_begin) - substr_begin;
            std::string value = line.substr(substr_begin, substr_len);
            valid[i] = validate(i, value);
          }
        }
      }
    }
  }
  if (check_all_there(&seen)) {
     ++passport_count;
  }
  if (check_all_there(&valid)) {
    ++valid_count;
  }
  std::cout << passport_count << '\n';
  std::cout << valid_count << '\n';
  return 0;
}

