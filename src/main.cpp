#include "boolean_algebra.hpp"
#include "petrick_method.hpp"
#include "pla.hpp"
#include "qmc.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <utility>

using namespace std;

bool debug = false;

void printUsage(char *executable_name) {
  cout << "Usage: " << executable_name << " <input_pla> <output_pla> [--debug]"
       << endl;
}

int main(int argc, char *argv[]) {
  stringstream ss;

  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  if (argc >= 4) {
    if (strcmp(argv[3], "--debug") == 0) {
      debug = true;
      auto now = std::chrono::system_clock::now();
      std::time_t end_time = std::chrono::system_clock::to_time_t(now);
      ss << "# Exact Boolean Minimization Debug" << endl;
      ss << "> Report generated time " << std::ctime(&end_time) << endl;
    }
  }

  const char *input_file = argv[1];
  const char *output_file = argv[2];

  ifstream ifs;
  ifs.open(input_file);
  if (!ifs.is_open()) {
    cout << "Error: Could not open input file " << input_file << endl;
    return 1;
  }

  ofstream ofs;
  ofs.open(output_file);
  if (!ofs.is_open()) {
    cout << "Error: Could not open output file " << output_file << endl;
    return 1;
  }

  PLA input_pla;
  input_pla.parse(ifs);
  if (!input_pla.end) {
    cout << "Error: Could not parse input file " << input_file << endl;
    return 1;
  }

  BooleanAlgebra algebra;
  algebra.vars = input_pla.input_vars;
  algebra.terms = input_pla.product_terms;

  auto all_minterms = algebra.getAllMinterms();
  auto dont_care = algebra.getDontCareMinterms();

  ss << "## Minterms" << endl;
  for (const auto &minterms : all_minterms) {
    ss << " - ";
    for (const int &i : minterms) {
      ss << "m" << i << ", ";
    }
    ss << endl;
  }

  ss << "## Don't care" << endl;
  for (const auto &d : dont_care) {
    ss << "m" << d << ", ";
  }
  ss << endl;

  set<int> all_minterms_set;
  for (const auto &minterms : all_minterms) {
    for (const auto &minterm : minterms)
      all_minterms_set.insert(minterm);
  }

  QuineMcCluskey qmc;
  qmc.vars_num = algebra.vars.size();
  qmc.buildTables(all_minterms_set);

  ss << "## QMC Table" << endl;
  size_t i = 0;
  for (const auto &g : qmc.groups) {
    ss << "### " << i << endl;

    ss << "| minterms | literals |" << endl;
    ss << "| --- | --- |" << endl;
    for (const auto &t : g) {
      ss << "| ";
      for (const int &m : t.minterms) {
        ss << "m" << m << ", ";
      }
      ss << " | ";

      for (const char &l : t.literals) {
        ss << l;
      }
      ss << " |" << endl;
    }

    i += 1;
  }

  QuineMcCluskey::groups_t pi = qmc.generate();

  ss << "## Prime Implicant" << endl;
  size_t j = 0;
  for (const auto &group : pi) {
    ss << "### Group " << j << endl;

    ss << "| minterms | literals | marked |" << endl;
    ss << "| --- | --- | --- |" << endl;
    for (const auto &table : group) {
      ss << "| ";
      for (const int &m : table.minterms)
        ss << "m" << m << ", ";

      ss << " | ";
      for (const char &c : table.literals)
        ss << c;

      ss << " | " << table.marked << " |" << endl;
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

  ss << "## Essential PI" << endl;
  ss << "| literals |" << endl;
  ss << "| --- |" << endl;
  for (const auto &table : epi) {
    ss << "| ";
    for (const char &c : table.literals) {
      ss << c;
    }
    ss << " |" << endl;
  }

  ss << "## Remaining PI" << endl;
  ss << "| literals |" << endl;
  ss << "| --- |" << endl;
  for (const auto &table : remaining_pi) {
    ss << "| ";
    for (const char &c : table.literals) {
      ss << c;
    }
    ss << " |" << endl;
  }

  ss << "## Remaining Minterms" << endl;
  for (const int &m : remaining_minterms) {
    ss << "m" << m << ", ";
  }
  ss << endl;

  PetrickMethod petrick;
  petrick.remaining_minterms = remaining_minterms;
  petrick.remaining_pi = remaining_pi;
  auto best = petrick.generate();

  ss << "## Best solutions" << endl;
  for (size_t i = 0; i < max(size_t(1), best.size()); ++i) {
    ss << " - ";
    // essential PI
    for (const auto &pi : epi) {
      size_t i = 0;
      for (const char &c : pi.literals) {
        if (c != '-')
          ss << algebra.vars[i] << (c == '0' ? "'" : "");
        i += 1;
      }
      ss << " + ";
    }

    if (i + 1 > best.size()) {
      ss << endl;
      continue;
    }

    // remaining PI
    for (const auto &pi : best[i]) {
      size_t i = 0;
      for (const char &c : pi.literals) {
        if (c != '-')
          ss << algebra.vars[i] << (c == '0' ? "'" : "");
        i += 1;
      }
      ss << " + ";
    }

    ss << endl;
  }

  // create output pla
  PLA output_pla;
  output_pla.input_num = input_pla.input_num;
  output_pla.input_vars = input_pla.input_vars;
  output_pla.output_num = input_pla.output_num;
  output_pla.output_vars = input_pla.output_vars;

  // essential PI
  for (const auto &pi : epi) {
    output_pla.product_terms.push_back(make_pair(pi.literals, '1'));
  }

  // remaining best solutions PI
  if (best.size() > 0) {
    for (const auto &pi : best[0]) {
      output_pla.product_terms.push_back(make_pair(pi.literals, '1'));
    }
  }

  output_pla.product_terms_num = output_pla.product_terms.size();

  output_pla.exports(ofs);

  if (debug)
    cout << ss.str();

  return 0;
}