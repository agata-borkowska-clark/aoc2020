#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::string line;
  std::getline(std::cin, line);
  std::vector<int> starting_numbers;
  std::stringstream ss(line);
  while (ss.good()) {
    std::string number_str;
    std::getline(ss, number_str, ',');
    starting_numbers.push_back(std::stoi(number_str));
  }

  // seen_number[i] is the timestamp when i was last seen
  std::map<long, long long> seen_numbers;
  // populate starting numbers
  assert(starting_numbers.size() > 0);
  for (size_t t = 0; t < starting_numbers.size(); ++t) {
    seen_numbers[starting_numbers[t]] = t;
  }
  // extrapolate up to 2020th number
  long last_number = starting_numbers.back();
  for (size_t t = starting_numbers.size(); t < 30000000; ++t) {
    if (t == 2020) {
      std::cout << last_number << '\n';
    }
    if (seen_numbers.find(last_number) == seen_numbers.end()) {
      // number not seen before
      seen_numbers[last_number] = t - 1;
      last_number = 0;
    } else {
      int number_to_say = t - 1 - seen_numbers[last_number];
      seen_numbers[last_number] = t - 1;
      last_number = number_to_say;
    }
  }
  std::cout << last_number << '\n';
}
