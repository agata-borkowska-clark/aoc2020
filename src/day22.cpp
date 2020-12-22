#include <deque>
#include <iostream>
#include <set>
#include <string>
#include <vector>

enum player {PLAYER1, PLAYER2};

using previous_arrangement = std::pair<std::set<std::deque<unsigned int>>, std::set<std::deque<unsigned int>>>;
std::vector<previous_arrangement> seen_before;

/* Returns true if one of the players already had their current deck */
bool check_seen_previously(const std::deque<unsigned int>& player_1, const std::deque<unsigned int>& player_2) {
  previous_arrangement& pa = seen_before.back();
  if (pa.first.find(player_1) != pa.first.end() 
      && pa.second.find(player_2) != pa.second.end()) {
    return true;
  }
  return false;
}

template <bool part1>
player play_the_game(std::deque<unsigned int>& player_1, std::deque<unsigned int>& player_2) {
  if (player_1.size() == 0) return PLAYER2;
  if (player_2.size() == 0) return PLAYER1;
  if (!part1) {
    if (check_seen_previously(player_1, player_2)) return PLAYER1;
    seen_before.back().first.insert(player_1);
    seen_before.back().second.insert(player_2);
  }
  unsigned int p1 = player_1.front();
  player_1. pop_front();
  unsigned int p2 = player_2.front();
  player_2.pop_front();
  if (!part1) {
    if (p1 <= player_1.size() && p2 <= player_2.size()) {
      seen_before.push_back({});
      std::deque<unsigned int> recursive_p1(player_1.begin(), player_1.begin() + p1);
      std::deque<unsigned int> recursive_p2(player_2.begin(), player_2.begin() + p2);
      player winner = play_the_game<part1>(recursive_p1, recursive_p2);  
      seen_before.pop_back();
      if (winner == PLAYER1) {
        player_1.push_back(p1);
        player_1.push_back(p2);
      } else {
        player_2.push_back(p2);
        player_2.push_back(p1);
      }
      return play_the_game<part1>(player_1, player_2);
    }
  }
  if (p1 > p2) {
    player_1.push_back(p1);
    player_1.push_back(p2);
  } else {
    player_2.push_back(p2);
    player_2.push_back(p1);
  }
    player winner = play_the_game<part1>(player_1, player_2);
    return winner;
}

long calculate_score(const std::deque<unsigned int>& deck) {
  long score = 0;
  size_t size = deck.size();
  for (size_t i = 1; i <= size; i++) {
    score += i * deck[size - i];
  }
  return score;
}

int main() {
  std::deque<unsigned int> player_1, player_2;

  std::string line;
  std::getline(std::cin, line);
  while (std::getline(std::cin, line) && line.length() > 0) {
    player_1.push_back(std::stoi(line));
  }
  std::getline(std::cin, line);
  while (std::getline(std::cin, line)) {
    player_2.push_back(std::stoi(line));
  }
  // copy the input
  std::deque<unsigned int> player_1_copy = player_1;
  std::deque<unsigned int> player_2_copy = player_2;
  player winner = play_the_game<true>(player_1, player_2);
  long score = winner == PLAYER1 ? calculate_score(player_1) : calculate_score(player_2);
  std::cout << score << '\n';

  // Part 2
  std::set<std::deque<unsigned int>> previous_decks_p1;
  std::set<std::deque<unsigned int>> previous_decks_p2;
  previous_arrangement pa(previous_decks_p1, previous_decks_p2);
  seen_before.push_back(pa);
  winner = play_the_game<false>(player_1_copy, player_2_copy);
  score = winner == PLAYER2 ? calculate_score(player_2_copy) : calculate_score(player_1_copy);
  std::cout << score << '\n';
}

