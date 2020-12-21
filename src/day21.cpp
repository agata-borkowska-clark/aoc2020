#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

auto const word_regex = std::regex("[a-z]+");
std::map<std::string, std::vector<std::vector<std::string>>> allergens_to_ingredients;
std::map<std::string, std::vector<std::string>> allergens_to_ingredients_reduced;
// list of all ingredients, from which the ones matched to allergens will be removed
std::map<std::string, int> all_ingredients; 

int main() {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector<std::string> ingredients;
    std::sregex_iterator i = std::sregex_iterator(line.begin(), line.end(), word_regex);
    std::smatch m = *i;
    // get a list of ingredients
    while (m.str() != "contains") {
      all_ingredients.emplace(m.str(), 0);
      all_ingredients[m.str()] += 1;
      ingredients.push_back(m.str());
      ++i;
      m = *i;
    }
    std::sort(ingredients.begin(), ingredients.end());
    ++i;
    // add it to the allergens<->ingredient lists map
    while (i != std::sregex_iterator()) {
      std::string allergen = (*i).str();
      if (allergens_to_ingredients.find(allergen) != allergens_to_ingredients.end()) {
        allergens_to_ingredients[allergen].push_back(ingredients);
      } else {
        std::vector<std::vector<std::string>> tmp;
        tmp.push_back(ingredients);
        allergens_to_ingredients[allergen] = tmp;
      }
      ++i;
    }
  }
  // find the intersection of all ingredients list for each allergen
  for (const auto& [allergen, ingredients] : allergens_to_ingredients) {
    if (ingredients.size() == 1) {
      allergens_to_ingredients_reduced.emplace(allergen, ingredients[0]);
      continue;
    }
    std::vector<std::string> tmp = ingredients[0];
    for (size_t i = 1; i < ingredients.size(); ++i) {
      std::vector<std::string> result(tmp.size() + ingredients[i].size());
      std::vector<std::string>::iterator it;
      it = std::set_intersection(tmp.begin(), tmp.end(), ingredients[i].begin(), ingredients[i].end(), result.begin());
      result.resize(it - result.begin());
      tmp = result;
    }
    allergens_to_ingredients_reduced.emplace(allergen, tmp);
  }
  // if an allergen has only one possible match, remove it from the lists of possible matches for other allergens
  bool something_changed = true;
  while (something_changed) {
    something_changed = false;
    for (const auto& [allergen, ingredients] : allergens_to_ingredients_reduced) {
      if (ingredients.size() == 1) {
        for (auto& [other_allergen, other_ingredients] : allergens_to_ingredients_reduced) {
          if (allergen != other_allergen) {
            std::string ingredient = ingredients[0];
            auto location = std::find(other_ingredients.begin(), other_ingredients.end(), ingredient);
            if (location != other_ingredients.end()) {
              something_changed = true;
              other_ingredients.erase(location);
            }
          }
        }
      }
    }
  }
  // remove potential matches from the set of all ingredients
  for (const auto& [allergen, ingredients] : allergens_to_ingredients_reduced) {
    for (const auto& ingredient : ingredients) {
      all_ingredients.erase(ingredient);
    }
  }
  int count = 0;
  for (const auto& [ingredient, number] : all_ingredients) {
    count += number;
  }
  std::cout << count << '\n';
  for (const auto& [key, value] : allergens_to_ingredients_reduced) {
    std::cout << value[0] << ',';  
  }
  std::cout << '\n';
}
