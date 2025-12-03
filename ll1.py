# E  → T E'
# E' → + T E' | - T E' | ε
# T  → F T'
# T' → * F T' | / F T' | ε
# F  → ( E ) | ID

table = {
    "E":  { "ID": ["T", "E'"],     "(": ["T", "E'"] },
    "E'": { "+": ["+", "T", "E'"], "-": ["-", "T", "E'"], ")": ["ε"], "$": ["ε"] },
    "T":  { "ID": ["F", "T'"],     "(": ["F", "T'"] },
    "T'": { "*": ["*", "F", "T'"], "/": ["/", "F", "T'"], ")": ["ε"], "+": ["ε"], "-": ["ε"], "$": ["ε"] },
    "F":  { "ID": ["ID"],          "(": ["(", "E", ")"] }
}

def ll1_parse(tokens):
    tokens.append("$")             
    stack = ["$", "E"]             
    index = 0
    print("Stack\t\tInput\t\tAction")

    while True:
        top = stack[-1]
        current = tokens[index]

        print(f"{stack}\t{tokens[index:]}\t", end="")

        if top == "$" and current == "$":
            print("ACCEPT")
            return True

        
        if top == current:
            print(f"Match '{current}'")
            stack.pop()
            index += 1
            continue

        if not top.isupper():
            print("ERROR (terminal mismatch)")
            return False
        
        if top not in table or current not in table[top]:
            print("ERROR (no rule)")
            return False
        production = table[top][current]
        print(f"{top} → {' '.join(production)}")

        stack.pop()

        if production != ["ε"]:
            for symbol in reversed(production):
                stack.append(symbol)
