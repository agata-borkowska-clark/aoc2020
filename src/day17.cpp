#include <iostream>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <vector>

struct coordinates {
  int x = 0;
  int y = 0;
  int z = 0;
  int w = 0;
  bool operator==(const coordinates& rhs) const {
    return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
  }
  bool operator!=(const coordinates& rhs) const {
    return this->x != rhs.x || this->y != rhs.y || this->z != rhs.z || this->w != rhs.w;
  }
  bool operator<(const coordinates& rhs) const {
    long mag_sq = this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
    return mag_sq < (rhs.x * rhs.x + rhs.y * rhs.y + rhs.x * rhs.x + rhs.w * rhs.w);
  }
};

struct point {
  coordinates coords;
  bool to_flip = false;
  bool operator==(const point& rhs) const {
    return this->coords == rhs.coords && this->to_flip == rhs.to_flip;
  }
  bool operator!=(const point& rhs) const {
    return this->coords != rhs.coords || this->to_flip != rhs.to_flip;
  }
  bool operator<(const point& rhs) const {
    bool coords_cmp = this->coords == rhs.coords;
    return coords_cmp ? this->to_flip < rhs.to_flip : this->coords < rhs.coords;
  }
};

std::set<std::unique_ptr<point>> grid_3d;
std::set<std::unique_ptr<point>> grid_4d;
int min_x = std::numeric_limits<int>::max() - 1;
int min_y = std::numeric_limits<int>::max() - 1;
int min_z = std::numeric_limits<int>::max() - 1;
int min_w = std::numeric_limits<int>::max() - 1;

int max_x = std::numeric_limits<int>::min() + 1;
int max_y = std::numeric_limits<int>::min() + 1;
int max_z = std::numeric_limits<int>::min() + 1;
int max_w = std::numeric_limits<int>::min() + 1;

void print_coords(const coordinates& c) {
  std::cout << '{' << c.x << ", " << c.y << ", " << c.z << ", " << c.w << '}';
}

std::vector<coordinates> adjacent_cells_3d(const coordinates& coords) {
  std::vector<coordinates> adjacent;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      for (int k = -1; k < 2; ++k) {
        if (i != 0 || j != 0 || k != 0) {
          coordinates c;
          c.x = coords.x + i;
          c.y = coords.y + j;
          c.z = coords.z + k;
          adjacent.push_back(std::move(c));
        }
      }
    }
  }
  return adjacent;
}

std::vector<coordinates> adjacent_cells_4d(const coordinates& coords) {
  std::vector<coordinates> adjacent;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      for (int k = -1; k < 2; ++k) {
        for (int l = -1; l < 2; ++l) {
          if (i != 0 || j != 0 || k != 0 || l != 0) {
            coordinates c;
            c.x = coords.x + i;
            c.y = coords.y + j;
            c.z = coords.z + k;
            c.w = coords.w + l;
            adjacent.push_back(std::move(c));
          }
        }
      }
    }
  }
  return adjacent;
}

int count_neighbours_3d(const coordinates& coords) {
  int neighbours = 0;
  for (const auto& p : grid_3d) {
    const coordinates& candidate = p->coords;
    if (candidate != coords) {
      std::vector<coordinates> adjacent = adjacent_cells_3d(coords);
      for (const auto& c : adjacent) {
        if (candidate == c) {
          ++neighbours;
        }
      }
    }
  }
  return neighbours;
}

int count_neighbours_4d(const coordinates& coords) {
  int neighbours = 0;
  for (const auto& p : grid_4d) {
    const coordinates& candidate = p->coords;
    if (candidate != coords) {
      std::vector<coordinates> adjacent = adjacent_cells_4d(coords);
      for (const auto& c : adjacent) {
        if (candidate == c) {
          ++neighbours;
        }
      }
    }
  }
  return neighbours;
}

void update_bounds(const coordinates& c) {
  if (c.x < min_x) {
    min_x = c.x;
  } else if (c.x > max_x) {
    max_x = c.x;
  }
  if (c.y < min_y) {
    min_y = c.y;
  } else if (c.y > max_y) {
    max_y = c.y;
  }
  if (c.z < min_z) {
    min_z = c.z;
  } else if (c.z > max_z) {
    max_z = c.z;
  }
  if (c.w < min_w) {
    min_w = c.w;
  } else if (c.w > max_w) {
    max_w = c.w;
  }
}

bool find_on_grid_3d(const coordinates& c) {
  for (auto const& p : grid_3d) {
    if (p->coords == c) {
      return true;
    }
  }
  return false;
}

bool find_on_grid_4d(const coordinates& c) {
  for (auto const& p : grid_4d) {
    if (p->coords == c) {
      return true;
    }
  }
  return false;
}

int main() {
  std::string line;
  int y = 0;
  while (std::getline(std::cin, line)) {
    for (size_t x = 0; x < line.length(); ++x) {
      if (line.at(x) == '#') {
        coordinates c;
        c.x = x;
        c.y = y;
        auto p = std::make_unique<point>();
        p->coords = c;
        update_bounds(c);
        grid_3d.insert(std::move(p));
        auto q = std::make_unique<point>();
        q->coords = c;
        grid_4d.insert(std::move(q));
      }
    }
    ++y;
  }
  for (int i = 0; i < 6; ++i) {
    // mark points for deletion
    for (const auto& p : grid_3d) {
      int n = count_neighbours_3d(p->coords);
      if (n < 2 || n > 3) {
        p->to_flip = true;
      }    
    }
    for (const auto& p : grid_4d) {
      int n = count_neighbours_4d(p->coords);
      if (n < 2 || n > 3) {
        p->to_flip = true;
      }    
    }
    // list points to add
    std::vector<coordinates> to_add_3d;
    for (int x = min_x - 1; x <= max_x + 1; ++x) {
      for (int y = min_y - 1; y <= max_y + 1; ++y) {
        for (int z = min_z - 1; z <= max_z + 1; ++z) {
          coordinates c{x, y, z};
          int neighbours = count_neighbours_3d(c);
          if (neighbours == 3 && !find_on_grid_3d(c)) {
            update_bounds(c);
            to_add_3d.push_back(std::move(c));
          }
        }
      }
    }
    std::vector<coordinates> to_add_4d;
    for (int x = min_x - 1; x <= max_x + 1; ++x) {
      for (int y = min_y - 1; y <= max_y + 1; ++y) {
        for (int z = min_z - 1; z <= max_z + 1; ++z) {
          for (int w = min_w - 1; w <= max_w + 1; ++w) {
            coordinates c{x, y, z, w};
            int neighbours = count_neighbours_4d(c);
            if (neighbours == 3 && !find_on_grid_4d(c)) {
              update_bounds(c);
              to_add_4d.push_back(std::move(c));
            }
          }
        }
      }
    }
    // remove points marked for deletion
    auto it = grid_3d.begin();
    while (it != grid_3d.end()) {
      if ((*it)->to_flip) {
        it = grid_3d.erase(it);
      } else {
        ++it;
      }
    }
    it = grid_4d.begin();
    while (it != grid_4d.end()) {
      if ((*it)->to_flip) {
        it = grid_4d.erase(it);
      } else {
        ++it;
      }
    }
    // add the new points
    for (auto const& c : to_add_3d) {
      auto p = std::make_unique<point>();
      p->coords = c;    
      grid_3d.insert(std::move(p));
    }
    for (auto const& c : to_add_4d) {
      auto p = std::make_unique<point>();
      p->coords = c;    
      grid_4d.insert(std::move(p));
    }
  }
  std::cout << grid_3d.size() << '\n';
  std::cout << grid_4d.size() << '\n';
}
