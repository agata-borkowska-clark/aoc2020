#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum operation {ADD, MULTIPLY, END};

struct op_node {
  long value = 0;
  operation op = END;
  std::unique_ptr<op_node> next;
};

operation find_next_op(size_t& i, size_t& end, const std::string& line) {
  operation op = END;
  size_t j = i;
  for (; j < end; ++j) {
    if (line.at(j) == '+') {
      op = ADD;
      break;
    } else if (line.at(j) == '*') {
      op = MULTIPLY;
      break;
    }
  }
  i = j;
  return op;
}

long evaluate_p1(size_t& start, size_t end, const std::string& line);
long evaluate_p2(size_t& start, size_t end, const std::string& line);

long evaluate_bracket(size_t& i, const std::string& line, bool p1) {
  size_t j = i;
  int bracket_sum = 0;
  do {
    if (line.at(j) == '(') {
      ++bracket_sum;
    } else if (line.at(j) == ')') {
      --bracket_sum;
    }
    ++j;
  } while (bracket_sum != 0);
  ++i;
  --j;
  long val = p1 ? evaluate_p1(i, j, line) : evaluate_p2(i, j, line);
  i = p1 ? j + 1 : j;
  return val;
}

long evaluate_p1(size_t& start, size_t end, const std::string& line) {
  long lhs = 0, rhs = 0;
  char what_do = '+';
  size_t i = start;
  while (i < end) {
    char c = line.at(i);
    if (c == ' ') {
      // Skip over spaces
      ++i;
      continue;
    } else {
      if (c == '(') {
        // Find the closing bracket and recursively evaluate the expression inside
        rhs = evaluate_bracket(i, line, true);
      } else if (c == ')') {
        std::abort();
      } else if (c == '+') {
        what_do = '+';
        ++i;
        continue;
      } else if (c == '*') {
        what_do = '*';
        ++i;
        continue;
      } else {
        rhs = (int) (c - '0');
      }
      if (what_do == '+') {
        lhs += rhs;
      } else {
        lhs *= rhs;
      }
      ++i;
    }
  }
  return lhs;
}

long evaluate_p2(size_t& start, size_t end, const std::string& line) {
  op_node root = {};
  root.op = ADD;
  op_node* last = &root;
  size_t i = start;
  while (i < end) {
    char c = line.at(i);
    if (c == ' ' || c == '+' || c == '*') {
      // Skip over spaces
      ++i;
      continue;
    } else {
      if (c == '(') {
        // Find the closing bracket and recursively evaluate the expression inside
        last->next = std::make_unique<op_node>();
        last = last->next.get();
        last->value = evaluate_bracket(i, line, false);
        last->op = find_next_op(i, end, line);
      } else if (c == ')') {
        // Skip. I can't be bothered figuring out nice pointer increments to not fall into this case
      } else {
        last->next = std::make_unique<op_node>();
        last = last->next.get();
        last->value = (long) (c - '0');
        last->op = find_next_op(i, end, line);  
      }
      i += 2;
    }
  }
  auto first = std::move(root.next);
  while (first->op != END) {
    if (first->op == ADD) {
      first->value += first->next->value;
      first->op = first->next->op;
      first->next = std::move(first->next->next);
    } else {
      while (first->next->op == ADD) {
        first->next->value += first->next->next->value;
        first->next->op = first->next->next->op;
        if (first->next->op == END) break;
        first->next->next = std::move(first->next->next->next);
      }
      first->value *= first->next->value;
      first->op = first->next->op;
      if (first->op == END) break;
      first->next = std::move(first->next->next);
    }
  }
  return first->value;
}

int main() {
  std::string line;
  unsigned long long sum1 = 0, sum2 = 0;
  while (std::getline(std::cin, line)) {
    size_t start1 = 0, start2 = 0;
    sum1 += evaluate_p1(start1, line.length(), line);
    sum2 += evaluate_p2(start2, line.length(), line);
  }
  std::cout << sum1 << '\n' << sum2 << '\n';
}

