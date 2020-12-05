#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int binsearch(std::string_view moves, int index, int min, int max) {
  if (min == max) return min;  
  char move = moves.at(index);
  if (move == 'F' || move == 'L') {
    if (max - min == 1) return min;
    return binsearch(moves, index + 1, min, min + (max - min) / 2); 
  } else {
    if (max - min == 1) return max;
    return binsearch(moves, index + 1, min + (max - min) / 2 + 1, max);
  } 
} 

int calculate_seat_id(int row, int column) {
  return row * 8 + column;
}

int main() {
  std::string line;
  std::vector<int> seat_list;
  int column, row, current_seat_id;
  while (std::getline(std::cin, line) && line.length() > 0) {
    row = binsearch(line, 0, 0, 127);
    column = binsearch(line, 7, 0, 7);
    current_seat_id = calculate_seat_id(row, column);
    seat_list.push_back(current_seat_id);
  }
  std::sort(seat_list.begin(), seat_list.end());
  std::cout << seat_list[seat_list.size() - 1] << '\n';
  for (size_t i = 1; i < seat_list.size(); ++i) {
    int one_less = seat_list[i] - 1;
    if (seat_list[i - 1] != one_less) {
      std::cout << one_less << '\n';
    }
  }    
  return 0;
}

