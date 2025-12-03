class CFG:
    def __init__(self):
        self.rules = []

    def add_rule(self, left, right):
        self.rules.append((left, right))

    def replace_terminals(self):
        term_map = {}
        out = []
        tcount = 1

        for left, right in self.rules:
            new_right = []
            for sym in right:
                if sym.islower() and len(right) > 1:
                    if sym not in term_map:
                        term_map[sym] = f"T{tcount}"
                        tcount += 1
                        out.append((term_map[sym], [sym]))
                    new_right.append(term_map[sym])
                else:
                    new_right.append(sym)
            out.append((left, new_right))
        self.rules = out

    def binarize(self):
        out = []
        vcount = 1

        for left, right in self.rules:
            if len(right) <= 2:
                out.append((left, right))
                continue
            prev = right[0]
            for s in right[1:-1]:
                new_var = f"X{vcount}"
                vcount += 1
                out.append((new_var, [prev, s]))
                prev = new_var
            out.append((left, [prev, right[-1]]))
        self.rules = out

    def convert_cnf(self):
        self.replace_terminals()
        self.binarize()

    def print(self):
        for left, right in self.rules:
            print(f"{left} -> {' '.join(right)}")

g = CFG()
g.add_rule("S", ["A", "B", "C"])
g.add_rule("A", ["a"])
g.add_rule("B", ["b"])
g.add_rule("C", ["c", "D"])
g.add_rule("D", ["d"])

print("Input CFG:")
g.print()

g.convert_cnf()

print("\nCNF Grammar:")
g.print()
