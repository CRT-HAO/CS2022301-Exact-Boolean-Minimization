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
    std::string literals;
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
  void buildTables(const std::set<int> &);
  groups_t generate();

private:
  static size_t getHighLiteralsNum(const std::string &);
};