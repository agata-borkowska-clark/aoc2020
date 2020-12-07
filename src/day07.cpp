#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

std::map<std::string, int> child_sums = {};

struct node {
  std::string colour;
  std::map<std::string, int> children = {};
  std::set<std::string> parents = {};
};

struct inner_bag {
  std::string colour;
  int quantity;
};

bool operator<(const inner_bag& l, const inner_bag& r) {
  if (l.colour == r.colour) {
    return l.quantity < r.quantity;
  }
  return l.colour < r.colour;
}

inner_bag find_next_colour(std::vector<std::string>& tokens, size_t begin) {
  assert(begin > 2);
  for (size_t i = begin; i < tokens.size(); ++i) {
    if (tokens.at(i).substr(0, 3) == "bag")  {
      std::string colour = tokens.at(i - 2) + tokens.at(i - 1);
      if (colour == "noother") {
        break;
      }
      inner_bag ib;
    try {
      int quantity =  std::stoi(tokens.at(i - 3)); 
      ib.quantity = quantity;
    } catch (const std::exception&) {
      std::cout << tokens.at(i - 2) << '\n';
    }
      ib.colour = colour;
      return ib;
    }
  }
  inner_bag ib;
  ib.colour = "noother";
  ib.quantity = 0;
  return ib;
}

void add_ancestors(std::map<std::string, node>& graph, std::string start, std::set<std::string>& ancestor_set) {
  node& n = graph.at(start);
  if (n.parents.size() == 0) {
    return;
  }
  ancestor_set.insert(n.parents.begin(), n.parents.end());
  for (auto const& parent : n.parents) {
    add_ancestors(graph, parent, ancestor_set);
  }  
  
} 

void populate_parents(std::set<inner_bag>& contained_colours,
      std::string& node_colour,
      std::map<std::string, node>& graph) {
  for (auto const& child : contained_colours) {
    const auto [i, is_new] = graph.try_emplace(child.colour);
    i->second.colour = child.colour;
    i->second.parents.insert(node_colour);
  }
}

int sum_children(std::map<std::string, node>& graph, std::string start, int sum) {
  node& n = graph.at(start);
  for (auto const& child : n.children) {
    int new_sum = child.second;
    int child_sum;
    try {
      child_sum = child_sums.at(child.first);
    } catch (const std::exception&) {
      child_sum = sum_children(graph, child.first, 0);
      child_sums.insert(std::pair<std::string, int>(child.first, child_sum));
    }
    new_sum += child.second * child_sum;
    sum += new_sum;
  }
  return sum;
}

int main() {
  std::string line;
  std::vector<std::string> tokens;
  std::map<std::string, node> graph;
  while (std::getline(std::cin, line) && line.length() > 0) {
    std::istringstream iss(line);
    tokens = {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    std::string node_colour = tokens[0] + tokens[1];
    /* get the list of colours that can be in this bag */
    std::set<inner_bag> contained_colours;
    for (size_t i = 3; i < tokens.size(); ++i) {
      inner_bag next_colour = find_next_colour(tokens, i);
      if (next_colour.colour == "noother") {
        /* we've reached the end of the list of contained bags (which may be empty) */
        break;
      } else {
        /* we've found a non-trivial colour */
        contained_colours.insert(next_colour);
        i += 3;
      }
    }
    const auto [i, is_new] = graph.try_emplace(node_colour);
    for (auto const& inner : contained_colours) {
      const auto [j, jsnew] = i->second.children.try_emplace(inner.colour);
      j->second += inner.quantity;
    }
    i->second.colour = node_colour;
    populate_parents(contained_colours, node_colour, graph);
  }
  std::set<std::string> ancestor_set = {};
  add_ancestors(graph, "shinygold", ancestor_set);
  std::cout << ancestor_set.size() << '\n';
  std::cout << sum_children(graph, "shinygold", 0) << '\n';
  return 0;
}

