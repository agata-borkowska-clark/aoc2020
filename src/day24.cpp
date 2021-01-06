#include <iostream>
#include <set>
#include <string>
#include <vector>

struct tile {
  int  e = 0;  // +e = -w
  int se = 0;  // +se = -nw
  int ne = 0;  // +ne = -sw

  bool operator==(const tile& rhs) const {
    return this->e == rhs.e && this->se == rhs.se && this->ne == rhs.ne;
  }
  bool operator!=(const tile& rhs) const {
    return this->e != rhs.e || this->se != rhs.se || this->ne != rhs.ne;
  }
  bool operator<(const tile& rhs) const {
    if (this->e == rhs.e) {
      if (this->se == rhs.se) {
        return this->ne < rhs.ne;
      } else {
        return this->se < rhs.se;
      }
    } else {
      return this->e < rhs.e;
    }
  }
};

void print_tile(tile& t) {
  std::cout << "e: " << t.e << ", se: " << t.se << ", ne: " << t.ne << '\n';
}

tile reduce_path(std::string& line) {
  tile t;
  size_t i = 0;
  while (i < line.length()) {
    if (line[i] == 'e') {
      ++t.e;
    } else if (line[i] == 'w') {
      --t.e;
    } else {
      std::string dir = line.substr(i, 2);
      std::cout << dir << '\n';
      // But I want to switch on a string!
      if (line[i] == 's') {
        if (line[i + 1] == 'e') {
          ++t.se;
        } else {
          --t.ne;
        }
      } else {
        if (line[i + 1] == 'e') {
          ++t.ne;
        } else {
          --t.se;
        }
      }
      ++i;
    }
    print_tile(t);
    ++i;
  }
  // cancel out n/s
  if (t.se == t.ne) {
    t.e += t.se;
    t.se = 0;
    t.ne = 0;
  } else if ((t.se > 0 && t.ne > 0) || (t.se < 0 && t.ne < 0)) {
    if (t.se > t.ne) {
      t.se -= t.ne;
      t.e += t.ne;
      t.ne = 0;
    } else {
      t.ne -= t.se;
      t.e += t.se;
      t.se = 0;
    } 
  } else if ((t.e < 0 && t.ne > 0) || (t.e > 0 && t.ne < 0))  {
    if (t.e > t.ne) {
      t.e += t.ne;
      t.se -= t.ne;
      t.ne = 0;
    } else {
      t.ne += t.e;
      t.se += t.e;
      t.e = 0;
    } 
  } else if ((t.e > 0 && t.se < 0) || (t.e < 0 && t.se > 0)) {
    if (t.e > t.se) {
      t.e += t.se;
      t.ne -= t.se;
      t.se = 0;
    } else {
      t.ne += t.e;
      t.se += t.e;
      t.e = 0;
    }
  }

  print_tile(t);
  return t;
}

// std::vector<std::string> input;
std::set<tile> floor;

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    // input.push_back(line);
    tile t = reduce_path(line);
    if (floor.find(t) != floor.end()) {
      floor.erase(t);
    } else {
      floor.insert(t);
    }
  }
  std::cout << floor.size() << '\n';
}
