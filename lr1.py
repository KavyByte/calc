#   E  → T E'
#   E' → + T E' | - T E' | ε
#   T  → F T'
#   T' → * F T' | / F T' | ε
#   F  → ( E ) | ID

from collections import defaultdict

grammar = {
    "S'": [["E"]],
    "E":  [["T", "E'"]],
    "E'": [["+","T","E'"], ["-","T","E'"], ["ε"]],
    "T":  [["F","T'"]],
    "T'": [["*","F","T'"], ["/","F","T'"], ["ε"]],
    "F":  [["(","E",")"], ["ID"]]
}

terminals = ["ID","+","-","*","/","(",")","$"]
nonterminals = list(grammar.keys())

def closure(items):
    changed = True
    while changed:
        changed = False
        new_items = set(items)

        for (lhs, rhs, dot, la) in items:
            if dot < len(rhs):
                B = rhs[dot]
                if B in grammar: 
                    next_symbols = rhs[dot+1:] + [la]
                    first_set = FIRST(next_symbols)

                    for prod in grammar[B]:
                        for f in first_set:
                            item = (B, tuple(prod), 0, f)
                            if item not in new_items:
                                new_items.add(item)
                                changed = True

        items = new_items
    return items

def goto(items, symbol):
    moved = set()
    for (lhs, rhs, dot, la) in items:
        if dot < len(rhs) and rhs[dot] == symbol:
            moved.add((lhs, rhs, dot+1, la))
    return closure(moved)

def FIRST(seq):
    if not seq:
        return set(["ε"])
    first = set()
    sym = seq[0]

    if sym in terminals:
        return set([sym])

    for prod in grammar[sym]:
        if prod == ["ε"]:
            first |= FIRST(seq[1:])
        else:
            first |= FIRST(prod)
    return first

def canonical_collection():
    start_item = {("S'", ("E",), 0, "$")}
    C = [closure(start_item)]
    done = False

    while not done:
        done = True
        for I in C:
            for sym in terminals + list(grammar.keys()):
                g = goto(I, sym)
                if g and g not in C:
                    C.append(g)
                    done = False
    return C

C = canonical_collection()

ACTION = defaultdict(dict)
GOTO   = defaultdict(dict)

for i, I in enumerate(C):
    for (lhs, rhs, dot, la) in I:

        if dot < len(rhs) and rhs[dot] in terminals:
            j = C.index(goto(I, rhs[dot]))
            ACTION[i][rhs[dot]] = ("shift", j)

        if dot == len(rhs):
            if lhs == "S'":
                ACTION[i]["$"] = ("accept",)
            else:
                ACTION[i][la] = ("reduce", lhs, list(rhs))

        if dot < len(rhs) and rhs[dot] in grammar:
            j = C.index(goto(I, rhs[dot]))
            GOTO[i][rhs[dot]] = j

def lr1_parse(tokens):
    tokens.append("$")
    stack = [0] 
    index = 0

    print("Stack\t\tInput\t\tAction")

    while True:
        state = stack[-1]
        current = tokens[index]

        print(f"{stack}\t{tokens[index:]}\t", end="")

        if current not in ACTION[state]:
            print("ERROR")
            return False

        action = ACTION[state][current]

        if action[0] == "shift":
            print(f"Shift → state {action[1]}")
            stack.append(current)
            stack.append(action[1])
            index += 1

        elif action[0] == "reduce":
            lhs, rhs = action[1], action[2]
            print(f"Reduce: {lhs} → {' '.join(rhs)}")

            if rhs != ["ε"]:
                for _ in rhs:
                    stack.pop()
                    stack.pop()

            state2 = stack[-1]
            stack.append(lhs)
            stack.append(GOTO[state2][lhs])

        elif action[0] == "accept":
            print("ACCEPT")
            return True
