//Recursive decent
#include <bits/stdc++.h>
using namespace std;

class Parser {
    vector<string> tokens;
    int pos;

public:
    Parser(const vector<string>& t) : tokens(t), pos(0) {}

    string peek() {
        if (pos < (int)tokens.size()) return tokens[pos];
        return "";
    }

    string get() {
        if (pos < (int)tokens.size()) return tokens[pos++];
        return "";
    }

    // E -> T E'
    int E() {
        int val = T();
        val = Eprime(val);
        return val;
    }

    // E' -> + T E' | ε
    int Eprime(int inherited) {
        while (peek() == "+") {
            get(); // consume '+'
            int tval = T();
            inherited += tval;
        }
        return inherited;
    }

    // T -> F T'
    int T() {
        int val = F();
        val = Tprime(val);
        return val;
    }

    // T' -> * F T' | ε
    int Tprime(int inherited) {
        while (peek() == "*") {
            get(); // consume '*'
            int fval = F();
            inherited *= fval;
        }
        return inherited;
    }

    // F -> (E) | id
    int F() {
        string cur = peek();
        if (cur == "(") {
            get(); // consume '('
            int val = E();
            if (peek() == ")") get(); // consume ')'
            else throw runtime_error("Missing closing parenthesis");
            return val;
        } else if (cur == "id") {
            get(); // consume 'id'
            return 5; // every id = 5
        }
        throw runtime_error("Unexpected token: " + cur);
    }
};

int main() {
    string input;
    cout << "Enter expression (space separated tokens):\n";
    getline(cin, input);

    stringstream ss(input);
    vector<string> tokens;
    string tok;
    while (ss >> tok) tokens.push_back(tok);

    try {
        Parser parser(tokens);
        int result = parser.E();
        cout << "Result = " << result << "\n";
    } catch (exception &e) {
        cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
