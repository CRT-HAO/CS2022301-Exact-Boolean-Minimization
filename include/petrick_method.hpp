#pragma once

#include "qmc.hpp"

#include <set>
#include <vector>

class PetrickMethod {
public:
  std::vector<QuineMcCluskey::Table> remaining_pi;
  std::set<int> remaining_minterms;

public:
  PetrickMethod() {}

public:
  std::vector<std::vector<QuineMcCluskey::Table>> generate() const {
    std::vector<std::set<std::set<size_t>>> pos(
        this->remaining_minterms.size());

    size_t i = 0;
    for (const int &m : this->remaining_minterms) {
      size_t j = 0;
      std::set<std::set<size_t>> minterm_vars;
      for (const auto &pi : this->remaining_pi) {
        if (find(pi.minterms.begin(), pi.minterms.end(), m) ==
            pi.minterms.end()) {
          j += 1;
          continue;
        }

        std::set<size_t> tmp = {j};
        minterm_vars.insert(tmp);

        j += 1;
      }
      pos[i] = minterm_vars;

      i += 1;
    }
    std::cout << std::endl;

    for (const auto &products : pos) {
      std::cout << "(";
      for (const auto &p : products) {
        for (const size_t &i : p) {
          std::cout << i;
        }
        std::cout << " + ";
      }
      std::cout << ")";
    }
    std::cout << std::endl;

    std::vector<std::set<std::set<size_t>>> sop = pos;
    std::cout << "size=" << pos.size() << std::endl;
    for (size_t i = 0; i < pos.size() - 1; ++i) {
      std::set<std::set<size_t>> &left_group = sop[0];
      std::set<std::set<size_t>> &right_group = sop[1];

      std::cout << "(";
      for (const std::set<size_t> &s : left_group) {
        for (const size_t &c : s)
          std::cout << c << " + ";
      }
      std::cout << ")(";
      for (const std::set<size_t> &s : right_group) {
        for (const size_t &c : s)
          std::cout << c << " + ";
      }
      std::cout << ")" << std::endl;

      std::set<std::set<size_t>> r;
      for (const std::set<size_t> &left : left_group) {
        for (const std::set<size_t> &right : right_group) {
          std::set<size_t> tmp = left;
          for (const size_t &c : right)
            tmp.insert(c);

          r.insert(tmp);
        }
      }

      sop.erase(sop.begin() + 1);
      sop[0] = r;
    }

    for (const std::set<size_t> &s : sop[0]) {
      for (const size_t &c : s)
        std::cout << c;
      std::cout << " + ";
    }
    std::cout << std::endl;

    size_t min_nums = SIZE_MAX;
    for (const auto &s : sop[0]) {
      if (s.size() < min_nums)
        min_nums = s.size();
    }

    std::vector<std::set<size_t>> mins;
    std::vector<std::vector<QuineMcCluskey::Table>> result;
    for (const auto &s : sop[0]) {
      if (s.size() == min_nums) {
        mins.push_back(s);
        std::vector<QuineMcCluskey::Table> tmp;
        for (const size_t &i : s)
          tmp.push_back(this->remaining_pi[i]);
        result.push_back(tmp);
      }
    }

    std::cout << "mins:" << std::endl;
    for (const std::set<size_t> &s : mins) {
      for (const size_t &c : s)
        std::cout << c;
      std::cout << " + ";
    }
    std::cout << std::endl;

    return result;
  }
};