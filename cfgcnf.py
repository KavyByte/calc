class CFG:
    def __init__(self):
        self.rules = []   

    def add_rule(self, left, right):
        if isinstance(right, str):
            if right == "Null":
                self.rules.append((left, ["Null"]))
            else:
                self.rules.append((left, right.split()))
        else:
            if right == []:
                self.rules.append((left, ["Null"]))
            else:
                self.rules.append((left, right))

    def remove_null_productions(self):
        nullable = set()
        changed = True
        while changed:
            changed = False
            for left, right in self.rules:
                
                if right == ["Null"]:
                    if left not in nullable:
                        nullable.add(left)
                        changed = True
                
                elif all(sym in nullable for sym in right):
                    if left not in nullable:
                        nullable.add(left)
                        changed = True

        new_rules = []
        from itertools import product

        for left, right in self.rules:
            if right == ["Null"]:
                continue  

            nullable_positions = [i for i, sym in enumerate(right) if sym in nullable]

            
            if not nullable_positions:
                new_rules.append((left, right))
                continue

            
            k = len(nullable_positions)
            for mask in product([0, 1], repeat=k):
                to_remove = {nullable_positions[i] for i, m in enumerate(mask) if m == 1}

                new_rhs = [sym for i, sym in enumerate(right) if i not in to_remove]

                if not new_rhs:
                    continue  

                if (left, new_rhs) not in new_rules:
                    new_rules.append((left, new_rhs))

        
        start = self.rules[0][0]
        if start in nullable:
            new_rules.append((start, ["Null"]))

        self.rules = new_rules

    
    def replace_terminals(self):
        term_map = {}
        out = []
        tcount = 1

        for left, right in self.rules:
            if right == ["Null"]:
                out.append((left, right))
                continue

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
            if right == ["Null"]:
                out.append((left, right))
                continue

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
        self.remove_null_productions()
        self.replace_terminals()
        self.binarize()

    def print(self):
        for left, right in self.rules:
            print(f"{left} -> {' '.join(right)}")

g = CFG()
g.add_rule("S", ["A", "B", "C"])
g.add_rule("S", ["a"])
g.add_rule("A", ["a"])
g.add_rule("B", ["b"])
g.add_rule("C", ["c", "D"])
g.add_rule("D", ["d"])
g.add_rule("D", "Null")  

print("Input CFG:")
g.print()

g.convert_cnf()

print("\nCNF Grammar:")
g.print()
