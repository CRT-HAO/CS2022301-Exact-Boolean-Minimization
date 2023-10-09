#include "boolean_algebra.hpp"

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<int> BooleanAlgebra::getMinterms(string term) const {
  vector<int> result;
  const size_t sum_num = POW_OF_TWO(this->vars.size());

  for (size_t i = 0; i < sum_num; ++i) {
    bool equal = true;
    // cout << term << "==";

    const size_t vars_num = this->vars.size();
    for (size_t j = 0; j < vars_num; ++j) {
      const char c = term[j];
      // cout << ((i >> (vars_num - j - 1)) & 1);
      if (c == '-' || (ASCII_TO_NUM(c) == ((i >> (vars_num - j - 1)) & 1))) {
        equal &= true;
      } else {
        equal &= false;
      }
    }
    // cout << "=" << equal << endl;
    if (equal)
      result.push_back(i);
  }

  return result;
}

vector<vector<int>> BooleanAlgebra::getAllMinterms() const {
  vector<vector<int>> result;

  for (const auto &term : this->terms) {
    vector<int> r = this->getMinterms(term.first);
    result.push_back(r);
  }

  return result;
}

set<int> BooleanAlgebra::getDontCareMinterms() const {
  set<int> result;

  const size_t sum_num = POW_OF_TWO(this->vars.size());

  for (size_t i = 0; i < sum_num; ++i) {
    for (const auto &term : this->terms) {
      if (term.second != '-')
        continue;

      bool equal = true;
      const size_t vars_num = this->vars.size();

      for (size_t j = 0; j < vars_num; ++j) {
        const char c = term.first[j];
        if (c == '-' || (ASCII_TO_NUM(c) == ((i >> (vars_num - j - 1)) & 1))) {
          equal &= true;
        } else {
          equal &= false;
        }
      }

      if (equal)
        result.insert(i);
    }
  }

  return result;
}