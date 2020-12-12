#include <iostream>
#include <string>
#include <vector>

constexpr char directions[] = {'N', 'E', 'S', 'W'};

struct ship {
  int facing_index = 1;
  int x_coord = 0;
  int y_coord = 0;
};

struct waypoint {
  int x_dir = 10;
  int y_dir = 1;
};

void move_ship(ship* ferry, const char direction, const int distance) {
  switch (direction) {
    case 'N':
      ferry->y_coord += distance;
      break;
    case 'E':
      ferry->x_coord += distance;
      break;
    case 'S':
      ferry->y_coord -= distance;
      break;
    case 'W':
      ferry->x_coord -= distance;
      break;
    }
}

void move_waypoint(waypoint* beacon, const char direction, const int distance) {
  switch (direction) {
    case 'N':
      beacon->y_dir += distance;
      break;
    case 'E':
      beacon->x_dir += distance;
      break;
    case 'S':
      beacon->y_dir -= distance;
      break;
    case 'W':
      beacon->x_dir -= distance;
      break;
    }
}

void rotate_right(waypoint* beacon) {
  int tmp = beacon->x_dir;
  beacon->x_dir = beacon->y_dir;
  beacon->y_dir = -tmp;
}

void rotate_left(waypoint* beacon) {
  int tmp = beacon->x_dir;
  beacon->x_dir = -beacon->y_dir;
  beacon->y_dir = tmp;
}

void flip(waypoint* beacon) {
  beacon->x_dir = -beacon->x_dir;
  beacon->y_dir = -beacon->y_dir;
}

int main() {
  std::vector<std::string> instructions;
  std::string line;
  while (std::getline(std::cin, line)) {
    instructions.push_back(line);
  }
  ship ferry;
  for (auto const& instr : instructions) {
    char what_do = instr.at(0);
    switch (what_do) {
      case 'F':
        move_ship(&ferry, directions[ferry.facing_index], std::stoi(instr.substr(1)));
        break;
      case 'R':
        {
          int angle_R = std::stoi(instr.substr(1));
          ferry.facing_index = (ferry.facing_index + angle_R / 90) % 4;
          break;
        }
      case 'L':
        {
          int angle_L = std::stoi(instr.substr(1));
          ferry.facing_index = (ferry.facing_index + 4 - angle_L / 90) % 4;
          break;
        }
      default:
        move_ship(&ferry, what_do, std::stoi(instr.substr(1)));
        break;
    }
  }
  // Part 1
  int x_distance = ferry.x_coord < 0 ? -ferry.x_coord : ferry.x_coord;
  int y_distance = ferry.y_coord < 0 ? -ferry.y_coord : ferry.y_coord;
  std::cout << x_distance + y_distance << '\n';
  // Part 2
  waypoint beacon;
  ship boat;
  for (auto const& instr : instructions) {
    char what_do = instr.at(0);
    switch (what_do) {
      case 'F':
        {
          int multiplier = std::stoi(instr.substr(1));
          boat.x_coord += multiplier * beacon.x_dir;
          boat.y_coord += multiplier * beacon.y_dir;
          break;
        }
      case 'R':
        {
          int angle_R = std::stoi(instr.substr(1)) / 90;
          switch (angle_R) {
            case 1:
              rotate_right(&beacon);
              break;
            case 2:
              flip(&beacon);
              break;
            case 3:
              rotate_left(&beacon);
              break;
          }
          break;
        }
      case 'L':
        {
          int angle_L = std::stoi(instr.substr(1)) / 90;
          switch (angle_L) {
            case 1:
              rotate_left(&beacon);
              break;
            case 2:
              flip(&beacon);
              break;
            case 3:
              rotate_right(&beacon);
              break;
            }
            break;
          }
      default:
        move_waypoint(&beacon, what_do, std::stoi(instr.substr(1)));
        break;
    }
  }

  x_distance = boat.x_coord < 0 ? -boat.x_coord : boat.x_coord;
  y_distance = boat.y_coord < 0 ? -boat.y_coord : boat.y_coord;
  std::cout << x_distance + y_distance << '\n';
  return 0;
}


