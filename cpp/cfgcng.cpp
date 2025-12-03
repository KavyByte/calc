// Cfg to Cnf
#include <bits/stdc++.h>
using namespace std;

struct Rule {
  string left;
  vector<string> right;
};

class CFG {
  vector<Rule> rules;
  set<string> variables, terminals;

public:
  void addRule(string left, vector<string> right) {
    rules.push_back({left, right});
    variables.insert(left);
    for (auto &sym : right) {
      if (islower(sym[0]))
        terminals.insert(sym);
      else
        variables.insert(sym);
    }
  }

  void termToVar() {
    map<string, string> termMap;
    int count = 1;
    vector<Rule> newRules;

    for (auto &r : rules) {
      vector<string> newRight;
      for (auto &sym : r.right) {
        if (islower(sym[0]) && r.right.size() > 1) {
          if (termMap.find(sym) == termMap.end()) {
            string var = "T" + to_string(count++);
            termMap[sym] = var;
            newRules.push_back({var, {sym}});
          }
          newRight.push_back(termMap[sym]);
        } else {
          newRight.push_back(sym);
        }
      }
      newRules.push_back({r.left, newRight});
    }
    rules = newRules;
  }

  void binarize() {
    vector<Rule> newRules;
    int count = 1;

    for (auto &r : rules) {
      if (r.right.size() <= 2) {
        newRules.push_back(r);
      } else {
        string prev = r.right[0];
        for (int i = 1; i < (int)r.right.size() - 1; i++) {
          string var = "X" + to_string(count++);
          newRules.push_back({var, {prev, r.right[i]}});
          prev = var;
        }
        newRules.push_back({r.left, {prev, r.right.back()}});
      }
    }
    rules = newRules;
  }

  void toCNF() {
    termToVar();
    binarize();
  }

  void print() {
    for (auto &r : rules) {
      cout << r.left << " -> ";
      for (int i = 0; i < r.right.size(); i++) {
        cout << r.right[i] << (i + 1 < r.right.size() ? " " : "");
      }
      cout << endl;
    }
  }
};

int main() {
  CFG cfg;

  cfg.addRule("S", {"A", "B", "C"});
  cfg.addRule("A", {"a"});
  cfg.addRule("B", {"b"});
  cfg.addRule("C", {"c", "d"});

  cout << "Input CFG:\n";
  cfg.print();
  cfg.toCNF();
  cout << "\nCNF Grammar:\n";
  cfg.print();

  return 0;
}
