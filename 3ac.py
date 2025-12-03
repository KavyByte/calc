class ThreeAddressCodeGenerator:
    def __init__(self):
        self.temp_count = 0
        self.code = []

    def new_temp(self):
        self.temp_count += 1
        return f"t{self.temp_count}"

    def generate(self, expr):
        """Generate 3AC for the given arithmetic expression."""
        expr = expr.replace(" ", "")  
        return self._gen_expr(expr)

    def _gen_expr(self, expr):
        
        if expr.isalnum():
            return expr

        
        if expr.startswith("(") and expr.endswith(")"):
            return self._gen_expr(expr[1:-1])

        
        for op in ['+', '-']:
            level = 0
            for i in range(len(expr)-1, -1, -1):
                if expr[i] == ')':
                    level += 1
                elif expr[i] == '(':
                    level -= 1
                elif level == 0 and expr[i] == op:
                    left = self._gen_expr(expr[:i])
                    right = self._gen_expr(expr[i+1:])
                    temp = self.new_temp()
                    self.code.append(f"{temp} = {left} {op} {right}")
                    return temp

        for op in ['*', '/']:
            level = 0
            for i in range(len(expr)-1, -1, -1):
                if expr[i] == ')':
                    level += 1
                elif expr[i] == '(':
                    level -= 1
                elif level == 0 and expr[i] == op:
                    left = self._gen_expr(expr[:i])
                    right = self._gen_expr(expr[i+1:])
                    temp = self.new_temp()
                    self.code.append(f"{temp} = {left} {op} {right}")
                    return temp

        return expr  


expr = "a + b * (c - d) / e"
tac_gen = ThreeAddressCodeGenerator()
result = tac_gen.generate(expr)

print("Three-Address Code:")
for line in tac_gen.code:
    print(line)

