def tokenize(expr):
    tokens = []
    i = 0
    while i < len(expr):
        if expr[i].isspace():
            i += 1
            continue
        if expr[i].isalpha():
            tokens.append("ID")
            i += 1
            continue
        if expr[i] in "+-*/()":
            tokens.append(expr[i])
            i += 1
            continue
        raise Exception("Invalid character: " + expr[i])
    tokens.append("$")
    return tokens

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.index = 0

    def current(self):
        return self.tokens[self.index]

    def match(self, token):
        if self.current() == token:
            print(f"Match {token}")
            self.index += 1
        else:
            raise Exception(f"Syntax Error: Expected {token}, got {self.current()}")
    
    def parse_E(self):
        print("E -> T E'")
        self.parse_T()
        self.parse_Ep()

    def parse_Ep(self):
        if self.current() == "+":
            print("E' -> + T E'")
            self.match("+")
            self.parse_T()
            self.parse_Ep()
        elif self.current() == "-":
            print("E' -> - T E'")
            self.match("-")
            self.parse_T()
            self.parse_Ep()
        else:
            print("E' -> ε")
    
    def parse_T(self):
        print("T -> F T'")
        self.parse_F()
        self.parse_Tp()

    def parse_Tp(self):
        if self.current() == "*":
            print("T' -> * F T'")
            self.match("*")
            self.parse_F()
            self.parse_Tp()
        elif self.current() == "/":
            print("T' -> / F T'")
            self.match("/")
            self.parse_F()
            self.parse_Tp()
        else:
            print("T' -> ε")

    def parse_F(self):
        if self.current() == "(":
            print("F -> ( E )")
            self.match("(")
            self.parse_E()
            self.match(")")
        elif self.current() == "ID":
            print("F -> ID")
            self.match("ID")
        else:
            raise Exception(f"Syntax Error: Unexpected symbol {self.current()}")

def recursive_descent(expr):
    tokens = tokenize(expr)
    print("Tokens:", tokens)
    parser = Parser(tokens)

    parser.parse_E()

    if parser.current() == "$":
        print("\nACCEPT")
    else:
        print("\nREJECT")

expr = "a + b * (c - d)"
recursive_descent(expr)

