from collections import defaultdict

grammar = {
    "E":  [["T", "E'"]],
    "E'": [["+","T","E'"], ["-","T","E'"], ["ε"]],
    "T":  [["F","T'"]],
    "T'": [["*","F","T'"], ["/","F","T'"], ["ε"]],
    "F":  [["(","E",")"], ["ID"]]
}

def is_terminal(symbol):
    return symbol not in grammar and symbol != "ε"

FIRST = defaultdict(set)

def compute_first(symbol):

    if is_terminal(symbol):
        return {symbol}


    if FIRST[symbol]:
        return FIRST[symbol]

    for production in grammar[symbol]:
    
        if production == ["ε"]:
            FIRST[symbol].add("ε")
            continue

        for sym in production:
            sym_first = compute_first(sym)
            FIRST[symbol].update(sym_first - {"ε"})
            
            if "ε" not in sym_first:
                break
        else:
            FIRST[symbol].add("ε")

    return FIRST[symbol]


FOLLOW = defaultdict(set)

def compute_follow():

    start_symbol = list(grammar.keys())[0]
    FOLLOW[start_symbol].add("$")

    changed = True
    while changed:
        changed = False

        for lhs in grammar:
            for production in grammar[lhs]:
                for i, B in enumerate(production):
                    if not B in grammar:
                        continue 

                    beta = production[i+1:]

                    if beta:
                        first_beta = set()
                    
                        for sym in beta:
                            sym_first = FIRST[sym] if sym in grammar else {sym}
                            first_beta.update(sym_first - {"ε"})
                            if "ε" not in sym_first:
                                break
                        else:
                            first_beta.add("ε")
                    
                        before = len(FOLLOW[B])
                        FOLLOW[B].update(first_beta - {"ε"})
                        if len(FOLLOW[B]) > before:
                            changed = True

                        if "ε" in first_beta:
                            before = len(FOLLOW[B])
                            FOLLOW[B].update(FOLLOW[lhs])
                            if len(FOLLOW[B]) > before:
                                changed = True
                    else:                    
                        before = len(FOLLOW[B])
                        FOLLOW[B].update(FOLLOW[lhs])
                        if len(FOLLOW[B]) > before:
                            changed = True

for nt in grammar:
    compute_first(nt)

compute_follow()

print("\nFIRST sets:")
for nt in grammar:
    print(f"FIRST({nt}) = {{ {', '.join(sorted(FIRST[nt]))} }}")

print("\nFOLLOW sets:")
for nt in grammar:
    print(f"FOLLOW({nt}) = {{ {', '.join(sorted(FOLLOW[nt]))} }}")
