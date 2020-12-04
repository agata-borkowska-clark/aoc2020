#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool validate(int field, std::string* value) {
  std::string colours[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  switch (field) {
    case 0:
      try {
        int year = std::stoi(*value);
        return (year >= 1920 && year <= 2002);
      } catch (const std::exception& e) {
        return false;
      }
    case 1:
      try {
        int year = std::stoi(*value);
        return (year >= 2010 && year <= 2020);
      } catch (const std::exception& e) {
        return false;
      }
    case 2:
      try {
        int year = std::stoi(*value);
        return (year >= 2020 && year <= 2030);
      } catch (const std::exception& e) {
        return false;
      }
    case 3:
      try {
        int hgt = std::stoi(*value);
        std::string unit = value->substr(value->length() - 2);
        if (unit.compare("cm") == 0) {
          return (hgt >= 150 && hgt <= 193);
        } else if (unit.compare("in") == 0) {
          return (hgt >= 59 && hgt <= 76);
        }
        return false;
      } catch (const std::exception& e) {
        return false;
      }
    case 4:
      try {
        auto const regex = std::regex("^#[0-9a-f]{6}$");
        return std::regex_search(*value, regex) == 1;
      } catch (const std::exception& e) {
        return false;
      }
    case 5:
      if (value->length() != 3) {
        return false;
      } 
      for (int i = 0; i < 7; ++i) {
        if (value->compare(colours[i]) == 0) {
          return true;
        }
      }
      return false;
    case 6:
      try {
        auto const regex = std::regex("^[0-9]{9}$");
        return std::regex_search(*value, regex) == 1;
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

void reset(std::array<bool, 8>* arr) {
  for (int i = 0; i < 7; ++i) {
    arr->at(i) = false;
  }
}

int main() {
  std::string keys[] = {"byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:", "cid:"};
  std::array<bool, 8> seen = {false, false, false, false, false, false, false, false};
  std::array<bool, 8> valid = {false, false, false, false, false, false, false, false};
  int field_count = 8;
  int passport_count = 0;
  int valid_count = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.length() == 0) {
      if (check_all_there(&seen)) {
        ++passport_count;
      }
      reset(&seen);
      if (check_all_there(&valid)) {
        ++valid_count;
      } 
      reset(&valid);
    } else {
      for (int i = 0; i < field_count - 1; ++i) {
//        if (!seen[i]) {  // don't look for fields we've already found
          int key_pos = line.find(keys[i]);
          if (key_pos != std::string::npos) {
            seen[i] = true;
            int substr_begin = key_pos + 4;
            int substr_len = line.find(' ', substr_begin) - substr_begin;
            std::string value = line.substr(substr_begin, substr_len);
            valid[i] = validate(i, &value);
          }
//        }
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

