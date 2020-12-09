#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void print_vector(std::vector<bool>* v) {
  for (size_t i = 0; i < v->size(); ++i) {
    std::cout << v->at(i) << ' ';
  }
}

bool test_already_done(std::vector<size_t>* pointer_backtrace, std::vector<bool>* already_done) {
  std::cout << "testing already done ";
  print_vector(already_done);
  std::cout << '\n';
  for (size_t i = 0; i < already_done->size(); ++i) {
    std::vector<size_t>::iterator it;
    it = std::find(pointer_backtrace->begin(), pointer_backtrace->end(), i);
    if (already_done->at(i)) {
      if (it == pointer_backtrace->end()) {
        std::cout << "sanity failed at line " << i << '\n';
        return false;
      }
    } else {
      if (it != pointer_backtrace->end()) {
        std::cout << "sanity failed at line " << i << " already_done says false" << '\n';
        return false;
      }
    }
  }
  return true;
}

void undo(std::vector<size_t>* pointer_backtrace, std::vector<std::string>& instructions,
        std::vector<bool>* already_done, size_t* pointer, int* accumulator, size_t stop) {
  std::string instruction;
  while (*pointer != stop) {
    *pointer = pointer_backtrace->back();
    pointer_backtrace->pop_back();
    already_done->at(*pointer) = false;
    instruction = instructions[*pointer].substr(0, 3);
    if (instruction == "acc") {
      *accumulator -= std::stoi(instructions[*pointer].substr(3));
    } 
  }
}

void retry(std::vector<size_t>* pointer_backtrace, std::vector<std::string>& instructions,
        std::vector<bool>* already_done, size_t* pointer, int* accumulator) {
  while (*pointer < instructions.size() && !already_done->at(*pointer)) {
    pointer_backtrace->push_back(*pointer);
    std::string instruction = instructions[*pointer];
    already_done->at(*pointer) = true;
    if (instruction.substr(0, 3) == "acc") {
      try {
        *accumulator += std::stoi(instruction.substr(3));
        ++*pointer;
      } catch (const std::exception&) {
        std::cout << "failed to parse int in instruction " << instruction << '\n';
      }
    } else if (instruction.substr(0, 3) == "jmp") {
      try {
        *pointer += std::stoi(instruction.substr(3));
      } catch (const std::exception&) {
        std::cout << "failed to parse int in instruction " << instruction << '\n';
      }
    } else {
      ++*pointer;
    }
  }
}


int main() {
  std::vector<std::string> instructions;
  std::vector<bool> already_done;
  std::string line;
  std::vector<size_t> pointer_backtrace;
  std::vector<int> accumulator_backtrace;
  while (std::getline(std::cin, line)) {
    instructions.push_back(line);
    already_done.push_back(false);
  }
  int accumulator = 0;
  size_t pointer = 0;
  retry(&pointer_backtrace, instructions, &already_done, &pointer, &accumulator); 
  std::cout << accumulator << '\n';

  // Part 2
  size_t stop = 0;
  for (size_t i = pointer_backtrace.size() - 1; i >= 0; --i) {
    if (instructions[pointer_backtrace[i]].substr(0, 3) != "acc") {
      stop = pointer_backtrace[i];
      break;
    }
  }
  while (pointer < instructions.size()) {
    undo(&pointer_backtrace, instructions, &already_done, &pointer, &accumulator, stop); 
    std::string instruction = instructions[pointer];
    pointer_backtrace.push_back(pointer);
    already_done[pointer] = true;
    for (size_t i = pointer_backtrace.size() - 2; i >= 0; --i) {
      if (instructions[pointer_backtrace[i]].substr(0, 3) != "acc") {
        if (pointer_backtrace[i] == stop) std::abort();
        stop = pointer_backtrace[i];
        break;
      }
    }
    if (instruction.substr(0, 3) == "nop") {
      pointer += std::stoi(instruction.substr(3));
    } else if (instruction.substr(0, 3) == "jmp") {
      ++pointer;
    } else {
      break;
    }
    retry(&pointer_backtrace, instructions, &already_done, &pointer, &accumulator);
  }
  std::cout << accumulator << '\n';
  return 0;
}

