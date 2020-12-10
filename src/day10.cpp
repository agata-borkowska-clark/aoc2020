#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

int count_differences(const std::vector<int>& adapters, const int diff) {
  int diff_count = 0;
  if (adapters[0] == diff) {
    ++diff_count;
  }
  for (size_t i = 1; i < adapters.size(); ++i) {
    if (adapters[i] - adapters[i - 1] == diff) {
      ++diff_count;
    }
  }
  return diff_count;
}

long process_adapter(const size_t adapter, 
                     const std::vector<int>& adapters,
                     std::map<size_t, long>* memoized_tails) {
  std::map<size_t, long>::iterator found_memoized = memoized_tails->find(adapter);
  if (found_memoized != memoized_tails->end()) {
    return found_memoized->second;
  } else {
    long paths_sum = 0;
    for (int i = 1; i <=3 && adapter + i < adapters.size(); ++i) {
      if (adapters[adapter + i] - adapters[adapter] <= 3) {
        paths_sum += process_adapter(adapter + i, adapters, memoized_tails);
      } else {
        break;
      }
    } 
    memoized_tails->emplace(adapter, paths_sum);
    return paths_sum;
  }  
}

int main() {
  std::vector<int> adapters(1, 0);
  std::istream_iterator<int> eos;
  adapters.insert(adapters.end(), std::istream_iterator<int>(std::cin), eos);
  std::sort(adapters.begin(), adapters.end());
  int device = adapters.back() + 3;
  adapters.push_back(device);
  int diff1 = count_differences(adapters, 1);
  // If I did it in one pass, it would be less readable, right?
  int diff3 = count_differences(adapters, 3);
  std::cout << diff1 * diff3 << '\n';
  
  // Part 2
  std::map<size_t, long> memoized_tails;
  memoized_tails.emplace(adapters.size() - 1, 1);
  // memoized_tails[i] contains number of ways to reach device from adapters[i]
  std::cout << process_adapter(0, adapters, &memoized_tails) << '\n';
  return 0;
}

