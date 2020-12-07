#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct node {
  // bool leafroot = false;
  std::string colour;
  std::set<std::string> children = {};
  std::set<std::string> parents = {};
};

std::string find_next_colour(std::vector<std::string>& tokens, size_t begin) {
  assert(begin > 1);
  for (size_t i = begin; i < tokens.size(); ++i) {
    try {
      if (tokens.at(i).substr(0, 3) == "bag")  {
        std::string colour = tokens.at(i - 2) + tokens.at(i - 1);
        return colour;
      }
    } catch (const std::exception&) {
      return "noother";
    }
  }
  return "noother";
}

void add_ancestors(std::map<std::string, node>& graph, std::string start, std::set<std::string>& ancestor_set) {
  node n = graph.at(start);
  if (n.parents.size() == 0) {
    return;
  }
  ancestor_set.insert(n.parents.begin(), n.parents.end());
  for (auto const& parent : n.parents) {
    std::cout << "adding ancestors for " << parent << '\n';
    add_ancestors(graph, parent, ancestor_set);
  }  
  
} 

void populate_parents(std::vector<std::string>& contained_colours,
      std::string& node_colour,
      std::map<std::string, node>& graph) {
  // std::cout << "populating parents of children of " <<node_colour << '\n';
  for (auto const& child : contained_colours) {
    std::cout << child << " as child of " << node_colour << "\n";
    try {
      node c = graph.at(child);
      std::cout << "existing child\n";
      c.parents.insert(node_colour);
      std::cout << "current parents: ";
      for (auto const& c : c.parents) {
        std::cout << c << ' ';
      }
      std::cout << '\n'; 
    } catch (const std::exception&) {
      node c;
      c.colour = child;
      c.parents = {};
      c.parents.insert(node_colour);
      graph.insert(std::pair<std::string, node>(child, c));
    }
  }
}

int main() {
  std::string line;
  std::vector<std::string> tokens;
  std::map<std::string, node> graph;
  while (std::getline(std::cin, line) && line.length() > 0) {
    std::cout << "new line\n";
    std::istringstream iss(line);
    tokens = {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    std::string node_colour = tokens[0] + tokens[1];
    /* get the list of colours that can be in this bag */
    std::vector<std::string> contained_colours = {};
    for (size_t i = 3; i < tokens.size(); ++i) {
      std::string next_colour = find_next_colour(tokens, i);
      if (next_colour == "noother") {
        /* we've reached the end of the list of contained bags (which may be empty) */
        break;
      } else {
        /* we've found a non-trivial colour */
        contained_colours.push_back(next_colour);
        i += 3;
      }
    }

    /* check if node exists - create it if it doesn't, update children if it does */
    try {
      node n = graph.at(node_colour);
      /*std::cout << "found colour " << node_colour << " with children ";
      for (auto const& colour : contained_colours) {
        std::cout << colour << " ";
      }
      std::cout << '\n';*/
      n.children.insert(contained_colours.begin(), contained_colours.end());
      populate_parents(contained_colours, node_colour, graph);
    } catch (const std::exception&) {
      node n;
      n.colour = node_colour;
      std::cout << "new colour " << node_colour << " with children ";
      for (auto const& colour : contained_colours) {
        std::cout << colour << ' ';
      }
      std::cout << '\n';
      n.children = {};
      n.children.insert(contained_colours.begin(), contained_colours.end());
      graph.insert(std::pair<std::string, node>(node_colour, n));
      populate_parents(contained_colours, node_colour, graph);
    }

  }
  /* populate sets of parents */
  /*for (auto const& n : graph) {
    std::string colour = n.first;
    for (auto const& child : n.second.children) {
      node cn = graph.at(child);
      cn.parents.insert(colour);
    }
  }*/
  node shinygold_node = graph.at("shinygold");
  std::cout << "shiny gold has " << shinygold_node.parents.size() << " parents\n";
  std::set<std::string> ancestor_set = {};
      std::cout << "current parents: ";
      for (auto const& c : shinygold_node.parents) {
        std::cout << c << ' ';
      }
      std::cout << '\n'; 
  add_ancestors(graph, "shinygold", ancestor_set);
  std::cout << ancestor_set.size() << '\n';
  return 0;
}

