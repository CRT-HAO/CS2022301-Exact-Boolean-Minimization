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
  std::vector<std::vector<QuineMcCluskey::Table>> generate() const;
};