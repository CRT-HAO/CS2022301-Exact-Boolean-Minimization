#include "pla.hpp"

#include <cstdlib>
#include <sstream>
#include <string>

#define CHAR_TO_NUM(c) ((c) - '0')

using namespace std;

void PLA::parse(istream &in) {
  for (string line; getline(in, line);) {
    stringstream ss(line);

    string command;
    ss >> command;
    if (command == ".i") {
      // eg. `.i 3`
      ss >> this->input_num;
    } else if (command == ".o") {
      // eg. `.o 1`
      ss >> this->output_num;
    } else if (command == ".ilb") {
      // eg. `.ilb a b c`
      this->input_vars.clear();
      for (size_t i = 0; i < this->input_num; ++i) {
        std::string var;
        ss >> var;
        this->input_vars.push_back(var);
      }
    } else if (command == ".ob") {
      // eg. `.ob f`
      this->output_vars.clear();
      for (size_t i = 0; i < this->output_num; ++i) {
        std::string var;
        ss >> var;
        this->output_vars.push_back(var);
      }
    } else if (command == ".p") {
      // eg. `.p 4`
      // we don't support .p command anymore
    } else if (command[0] == '1' || command[0] == '0' || command[0] == '-') {
      // eg.
      // 1-1 1
      // -11 1`
      string product_terms = command;
      char sum;
      ss >> sum;
      this->product_terms.push_back(pair<string, char>(product_terms, sum));
    } else if (command == ".e") {
      // eg. `.e`
      this->end = true;
    } else {
      throw std::runtime_error("Unknown command");
    }
  }
}

void PLA::exports(std::ostream &out) const {
  out << ".i " << this->input_num << endl;
  out << ".o " << this->output_num << endl;

  out << ".ilb ";
  for (const string &s : this->input_vars)
    out << s << " ";
  out << endl;

  out << ".ob ";
  for (const string &s : this->output_vars)
    out << s << " ";
  out << endl;

  out << ".p " << this->product_terms_num << endl;

  for (const auto &p : this->product_terms)
    out << p.first << " " << p.second << endl;

  out << ".e" << endl;
}
