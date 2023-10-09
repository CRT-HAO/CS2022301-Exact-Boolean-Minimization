#pragma once

#include <iostream>
#include <set>
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
  std::vector<int> getMinterms(std::string) const;
  std::vector<std::vector<int>> getAllMinterms() const;
  std::set<int> getDontCareMinterms() const;
};