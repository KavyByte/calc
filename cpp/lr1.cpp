// lr(1)

#include <bits/stdc++.h>
using namespace std;

struct Item {
    string left;
    string right;
    int dot;
    char lookahead;

    bool operator<(Item const &o) const {
        return tie(left, right, dot, lookahead) < tie(o.left, o.right, o.dot, o.lookahead);
    }

    bool operator==(Item const& o) const {
        return left == o.left && right == o.right && dot == o.dot && lookahead == o.lookahead;
    }
};

map<string, vector<string>> grammar;
set<string> nonterminals;
set<char> terminals;
map<string, set<char>> FIRST;

bool isNonterminal(string s) {
    return s.size() == 1 && isupper(s[0]);
}

set<char> firstOfString(string beta) {
    set<char> res;
    bool epsilon = true;
    for (char ch : beta) {
        if (isupper(ch)) {
            for (char f : FIRST[string(1, ch)]) {
                if (f != '#') res.insert(f);
            }
            if (!FIRST[string(1, ch)].count('#')) {
                epsilon = false;
                break;
            }
        } else {
            res.insert(ch);
            epsilon = false;
            break;
        }
    }
    if (epsilon) res.insert('#');
    return res;
}

set<Item> closure(set<Item> I) {
    bool changed = true;

    while (changed) {
        changed = false;
        set<Item> newItems = I;

        for (auto &it : I) {
            if (it.dot < it.right.size()) {
                char B = it.right[it.dot];
                if (isupper(B)) {
                    string Bstr(1, B);

                    string beta = it.right.substr(it.dot + 1);
                    if (beta == "") beta = string(1, it.lookahead);

                    set<char> LA = firstOfString(beta);

                    for (auto &prod : grammar[Bstr]) {
                        for (char a : LA) {
                            Item newItem = {Bstr, prod, 0, a};
                            if (!newItems.count(newItem)) {
                                newItems.insert(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        I = newItems;
    }
    return I;
}

set<Item> go_to(const set<Item> &I, char X) {
    set<Item> J;
    for (auto &it : I) {
        if (it.dot < it.right.size() && it.right[it.dot] == X) {
            J.insert({it.left, it.right, it.dot + 1, it.lookahead});
        }
    }
    return closure(J);
}


vector< set<Item> > C;
map< pair<int,char>, int > GOTO;
map< pair<int,char>, string > ACTION;

void buildLR1Automaton() {
    string start = "S'";
    grammar[start] = { "S" };
    nonterminals.insert("S'");

    Item startItem = { "S'", "S", 0, '$' };
    C.push_back( closure({startItem}) );

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < C.size(); i++) {
            for (char X : terminals) {
                set<Item> J = go_to(C[i], X);
                if (!J.empty()) {
                    if (find(C.begin(), C.end(), J) == C.end()) {
                        C.push_back(J);
                        GOTO[{i, X}] = C.size() - 1;
                        changed = true;
                    } else {
                        int idx = find(C.begin(), C.end(), J) - C.begin();
                        GOTO[{i, X}] = idx;
                    }
                }
            }
            for (string nt : nonterminals) {
                char X = nt[0];
                set<Item> J = go_to(C[i], X);
                if (!J.empty()) {
                    if (find(C.begin(), C.end(), J) == C.end()) {
                        C.push_back(J);
                        GOTO[{i, X}] = C.size() - 1;
                        changed = true;
                    } else {
                        int idx = find(C.begin(), C.end(), J) - C.begin();
                        GOTO[{i, X}] = idx;
                    }
                }
            }
        }
    }
}

void buildACTIONTable() {
    for (int i = 0; i < C.size(); i++) {
        for (auto &it : C[i]) {
            if (it.dot < it.right.size()) {
                char a = it.right[it.dot];
                if (!isupper(a)) {
                    if (GOTO.count({i, a})) {
                        ACTION[{i, a}] = "s" + to_string(GOTO[{i, a}]);
                    }
                }
            } else {
                if (it.left == "S'" && it.right == "S" && it.lookahead == '$') {
                    ACTION[{i, '$'}] = "acc";
                } else {
                    for (char a : {it.lookahead}) {
                        ACTION[{i, a}] = "r(" + it.left + "->" + it.right + ")";
                    }
                }
            }
        }
    }
}

void parse(string input) {
    input.push_back('$');

    vector<int> stateStack;
    vector<char> symbolStack;

    stateStack.push_back(0);
    int ip = 0;

    cout << "\nLR(1) Parsing Steps:\n";
    cout << left << setw(15) << "StateStack"
         << setw(15) << "SymbolStack"
         << setw(15) << "Input"
         << "Action\n";

    while (true) {
        int state = stateStack.back();
        char a = input[ip];

        string action = ACTION[{state, a}];

        string ss, sy;
        for (int s : stateStack) ss += to_string(s) + " ";
        for (char c : symbolStack) sy += c;

        cout << setw(15) << ss
             << setw(15) << sy
             << setw(15) << input.substr(ip)
             << action << "\n";

        if (action == "") {
            cout << "ERROR: No ACTION rule!\n";
            return;
        }

        if (action[0] == 's') {
            int st = stoi(action.substr(1));
            symbolStack.push_back(a);
            stateStack.push_back(st);
            ip++;

        } else if (action[0] == 'r') {
            string rule = action.substr(2, action.size() - 3);
            string left = rule.substr(0, rule.find("->"));
            string right = rule.substr(rule.find("->") + 2);

            for (int k = 0; k < right.size(); k++) {
                symbolStack.pop_back();
                stateStack.pop_back();
            }

            symbolStack.push_back(left[0]);
            int s = stateStack.back();
            stateStack.push_back(GOTO[{s, left[0]}]);

        } else if (action == "acc") {
            cout << "Input Accepted!\n";
            return;
        }
    }
}

int main() {
    int n;
    cout << "Enter number of productions:\n";
    cin >> n;

    cout << "Enter productions (e.g., S A|b):\n";

    for (int i = 0; i < n; i++) {
        string left, right;
        cin >> left >> right;

        nonterminals.insert(left);

        string cur = "";
        for (char c : right) {
            if (c == '|') {
                grammar[left].push_back(cur);
                cur.clear();
            } else {
                cur.push_back(c);
                if (!isupper(c)) terminals.insert(c);
            }
        }
        if (!cur.empty()) grammar[left].push_back(cur);
    }

    FIRST.clear();
    for (auto &p : grammar) {
        for (auto &prod : p.second) {
            for (char c : prod) {
                if (!isupper(c)) {
                    terminals.insert(c);
                }
            }
        }
    }
    FIRST["$"].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            string A = p.first;

            for (string rhs : p.second) {
                if (!isupper(rhs[0])) {
                    if (!FIRST[A].count(rhs[0])) {
                        FIRST[A].insert(rhs[0]);
                        changed = true;
                    }
                } else {
                    for (char f : FIRST[string(1, rhs[0])]) {
                        if (!FIRST[A].count(f)) {
                            FIRST[A].insert(f);
                            changed = true;
                        }
                    }
                }
            }
        }
    }

    buildLR1Automaton();
    buildACTIONTable();

    cout << "\nEnter input string to parse:\n";
    string input;
    cin >> input;

    parse(input);
}
