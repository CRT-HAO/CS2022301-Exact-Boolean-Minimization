#include "petrick_method.hpp"
#include "qmc.hpp"

#include <algorithm>
#include <set>
#include <vector>

using namespace std;

vector<vector<QuineMcCluskey::Table>> PetrickMethod::generate() const {
  // skip generate, if the algebra is the simplest one
  // no remaining minterms means the algebra is the simplest one
  if (this->remaining_minterms.size() == 0)
    return {};

  vector<set<set<size_t>>> pos(this->remaining_minterms.size());

  size_t i = 0;
  for (const int &m : this->remaining_minterms) {
    size_t j = 0;
    set<set<size_t>> minterm_vars;
    for (const auto &pi : this->remaining_pi) {
      if (find(pi.minterms.begin(), pi.minterms.end(), m) ==
          pi.minterms.end()) {
        j += 1;
        continue;
      }

      set<size_t> tmp = {j};
      minterm_vars.insert(tmp);

      j += 1;
    }
    pos[i] = minterm_vars;

    i += 1;
  }

  vector<set<set<size_t>>> sop = pos;
  for (size_t i = 0; i < pos.size() - 1; ++i) {
    set<set<size_t>> &left_group = sop[0];
    set<set<size_t>> &right_group = sop[1];

    set<set<size_t>> r;
    for (const set<size_t> &left : left_group) {
      for (const set<size_t> &right : right_group) {
        set<size_t> tmp = left;
        for (const size_t &c : right)
          tmp.insert(c);

        r.insert(tmp);
      }
    }

    sop.erase(sop.begin() + 1);
    sop[0] = r;
  }

  size_t min_nums = SIZE_MAX;
  for (const auto &s : sop[0]) {
    if (s.size() < min_nums)
      min_nums = s.size();
  }

  vector<set<size_t>> mins;
  vector<vector<QuineMcCluskey::Table>> result;
  for (const auto &s : sop[0]) {
    if (s.size() == min_nums) {
      mins.push_back(s);
      vector<QuineMcCluskey::Table> tmp;
      for (const size_t &i : s)
        tmp.push_back(this->remaining_pi[i]);
      result.push_back(tmp);
    }
  }

  return result;
}