#include <bits/stdc++.h>
using namespace std;

class ThreeAddressCode {
    int tempCount;
    vector<string> code;

public:
    ThreeAddressCode() { tempCount = 0; }

    string newTemp() {
        tempCount++;
        return "t" + to_string(tempCount);
    }

    string generate(string expr) {
        expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());
        return genExpr(expr);
    }

    void printCode() {
        for (auto &line : code)
            cout << line << endl;
    }

private:
    string genExpr(string expr) {
        if (expr.empty()) return "";

        bool alnum_only = all_of(expr.begin(), expr.end(), [](char c){ return isalnum(c); });
        if (alnum_only) return expr;

        if (expr.front() == '(' && expr.back() == ')')
            return genExpr(expr.substr(1, expr.size() - 2));

        for (char op : {'+', '-'}) {
            int level = 0;
            for (int i = expr.size() - 1; i >= 0; i--) {
                if (expr[i] == ')') level++;
                else if (expr[i] == '(') level--;
                else if (level == 0 && expr[i] == op) {
                    string left = genExpr(expr.substr(0, i));
                    string right = genExpr(expr.substr(i+1));
                    string temp = newTemp();
                    code.push_back(temp + " = " + left + " " + op + " " + right);
                    return temp;
                }
            }
        }

        for (char op : {'*', '/'}) {
            int level = 0;
            for (int i = expr.size() - 1; i >= 0; i--) {
                if (expr[i] == ')') level++;
                else if (expr[i] == '(') level--;
                else if (level == 0 && expr[i] == op) {
                    string left = genExpr(expr.substr(0, i));
                    string right = genExpr(expr.substr(i+1));
                    string temp = newTemp();
                    code.push_back(temp + " = " + left + " " + op + " " + right);
                    return temp;
                }
            }
        }

        return expr;
    }
};

int main() {
    string expr = "a + b * (c - d) / e";
    ThreeAddressCode tac;
    tac.generate(expr);
    tac.printCode();
    return 0;
}

