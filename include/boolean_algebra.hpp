#pragma once

#include <iostream>
#include <string>
#include <vector>

#define POW_OF_TWO(n) ((n >= 0) ? (1 << (n)) : 0) // 2 ^ i
#define ASCII_TO_NUM(c) ((c) - '0')
#define NUM_TO_ASCII(c) ((c) + '0')

class BooleanAlgebra {
public:
  std::vector<std::string> vars;
  std::vector<std::pair<std::string, char>> terms;

public:
  std::vector<int> getMinterms(std::string term) const {
    std::vector<int> result;
    const size_t sum_num = POW_OF_TWO(this->vars.size());

    for (size_t i = 0; i < sum_num; ++i) {
      bool equal = true;
      // std::cout << term << "==";

      const size_t vars_num = this->vars.size();
      for (size_t j = 0; j < vars_num; ++j) {
        const char c = term[j];
        // std::cout << ((i >> (vars_num - j - 1)) & 1);
        if (c == '-' || (ASCII_TO_NUM(c) == ((i >> (vars_num - j - 1)) & 1))) {
          equal &= true;
        } else {
          equal &= false;
        }
      }
      // std::cout << "=" << equal << std::endl;
      if (equal)
        result.push_back(i);
    }

    return result;
  }

  std::vector<std::vector<int>> getAllMinterms() const {
    std::vector<std::vector<int>> result;

    for (const auto &term : this->terms) {
      std::vector<int> r = this->getMinterms(term.first);
      result.push_back(r);
    }

    return result;
  }
};