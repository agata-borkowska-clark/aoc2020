#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <vector>

constexpr int preamble_size = 25;

void populate_preamble_sets(std::vector<long>& input, std::map<size_t, std::set<long>>* sum_sets) {
  for (int i = 0; i < preamble_size; ++i) {
    std::set<long> sum;
    for (int j = 0; j < preamble_size; ++j) {
      if (j != i) {
        sum.insert(input[i] + input[j]);
      }
    }
    sum_sets->emplace(std::pair<long, std::set<long>>(i, std::move(sum)));
  }
}

bool find_in_preamble(size_t i, std::map<size_t, std::set<long>>* sum_sets, std::vector<long>& input) {
  for (size_t j = i - preamble_size; j < i; ++j) {
    if (sum_sets->at(j).find(input[i]) != sum_sets->at(j).end()) {
      return true;
    }
  }
  return false;
}

void move_preamble_window(size_t i, std::map<size_t, std::set<long>>* sum_sets, std::vector<long>& input) {
  // remove first element and add the last to the middle of the window
  long to_subtract = input[i - preamble_size];
  long to_add = input[i];
  for (size_t j = i - preamble_size + 1; j < i; ++j) {
      sum_sets->at(j).erase(to_subtract + input[j]);
      sum_sets->at(j).insert(to_add + input[j]);
  }
  // populate the set for the last element
  std::set<long> sum;
  for (size_t j = i - preamble_size + 1; j < i; ++j) {
    sum.insert(to_add + input[j]);
  }
  sum_sets->emplace(std::pair<long, std::set<long>>(i, std::move(sum)));
}

long find_weakness(size_t start, size_t end, std::vector<long>& input) {
  long min = LONG_MAX;
  long max = LONG_MIN;
  for (size_t i = start; i <= end; ++i) {
    if (input[i] < min) {
      min = input[i];
    } else if (input[i] > max) {
      max = input[i];
    }
  }
  return min + max;
}

int main() {
  std::vector<long> input{std::istream_iterator<long>(std::cin),{}};
  std::map<size_t, std::set<long>> sum_sets;
  long invalid_number = 0;
  populate_preamble_sets(input, &sum_sets);
  for (size_t i = preamble_size; i < input.size(); ++i) {
    bool found = find_in_preamble(i, &sum_sets, input);
    if (!found) {
      invalid_number = input[i];
      std::cout << invalid_number << '\n';
      break;
    } else {
      move_preamble_window(i, &sum_sets, input);
    }
  }
  size_t start = 0;
  size_t end = 1;
  long summed_window = input[start] + input[end];
  while (end < input.size() && start <= end) {
    if (summed_window < invalid_number) {
      ++end;
      summed_window += input[end];
    } else if (summed_window > invalid_number) {
      summed_window -= input[start];
      ++start;
    } else {
      break;
    }
  }

  std::cout << find_weakness(start, end, input) << '\n';
  return 0;
}
