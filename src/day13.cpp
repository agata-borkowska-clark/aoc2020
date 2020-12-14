#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

long mul_inv(long a, long b) {
  long b0 = b, t, q;
  long x0 = 0, x1 = 1;
  if (b == 1) return 1;
  while (a > 1) {
    q = a / b;
    t = b, b = a % b, a = t;
    t = x0, x0 = x1 - q * x0, x1 = t;
  }
  if (x1 < 0) x1 += b0;
  return x1;
}

// Chinese Remainder Theorem
unsigned long evaluate(std::vector<int>* a, std::vector<int>* b) {
  long p, prod = 1, sum = 0;
  size_t len = a->size();
  size_t i = 0;

  for (i = 0; i < len; i++) prod *= b->at(i);
 
  for (i = 0; i < len; i++) {
    p = prod / b->at(i);
    sum += a->at(i) * mul_inv(p, b->at(i)) * p;
  }
 
  return sum % prod;
}

int main() {
  std::string line;
  std::getline(std::cin, line);
  int current_time = std::stoi(line);
  std::getline(std::cin, line);
  std::vector<int> bus_ids;
  std::stringstream ss(line);
  while (ss.good()) {
    std::string id;
    std::getline(ss, id, ',');
    if (id == "x") {
      bus_ids.push_back(0);
    } else {
      bus_ids.push_back(std::stoi(id));
    }
  }
 // int next_departure[bus_ids.size()];
  // Part 1
  int earliest_departure = std::numeric_limits<int>::max();
  int earliest_bus_id = bus_ids[0];
  for (size_t i = 0; i < bus_ids.size(); ++i) {
    if (bus_ids[i] == 0) {
      continue;
    } else {
      int id = bus_ids[i];
      int next_bus = current_time + id - current_time % id;
      if (next_bus < earliest_departure) {
        earliest_departure = next_bus;
        earliest_bus_id = id;
      }
    }
  }
  int p1 = earliest_bus_id * (earliest_departure - current_time);
  std::cout << p1 << '\n';

  // Part 2
  std::vector<int> ids, indices;
  for (size_t i = 0; i < bus_ids.size(); ++i) {
    if (bus_ids[i] != 0) {
      ids.push_back(bus_ids[i]);
      indices.push_back(bus_ids[i] - i);
    }
  }
/*  int largest_bus_id = 0;
  size_t largest_index;
  for (size_t i = 0; i < bus_ids.size(); ++i) {
    if (bus_ids[i] > largest_bus_id) {
      largest_bus_id = bus_ids[i];
      largest_index = i;
    }
  }
  unsigned long start_time = 100000000000000;
//  unsigned long start_time = 0;
  unsigned long counter = 0;
  start_time = start_time - start_time % largest_bus_id - largest_index;
  bool all_matched = false;
  while (!all_matched && counter < 1000000) {
    all_matched = true;
    for (size_t i = 0; i < bus_ids.size(); ++i) {
      if (bus_ids[i] == 0) {
        continue;
      } else {
        if ((start_time + i) % bus_ids[i] != 0) {
          all_matched = false;
          break;
        }
      }
    }
    ++counter;
    start_time += largest_bus_id;
  }*/
  unsigned long start_time = evaluate(&indices, &ids);
  std::cout << start_time << '\n';
  return 0;
}
