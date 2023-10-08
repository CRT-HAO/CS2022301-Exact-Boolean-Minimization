#pragma once

#include "boolean_algebra.hpp"

#include <set>
#include <string>
#include <vector>

class QuineMcCluskey {
public:
  class Table {
  public:
    std::vector<int> minterms;
    std::vector<char> literals;
    bool marked;

  public:
    Table() : marked(false) {}
  };

  typedef std::vector<Table> group_t;
  typedef std::vector<group_t> groups_t;

public:
  size_t vars_num;
  groups_t groups;

public:
  QuineMcCluskey() : vars_num(0) {}

public:
  void buildTables(const std::set<int> &minterms) {
    this->groups.clear();
    this->groups.resize(this->vars_num + 1);

    for (const auto &term : minterms) {
      size_t term_high_literals = 0;
      std::vector<char> term_literals_vars(vars_num, '\0');
      for (size_t i = 0; i < this->vars_num; ++i) {
        int t = (term >> (this->vars_num - i - 1)) & 1;

        term_literals_vars[i] = NUM_TO_ASCII(t);

        if (t)
          term_high_literals += 1;
      }

      Table table;
      table.minterms.push_back(term);
      table.literals = term_literals_vars;

      this->groups[term_high_literals].push_back(table);
    }
  }

  groups_t generate() {
    std::vector<groups_t> all_groups(this->vars_num);
    all_groups[0] = this->groups;

    // rounds
    for (size_t i = 0; i < this->vars_num - 1; ++i) {
      all_groups[i + 1].resize(this->vars_num + 1);

      // compare between groups
      std::set<std::vector<char>> exists_literals;
      for (size_t j = 0; j < this->vars_num; ++j) {
        group_t &low_group = all_groups[i][j];
        group_t &high_group = all_groups[i][j + 1];

        for (auto &low_table : low_group) {
          for (auto &high_table : high_group) {
            size_t diff = 0;
            size_t index = 0;
            for (size_t k = 0; k < this->vars_num; ++k) {
              if (low_table.literals[k] != high_table.literals[k]) {
                index = k;
                diff += 1;
              }
            }

            if (diff == 1) {
              Table new_table = low_table;
              new_table.literals[index] = '-';
              new_table.marked = false;

              low_table.marked = true;
              high_table.marked = true;

              if (exists_literals.find(new_table.literals) !=
                  exists_literals.end())
                continue;

              new_table.minterms.insert(new_table.minterms.end(),
                                        high_table.minterms.begin(),
                                        high_table.minterms.end());

              all_groups[i + 1][getHighLiteralsNum(new_table.literals)]
                  .push_back(new_table);
              exists_literals.insert(new_table.literals);
            }
          }
        }
      }
    }

    groups_t result(this->vars_num + 1);

    for (const auto &groups : all_groups) {
      size_t i = 0;
      for (const auto &group : groups) {
        for (const auto &table : group) {
          if (!table.marked)
            result[i].push_back(table);
        }
        i += 1;
      }
    }

    return result;
  }

private:
  static size_t getHighLiteralsNum(const std::vector<char> &literals) {
    size_t num = 0;
    for (const char &c : literals) {
      if (c == '1')
        num += 1;
    }

    return num;
  }
};