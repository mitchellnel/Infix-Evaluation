# Infix Evaluation
The evaluate() function in eval.cpp evaluates an infix integer arithmetic expression that consists of the binary operators +, -, \*, and /, parentheses, and operands (with blanks allowed for readability).

The / operator denotes integer division (with truncation), so that the value of “ 8 / 5 ” is 1 (not 1.6).

Operators have their conventional precedence and associativity.

Multiplication must be explicitly indicated with the * operator.

The operands in the expression are single lower case letters. Along with the expression string, the function is passed a [Map](https://github.com/mitchellnel/Map-Implementations/tree/master/Map%20(as%20Linked%20List)/Map/Map) with KeyType char, and ValueType int.

Each letter character in the expression represents the integer value in the map that is paired with that letter key. For example, if the Map maps a to 3, c to 5, 1 to 2, and u to 11, then the expression “ u – c + 1 * a ” would evaluate to 12.

The function prototype is:
``` int evaluate(string infix, const Map& values, string& postfix, int& result); ```

The function also detects whether the infix string is syntactically valid. The pseudocode for the validateInfix() and isValid() functions (used to help validate the infix string is syntactically valid) is shown below:
```
 remove spaces from infix string
 keep track of number of parentheses
 
 check first char in infix
    if it's an operand
        okay
    if it's a '('
        increment nParentheses
    else
        invalid string
 
 for every char in the infix string, barring the very last one
    keep track of current char
    keep track of prev char
    keep track of next char
 
    if curr char is operand
        check if valid in context
            prev must be either operator or '('
            next must be either operator or ')'
            else invalid
    
    if curr char is '('
        if valid in context (prev must be either operator or another '('; next must be operand or another '('
            increment nParentheses
        else
            invalid string
 
    if curr char is ')'
        if valid in context (prev must be either operand or another ')'; next must be either operator or ')'
            decrement nParentheses
            if nParenthese negative
                invalid string
        else
            invalid string
 
    if curr char is operator
        check if valid in context
            prev must be either operand or ')'
            next must be either operand or '('
        else
            invalid string
 end loop
 
 check last char in infix
    if it's an operand
        if prev char isn't operator
            invalid string
    if it's a ')'
        if prev char isn't operand or ')'
            invalid string
        decrement nParentheses
        if nParentheses not 0
            invalid string
    else anything other than that
        invalid string
```

The actual evaluation portion of the function uses a stack-based infix evaluation algorithm.
