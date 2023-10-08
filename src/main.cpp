#include "boolean_algebra.hpp"
#include "petrick_method.hpp"
#include "qmc.hpp"

#include <iostream>
#include <map>
#include <set>
#include <utility>

using namespace std;

int main(int argc, char *argv[]) {
  BooleanAlgebra algebra;

  algebra.vars.push_back("a");
  algebra.vars.push_back("b");
  algebra.vars.push_back("c");
  algebra.vars.push_back("d");

  // 00-0 1
  // 0-11 1
  // 1-01 1
  // 0101 1
  // 1111 -
  // 100- 1
  // -01- 1
  algebra.terms.push_back(make_pair("00-0", '1'));
  algebra.terms.push_back(make_pair("0-11", '1'));
  algebra.terms.push_back(make_pair("1-01", '1'));
  algebra.terms.push_back(make_pair("0101", '1'));
  algebra.terms.push_back(make_pair("1111", '-'));
  algebra.terms.push_back(make_pair("100-", '1'));
  algebra.terms.push_back(make_pair("-01-", '1'));

  auto all_minterms = algebra.getAllMinterms();
  auto dont_care = algebra.getDontCareMinterms();

  cout << "= Minterms ===================" << endl;
  for (const auto &minterms : all_minterms) {
    for (const int &i : minterms) {
      cout << "m" << i << ", ";
    }
    cout << endl << "========" << endl;
  }

  cout << "# Don't care" << endl;
  for (const auto &d : dont_care) {
    cout << "m" << d << ", ";
  }
  cout << endl;

  set<int> all_minterms_set;
  for (const auto &minterms : all_minterms) {
    for (const auto &minterm : minterms)
      all_minterms_set.insert(minterm);
  }

  QuineMcCluskey qmc;
  qmc.vars_num = algebra.vars.size();
  qmc.buildTables(all_minterms_set);

  cout << "# Table" << endl;
  size_t i = 0;
  for (const auto &g : qmc.groups) {
    cout << "## " << i << endl;

    cout << "| minterms | literals |" << endl;
    cout << "| --- | --- |" << endl;
    for (const auto &t : g) {
      cout << "| ";
      for (const int &m : t.minterms) {
        cout << "m" << m << ", ";
      }
      cout << " | ";

      for (const char &l : t.literals) {
        cout << l;
      }
      cout << " |" << endl;
    }

    i += 1;
  }

  QuineMcCluskey::groups_t pi = qmc.generate();

  cout << "# Prime Implicant" << endl;
  size_t j = 0;
  for (const auto &group : pi) {
    cout << "## Group " << j << endl;

    cout << "| minterms | literals | marked |" << endl;
    cout << "| --- | --- | --- |" << endl;
    for (const auto &table : group) {
      cout << "| ";
      for (const int &m : table.minterms)
        cout << "m" << m << ", ";

      cout << " | ";
      for (const char &c : table.literals)
        cout << c;

      cout << " | " << table.marked << " |" << endl;
    }

    j += 1;
  }

  // convert groups PI to flat PI array
  vector<QuineMcCluskey::Table> pi_flat;
  vector<QuineMcCluskey::Table> epi;
  vector<QuineMcCluskey::Table> remaining_pi;
  for (const auto &group : pi) {
    for (const auto &table : group)
      pi_flat.push_back(table);
  }

  // essential PI identification
  set<int> remaining_minterms;
  for (const auto &table : pi_flat) {
    for (const int &m : table.minterms) {
      // skip this minterm if it is don't care
      if (dont_care.find(m) != dont_care.end())
        continue;

      remaining_minterms.insert(m);
    }
  }

  for (auto it_a = pi_flat.begin(); it_a != pi_flat.end(); ++it_a) {
    bool remaining = true;
    for (const auto &term : it_a->minterms) {
      // skip this minterm if it is don't care
      if (dont_care.find(term) != dont_care.end())
        continue;

      bool repeated = false;
      for (auto it_b = pi_flat.begin(); it_b != pi_flat.end(); ++it_b) {
        if (it_a == it_b)
          continue;

        if (find(it_b->minterms.begin(), it_b->minterms.end(), term) !=
            it_b->minterms.end()) {
          repeated = true;
          break;
        }
      }

      if (repeated)
        continue;

      epi.push_back(*it_a);
      // remove minterms from remaining minterms
      for (const int &m : it_a->minterms)
        remaining_minterms.erase(m);
      remaining = false;
      break;
    }

    if (remaining)
      remaining_pi.push_back(*it_a);
  }

  cout << "# Essential PI" << endl;
  cout << "| literals |" << endl;
  cout << "| --- |" << endl;
  for (const auto &table : epi) {
    cout << "| ";
    for (const char &c : table.literals) {
      cout << c;
    }
    cout << " |" << endl;
  }

  cout << "## Remaining PI" << endl;
  cout << "| literals |" << endl;
  cout << "| --- |" << endl;
  for (const auto &table : remaining_pi) {
    cout << "| ";
    for (const char &c : table.literals) {
      cout << c;
    }
    cout << " |" << endl;
  }

  cout << "## Remaining Minterms" << endl;
  for (const int &m : remaining_minterms) {
    cout << "m" << m << ", ";
  }
  cout << endl;

  PetrickMethod petrick;
  petrick.remaining_minterms = remaining_minterms;
  petrick.remaining_pi = remaining_pi;
  auto best = petrick.generate();

  cout << "# Best solutions" << endl;
  for (const auto &r : best) {
    cout << " - ";
    // essential PI
    for (const auto &pi : epi) {
      size_t i = 0;
      for (const char &c : pi.literals) {
        if (c != '-')
          cout << algebra.vars[i] << (c == '0' ? "'" : "");
        i += 1;
      }
      cout << " + ";
    }

    // remaining PI
    for (const auto &pi : r) {
      size_t i = 0;
      for (const char &c : pi.literals) {
        if (c != '-')
          cout << algebra.vars[i] << (c == '0' ? "'" : "");
        i += 1;
      }
      cout << " + ";
    }

    cout << endl;
  }

  // b'd' + bd + b'c + ab'
  // b'd' + bd + b'c + ad
  // b'd' + bd + ab' + cd
  // b'd' + bd + cd + ad

  return 0;
}