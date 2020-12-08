#include <bitset>
#include <iostream>
#include <string>
#include <vector>

void undo(std::vector<size_t>* pointer_backtrace,// std::vector<int>* accumulator_backtrace,
        std::vector<std::string>& instructions, std::vector<bool>* already_done,
        size_t* pointer, int* accumulator, std::vector<bool>* already_undone) {
  bool nothing_changed = true;
  while (nothing_changed) {
    *pointer = pointer_backtrace->back();
    pointer_backtrace->pop_back();
    already_done->at(*pointer) = false;
    std::string instruction = instructions[*pointer].substr(0, 3);
    //std::cout << "undoing " << instructions[*pointer] << " at pointer " << *pointer <<'\n';
    if (instruction == "acc") {
      //*accumulator = accumulator_backtrace->back();
      //accumulator_backtrace->pop_back();
      *accumulator -= std::stoi(instructions[*pointer].substr(3));
      //std::cout << "current accumulator value " << *accumulator << '\n';
    //} else if (instruction == "nop" && std::stoi(instructions[*pointer].substr(3)) == 0) {
      // No-op is a no-op
    //  std::cout << "ignoring " << instructions[*pointer] << '\n';
    } else {
      if (!already_undone->at(*pointer)) {
        nothing_changed = false;
      }
    }
  }
  already_undone->at(*pointer) = true;
  std::cout << "done undoing with pointer at " << *pointer << '\n';
}

void retry(std::vector<size_t>* pointer_backtrace, // std::vector<int>* accumulator_backtrace,
        std::vector<std::string>& instructions, std::vector<bool>* already_done,
        size_t* pointer, int* accumulator) {
  while (*pointer < instructions.size() && !already_done->at(*pointer)) {
    pointer_backtrace->push_back(*pointer);
    std::string instruction = instructions[*pointer];
    //std::cout << "retrying " << instruction << " at pointer " << *pointer << '\n';
    already_done->at(*pointer) = true;
    if (instruction.substr(0, 3) == "acc") {
      try {
        // accumulator_backtrace->push_back(*accumulator);
        *accumulator += std::stoi(instruction.substr(3));
        //std::cout << "current accumulator value " << *accumulator << '\n';
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
  std::cout << "done retrying with pointer " << *pointer << "\n";
}

int main() {
  std::vector<std::string> instructions;
  std::vector<bool> already_done;
  std::vector<bool> already_undone;
  std::string line;
  std::vector<size_t> pointer_backtrace;
  std::vector<int> accumulator_backtrace;
  while (std::getline(std::cin, line)) {
    instructions.push_back(line);
    already_done.push_back(false);
    already_undone.push_back(false);
  }
  int accumulator = 0;
  size_t pointer = 0;
  retry(&pointer_backtrace, /*&accumulator_backtrace,*/ instructions, &already_done, &pointer, &accumulator); 
  std::cout << accumulator << '\n';

  // Part 2
  //pointer = pointer_backtrace.back();
  //pointer_backtrace.pop_back();
  while (pointer < instructions.size()) {
    undo(&pointer_backtrace, /*&accumulator_backtrace,*/ instructions, &already_done, &pointer, &accumulator, &already_undone); 
    std::string instruction = instructions[pointer];
    pointer_backtrace.push_back(pointer);
    if (instruction.substr(0, 3) == "nop") {
      pointer += std::stoi(instruction.substr(3));
      std::cout << "INSTRUCTION SWAP - jumping " << std::stoi(instruction.substr(3)) << '\n';
    } else if (instruction.substr(0, 3) == "jmp") {
      std::cout << "INSTRUCTION SWAP - nop " << std::stoi(instruction.substr(3)) << "\n";
      ++pointer;
    } else {
      std::cout << "trying to change instruction " << instruction << " - this shouldn't happen\n";
    }
    retry(&pointer_backtrace, /*&accumulator_backtrace,*/ instructions, &already_done, &pointer, &accumulator);
  }
  std::cout << accumulator << '\n';
  return 0;
}

