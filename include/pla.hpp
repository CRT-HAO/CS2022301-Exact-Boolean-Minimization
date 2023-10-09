#pragma once

#include <istream>
#include <string>
#include <vector>

class PLA {
public:
  int input_num{0};
  int output_num{0};
  std::vector<std::string> input_vars;
  std::vector<std::string> output_vars;
  int product_terms_num{0};
  std::vector<std::pair<std::string, char>> product_terms;
  bool end{false};

public:
  PLA() {}

public:
  void parse(std::istream &);
  void exports(std::ostream &) const;
};