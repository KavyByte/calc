#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
map<pair<char, char>, string> parsingTable;
set<char> visitedFirst, visitedFollow;
char startSymbol;

void computeFirst(char symbol) {
    if (visitedFirst.count(symbol)) return;
    visitedFirst.insert(symbol);

    for (string prod : grammar[symbol]) {
        bool epsilon = true;

        for (char ch : prod) {
            if (isupper(ch)) {
                computeFirst(ch);
                for (char f : firstSet[ch])
                    if (f != '#') firstSet[symbol].insert(f);

                if (!firstSet[ch].count('#')) {
                    epsilon = false;
                    break;
                }
            } else {
                firstSet[symbol].insert(ch);
                epsilon = false;
                break;
            }
        }
        if (epsilon) firstSet[symbol].insert('#');
    }
}

void computeFollow(char symbol) {
    if (visitedFollow.count(symbol)) return;
    visitedFollow.insert(symbol);

    if (symbol == startSymbol)
        followSet[symbol].insert('$');

    for (auto &p : grammar) {
        char A = p.first;

        for (string prod : p.second) {
            for (int i = 0; i < prod.size(); i++) {
                if (prod[i] == symbol) {
                    bool epsilon = true;

                    for (int j = i + 1; j < prod.size(); j++) {
                        char B = prod[j];
                        if (isupper(B)) {
                            for (char f : firstSet[B])
                                if (f != '#') followSet[symbol].insert(f);

                            if (!firstSet[B].count('#')) {
                                epsilon = false;
                                break;
                            }
                        } else {
                            followSet[symbol].insert(B);
                            epsilon = false;
                            break;
                        }
                    }
                    if (epsilon && A != symbol) {
                        computeFollow(A);
                        for (char f : followSet[A]) followSet[symbol].insert(f);
                    }
                }
            }
        }
    }
}

void buildTable() {
    for (auto &p : grammar) {
        char A = p.first;

        for (string prod : p.second) {
            set<char> firstAlpha;
            bool epsilon = true;

            for (char ch : prod) {
                if (isupper(ch)) {
                    for (char f : firstSet[ch])
                        if (f != '#') firstAlpha.insert(f);

                    if (!firstSet[ch].count('#')) {
                        epsilon = false;
                        break;
                    }
                } else {
                    firstAlpha.insert(ch);
                    epsilon = false;
                    break;
                }
            }
            if (epsilon) firstAlpha.insert('#');

            for (char t : firstAlpha)
                if (t != '#') parsingTable[{A, t}] = prod;

            if (firstAlpha.count('#')) {
                for (char b : followSet[A])
                    parsingTable[{A, b}] = prod;
            }
        }
    }
}

void runParser(const string &input) {
    cout << "\n\n=== LL(1) Parsing Process ===\n";
    stack<char> st;
    st.push('$');
    st.push(startSymbol);

    string ip = input + "$";
    int ptr = 0;

    cout << left << setw(20) << "Stack"
         << setw(20) << "Input"
         << setw(20) << "Action\n";

    while (!st.empty()) {
        string stackContent;
        stack<char> temp = st;

        while (!temp.empty()) {
            stackContent.push_back(temp.top());
            temp.pop();
        }
        reverse(stackContent.begin(), stackContent.end());

        string inputRemaining = ip.substr(ptr);

        cout << setw(20) << stackContent
             << setw(20) << inputRemaining;

        char top = st.top();
        char cur = ip[ptr];

        if (!isupper(top)) {
            if (top == cur) {
                cout << setw(20) << "Match " + string(1, cur) << "\n";
                st.pop();
                ptr++;
            } else {
                cout << "ERROR: expected " << top << ", got " << cur << "\n";
                return;
            }
        } else {
            if (parsingTable.count({top, cur})) {
                string prod = parsingTable[{top, cur}];
                cout << setw(20) << string(1, top) + " → " + prod << "\n";
                st.pop();
                if (prod != "#")
                    for (int i = prod.size() - 1; i >= 0; i--)
                        st.push(prod[i]);
            } else {
                cout << "ERROR: no rule for (" << top << ", " << cur << ")\n";
                return;
            }
        }
    }

    cout << "\nInput string accepted successfully.\n";
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions A->alpha (use # for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string prod;
        cin >> prod;
        char lhs = prod[0];
        string rhs = prod.substr(3);
        grammar[lhs].push_back(rhs);
        if (i == 0) startSymbol = lhs;
    }

    for (auto &p : grammar) computeFirst(p.first);
    for (auto &p : grammar) computeFollow(p.first);
    buildTable();

    cout << "\n=== FIRST Sets ===\n";
    for (auto &p : firstSet) {
        cout << p.first << " : { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }

    cout << "\n=== FOLLOW Sets ===\n";
    for (auto &p : followSet) {
        cout << p.first << " : { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }

    cout << "\n=== Parsing Table ===\n";
    for (auto &p : parsingTable) {
        cout << "(" << p.first.first << ", " << p.first.second << ") → "
             << p.second << "\n";
    }

    string input;
    cout << "\nEnter input string to parse: ";
    cin >> input;

    runParser(input);

    return 0;
}
