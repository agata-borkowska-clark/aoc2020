#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int count_adjacent(const size_t x, const size_t y, const char c, const std::vector<std::string>* plan) {
  int count = 0;
  // up
  if (y > 0 && plan->at(y - 1).at(x) == c) {
    ++count;
  }
  // up-right
  if (y > 0 && x + 1 < plan->at(y - 1).length() && plan->at(y - 1).at(x + 1) == c) {
    ++count;
  }
  // right
  if (x + 1 < plan->at(y).length() && plan->at(y).at(x + 1) == c) {
    ++count;
  }
  // right-down
  if (y + 1 < plan->size() && x + 1 < plan->at(y).length() && plan->at(y + 1).at(x + 1) == c) {
    ++count;
  }
  // down
  if (y + 1 < plan->size() && plan->at(y + 1).at(x) == c) {
    ++count;
  }
  // down-left
  if (y + 1 < plan->size() && x > 0 && plan->at(y + 1).at(x - 1) == c) {
    ++count;
  }
  // left
  if (x > 0 && plan->at(y).at(x - 1) == c) {
    ++count;
  }
  // left-up
  if (y > 0 && x > 0 && plan->at(y - 1).at(x - 1) == c) {
    ++count;
  }
  return count;
}

int count_visible(const size_t x, const size_t y, const char c, const std::vector<std::string>* plan) {
  int count = 0;
  // up
  size_t upx = x;
  size_t upy = y;
  while (upy > 0) {
    if (plan->at(upy - 1).at(upx) == c) {
      ++count;
      break;
    } else if (plan->at(upy - 1).at(upx) == '.') {
      --upy;
    } else {
      break;
    }
  }
  // up-right
  size_t uprx = x;
  size_t upry = y;
  while (upry > 0 && uprx + 1 < plan->at(upry - 1).length()) {  
    if (plan->at(upry - 1).at(uprx + 1) == c) {
      ++count;
      break;
    } else if (plan->at(upry - 1).at(uprx + 1) == '.') {
      --upry;
      ++uprx;
    } else {
      break;
    }
  }
  // right
  size_t rx = x;
  size_t ry = y;
  while (rx + 1 < plan->at(ry).length()) {
    if (plan->at(ry).at(rx + 1) == c) {
      ++count;
      break;
    } else if (plan->at(ry).at(rx + 1) == '.') {
      ++rx;
    } else {
      break;
    }
  }
  // right-down
  size_t rdx = x;
  size_t rdy = y;
  while (rdy + 1 < plan->size() && rdx + 1 < plan->at(rdy).length()) {
    if (plan->at(rdy + 1).at(rdx + 1) == c) {
      ++count;
      break;
    } else if (plan->at(rdy + 1).at(rdx + 1) == '.') {
      ++rdy;
      ++rdx;
    } else {
      break;
    }
  }
  // down
  size_t dx = x;
  size_t dy = y;
  while (dy + 1 < plan->size()) {
    if (plan->at(dy + 1).at(dx) == c) {
      ++count;
      break;
    } else if (plan->at(dy + 1).at(dx) == '.') {
      ++dy;
    } else {
      break;
    }
  } 
  // down-left
  size_t dlx = x;
  size_t dly = y;
  while (dly + 1 < plan->size() && dlx > 0) {
    if (plan->at(dly + 1).at(dlx - 1) == c) {
      ++count;
      break;
    } else if (plan->at(dly + 1).at(dlx - 1) == '.') {
      ++dly;
      --dlx;
    } else {
      break;
    }
  }
  // left
  size_t lx = x;
  size_t ly = y;
  while (lx > 0) {
    if (plan->at(ly).at(lx - 1) == c) {
      ++count;
      break;
    } else if (plan->at(ly).at(lx - 1) == '.') {
      --lx;
    } else {
      break;
    }
  }
  // left-up
  size_t lupx = x;
  size_t lupy = y;
  while (lupy > 0 && lupx > 0) {
    if (plan->at(lupy - 1).at(lupx - 1) == c) {
      ++count;
      break;
    } else if (plan->at(lupy - 1).at(lupx - 1) == '.') {
      --lupy;
      --lupx;
    } else {
      break;
    }
  }
  return count;
}

