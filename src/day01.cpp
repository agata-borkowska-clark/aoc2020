#include <iostream>
#include <iterator>
#include <vector>

int main() {
  std::vector<int> input{std::istream_iterator<int>(std::cin),{}};
  int p1 = 0;
  int p2 = 0;
  for (int i = 0; i < input.size() - 1; ++i) {
    for (int j = i + 1; j < input.size(); ++j) {
      int ini = input[i];
      int inj = input[j];
      if (ini + inj == 2020) {
        p1 = ini * inj;
      } else {
        for (int k = j + 1; k < input.size(); ++k) {
          int ink = input[k];
          if (ini + inj + ink == 2020) {
            p2 = ini * inj * ink;
          }
        }
      }
    }
  }
  std::cout << p1 << '\n' << p2 << '\n';
  return 0;
}
