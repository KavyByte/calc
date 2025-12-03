precedence = {
    "ID":     {"ID": ">", "PLUS": ">", "MINUS": ">", "TIMES": ">", "DIVIDE": ">", "LPAREN": ">", "RPAREN": ">", "$": ">"},
    "PLUS":   {"ID": "<", "PLUS": ">", "MINUS": ">", "TIMES": "<", "DIVIDE": "<", "LPAREN": "<", "RPAREN": ">", "$": ">"},
    "MINUS":  {"ID": "<", "PLUS": ">", "MINUS": ">", "TIMES": "<", "DIVIDE": "<", "LPAREN": "<", "RPAREN": ">", "$": ">"},
    "TIMES":  {"ID": "<", "PLUS": ">", "MINUS": ">", "TIMES": ">", "DIVIDE": ">", "LPAREN": "<", "RPAREN": ">", "$": ">"},
    "DIVIDE": {"ID": "<", "PLUS": ">", "MINUS": ">", "TIMES": ">", "DIVIDE": ">", "LPAREN": "<", "RPAREN": ">", "$": ">"},
    "LPAREN": {"ID": "<", "PLUS": "<", "MINUS": "<", "TIMES": "<", "DIVIDE": "<", "LPAREN": "<", "RPAREN": "=", "$": None},
    "RPAREN": {"ID": None, "PLUS": None, "MINUS": ">", "TIMES": ">", "DIVIDE": ">", "LPAREN": ">", "RPAREN": None, "$": ">"},
    "$":      {"ID": "<", "PLUS": "<", "MINUS": "<", "TIMES": "<", "DIVIDE": "<", "LPAREN": "<", "RPAREN": None, "$": "="}
}

def parser(tokens):
    stack = ['$']
    tokens.append('$')
    input_index = 0

    print(f"Stack,Input,Action")
    
    while True:
        stack_top = stack[-1]
        input_token = tokens[input_index]

        stack_str = " ".join(stack)
        input_str = " ".join(tokens[input_index:])
        print(f"{stack_str},{input_str},", end="")

        if stack_top == '$' and input_token == '$':
            print("Accept")
            break
            
        if stack_top not in precedence or input_token not in precedence[stack_top]:
            print("Error")
            break

        relation = precedence[stack_top][input_token]

        if relation == '<':
            print("Shift")
            stack.append(input_token)
            input_index += 1
        elif relation == '>':
            print("Reduce")

            while True:
                popped_token = stack.pop()
                if not stack or stack[-1] not in precedence or popped_token not in precedence[stack[-1]]:
                    break
                if precedence[stack[-1]][popped_token] == '<':
                    break
        elif relation == '=':
            print("Match and Pop")
            stack.pop()
            input_index += 1
        else:
            print("Error")
            break