bool step_forward_p1(const std::vector<std::string>* from, std::vector<std::string>* to) {
  bool something_changed = false;
  for (size_t y = 0; y < from->size(); ++y) {
    for (size_t x = 0; x < from->at(y).length(); ++x) {
      char c = from->at(y).at(x);
      if (c == '.') {
        continue;
      } else if (c == 'L') {
        int occupied = count_adjacent(x, y, '#', from);
        if (occupied == 0) {
          to->at(y).at(x) = '#';
          something_changed = true;
        } else {
          to->at(y).at(x) = 'L';
        }
      } else {
        int occupied = count_adjacent(x, y, '#', from);
        if (occupied >= 4) {
          to->at(y).at(x) = 'L';
          something_changed = true;
        } else {
          to->at(y).at(x) = '#';
        }
      }
    }
  }
  return something_changed;
}

bool step_forward_p2(const std::vector<std::string>* from, std::vector<std::string>* to) {
  bool something_changed = false;
  for (size_t y = 0; y < from->size(); ++y) {
    for (size_t x = 0; x < from->at(y).length(); ++x) {
      char c = from->at(y).at(x);
      if (c == '.') {
        continue;
      } else if (c == 'L') {
        int occupied = count_visible(x, y, '#', from);
        if (occupied == 0) {
          to->at(y).at(x) = '#';
          something_changed = true;
        } else {
          to->at(y).at(x) = 'L';
        }
      } else {
        int occupied = count_visible(x, y, '#', from);
        if (occupied >= 5) {
          to->at(y).at(x) = 'L';
          something_changed = true;
        } else {
          to->at(y).at(x) = '#';
        }
      }
    }
  }
  return something_changed;
}


unsigned long count_all(const std::vector<std::string>* plan, const char c) {
  unsigned long count = 0;
  for (auto const& row : *plan) {
    count += std::count(row.begin(), row.end(), c);
  }
  return count;
}

void print_plan(const std::vector<std::string>* plan) {
  for (auto const& row : *plan) {
    std::cout << row << '\n';
  }
  std::cout << '\n';
}

int main() {
  // one map serves as the previous step, while the next step is calculated on the other
  std::vector<std::string> map1;
  std::vector<std::string> map2;
  std::vector<std::string> mapa;
  std::vector<std::string> mapb;
  std::string line;
  while (std::getline(std::cin, line)) {
    map1.push_back(line);
    map2.push_back(line);
    mapa.push_back(line);
    mapb.push_back(line);
  }

  bool not_the_case_that_nothing_changed_p1 = true;
  bool not_the_case_that_nothing_changed_p2 = true;
  while(not_the_case_that_nothing_changed_p1 || not_the_case_that_nothing_changed_p2) {
    if (not_the_case_that_nothing_changed_p1) {
      not_the_case_that_nothing_changed_p1 = step_forward_p1(&map1, &map2);
    } 
    if (not_the_case_that_nothing_changed_p2) {
      not_the_case_that_nothing_changed_p2 = step_forward_p2(&mapa, &mapb);
    }
    if (!not_the_case_that_nothing_changed_p1 && !not_the_case_that_nothing_changed_p2) {
      break;
    }
    if (not_the_case_that_nothing_changed_p1) {
      not_the_case_that_nothing_changed_p1 = step_forward_p1(&map2, &map1);
    }
    if (not_the_case_that_nothing_changed_p2) {
      not_the_case_that_nothing_changed_p2 = step_forward_p2(&mapb, &mapa);    
    }
  }
  std::cout << count_all(&map1, '#') << '\n';
  std::cout << count_all(&mapa, '#') << '\n';
  return 0;
}

