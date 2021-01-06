#include <iostream>
#include <string>

size_t cup_count;

/* cyclic linked list */
struct cup {
  int label;
  cup* next;
};

/* shuffle() takes a pointer to the current cup and returns the pointer to the new current */
cup* shuffle(cup* current) {
//  if (current->label <= 10) {
    std::cout << "Current: " << current->label << '\n';
//  }
  // take out 3 cups
  cup* taken_out[3];
  for (int i = 0; i < 3; ++i) {
    taken_out[i] = current->next;
    std::cout << "taking out " << taken_out[i]->label << '\n';
    current->next = taken_out[i]->next;
  }
  // find destination
  int dest_label = (cup_count + current->label - 1) % cup_count;
  if (dest_label == 0) dest_label = cup_count;
  cup* dest = current->next;
  while (true) {
    std::cout << "looking for destination label " << dest_label << '\n';
    while (dest->label != dest_label && dest != current) {
      dest = dest->next;
    }
    if (dest->label == dest_label) break;
    dest = dest->next;
    dest_label = (cup_count + dest_label - 1) % cup_count;
    if (dest_label == 0) dest_label = cup_count;
  }
  // insert the taken out cups
  taken_out[2]->next = dest->next;
  dest->next = taken_out[0];
  current = current->next;
  return current;
}

int main() {
  std::string input;
  std::getline(std::cin, input);
  //cup_count = input.length();
  cup_count = 100;
  cup* current = new cup();
  current->label = input[0] - '0';
  cup* c = current;
  for (size_t i = 1; i < input.length(); ++i) {
    c->next = new cup();
    c = c->next;
    c->label = input[i] - '0';
    std::cout << c->label << '\n';
  }
  for (int i = 10; i <= cup_count; ++i) {
    c->next = new cup();
    c = c->next;
    c->label = i;
  }
  c->next = current;
  c = c->next;
  // Perform shuffle
  for (int i = 0; i < 100; ++i) {
    current = shuffle(current);
  for (size_t i = 0; i < cup_count - 1; ++i) {
    std::cout << c->label << ' ';
    c = c->next;
  }
  std::cout << '\n';

  }  

  // Find the cup labelled 1
  while (c->label != 1) {
    c = c->next;
  }
  c = c->next;
  // Print output
  for (size_t i = 0; i < cup_count - 1; ++i) {
    std::cout << c->label << ' ';
    c = c->next;
  }
  std::cout << '\n';
}
