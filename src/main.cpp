#include "boolean_algebra.hpp"
#include "qmc.hpp"

#include <iostream>
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

  cout << "= Minterms ===================" << endl;
  for (const auto &minterms : all_minterms) {
    for (const int &i : minterms) {
      cout << "m" << i << ", ";
    }
    cout << endl << "========" << endl;
  }

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

  return 0;
}