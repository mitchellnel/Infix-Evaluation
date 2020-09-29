//
//  eval.cpp
//  Homework 2
//
//  Created by Mitchell McLinton on 29/1/20.
//  Copyright © 2020 Mitchell McLinton. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

/////////////////////////////////////////////////////
// evaluate PROTOTYPE
/////////////////////////////////////////////////////
int evaluate(string infix, const Map& values, string& postfix, int& result);
  // Evaluates an integer arithmetic expression
  //   If infix is a syntactically valid infix integer expression whose
  //   only operands are single lower case letters (whether or not they
  //   appear in the values map), then postfix is set to the postfix
  //   form of the expression; otherwise postfix may or may not be
  //   changed, result is unchanged, and the function returns 1.

  //   If infix is syntactically valid but contains at least one lower
  //   case letter operand that does not appear in the values map, then
  //   result is unchanged and the function returns 2.

  //   If infix is syntactically valid and all its lower case operand letters
  //   appear in the values map, then if evaluating the expression
  //   (using for each letter in the expression the value in the map
  //   that corresponds to it) attempts to divide by zero, then result
  //   is unchanged and the function returns 3;

  //   otherwise, result is set to the value of the expression and the function returns 0.

///////////////////////////////////////////////////////////////////////////
// AUXILIARY FUNCTION PROTOTYPES
///////////////////////////////////////////////////////////////////////////
int precedence(char op);
  // Returns a number that represents the precedence of the operation
  //   A higher number indicates a higher precedence
  //   Return -1 if the character isn't an operator

bool isOperator(char ch);
  // Whether the character is an operator
  //   Use precedence
  //   One line

bool isOperand(char ch);
  // Whether the character is an operand (lowercase letter)
  //   One line

bool isValid(char ch, char prev, char next);
  // Whether the character is valid in context
  //   For example, an operand isn't valid after a right parenthesis.
  //   Use isOperator, isOperand

void removeSpaces(string& str);
  // Removes spaces from string passed-by-reference

bool validateInfix(string infix);
  // Validates the infix expression
  //    Use isOperator, isOperand, isValid, removeSpaces

string toPostfix(string infix);
  // Removes spaces and converts the infix string to postfix (pseudocode provided for this algorithm)
  //   Use validateInfix, removeSpaces

/////////////////////////////////////////////////////////
// evaluate IMPLEMENTATION
/////////////////////////////////////////////////////////
int evaluate(string infix, const Map& values, string& postfix, int& result) {
    if (toPostfix(infix) == "invalid")
        return 1;
    else
        postfix = toPostfix(infix);
    
    stack<int> operandStack;
    
    for (int i = 0; i != postfix.size(); i++) {
        char ch = postfix[i];
        if (isOperand(ch)) {
            int operand;
            if (values.contains(ch)) {  // retrieve value from Map
                values.get(ch, operand);
                operandStack.push(operand);
            } else {
                return 2;   // letter operand does not appear in Map
            }
        } else {  // ch is a binary operator
            int operand2 = operandStack.top();
            operandStack.pop();
            
            int operand1 = operandStack.top();
            operandStack.pop();
            
            switch (ch) {
                case '+':
                    operandStack.push(operand1 + operand2);
                    break;
                    
                case '-':
                    operandStack.push(operand1 - operand2);
                    break;
                    
                case '*':
                    operandStack.push(operand1 * operand2);
                    break;
                    
                case '/':
                    if (operand2 != 0)
                        operandStack.push(operand1 / operand2);
                    else
                        return 3; // tried to divide by zero
            }
        }
    }
    
    // when the loop has finished, operand stack will contain one item, which is the result of the expression
    result = operandStack.top();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// AUXILIARY FUNCTION IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
            break;
            
        case '*':
        case '/':
            return 2;
            break;
            
        default:
            return -1;
            break;
    }
}

bool isOperator(char ch) {
    return precedence(ch) != -1;    // precedence returns -1 for non-operator char
}

bool isOperand(char ch) {
    return islower(ch);
}

bool isValid(char ch, char prev, char next) {
    if (isOperand(ch)) {
        // operand ch is valid in context if:
        //      prev is either operator or '('
        if (!isOperator(prev) && prev != '(')
            return false;
        //      next is either operator or ')'
        if (!isOperator(next) && next != ')')
            return false;
    } else if (ch == '(') {
        // '(' is valid in context if:
        //      prev is either operator or '('
        if (!isOperator(prev) && prev != '(')
            return false;
        //      next is either operand or '('
        if (!isOperand(next) && next != '(')
            return false;
    } else if (ch == ')') {
        // ')' is valid in context if:
        //      prev is either operand or ')'
        if (!isOperand(prev) && prev != ')')
            return false;
        //      next is either operator or ')'
        if (!isOperator(next) && next != ')')
            return false;
    } else if (isOperator(ch)) {
        // operator ch is valid in context if:
        //      prev is either operand or ')'
        if (!isOperand(prev) && prev != ')')
            return false;
        //      next is either operand or '('
        if (!isOperand(next) && next != '(')
            return false;
    } else {
        // something invalid passed
        return false;
    }
    
    return true;    // we only reached this if it passed its valid context test
}

void removeSpaces(string& str) {
    for (int i = 0; i != str.size(); i++) {
        if (str[i] == ' ') {
            for (int j = i+1; j != str.size(); j++) {   // left shift of chars
                str[j-1] = str[j];
            }
            str.resize(str.size()-1);   // drop the last character
            i--;    // decrement outer loop counter to check the current char again
        }
    }
}

bool validateInfix(string infix) {
    removeSpaces(infix);    // this should only be called with a spaceless infix, but we call again to make sure
    
    // Keep track of number of parentheses
    int nParentheses = 0;
    
    // Invalid cases:
    if (infix.empty()) return false;    // empty string
    if (infix.size() == 1 && !isOperand(infix[0])) return false;    // lone operator/parentheses
    
    // Automatically valid cases:
    if (infix.size() == 1 && isOperand(infix[0])) return true;      // lone operand
    
    // Check first char
    // First char of an infix can only be an operand or a '('
    if (infix[0] == '(')
        nParentheses++;
    else if (!isOperand(infix[0]))
        return false;
    
    for (int i = 1; i != infix.size()-1; i++) {
        char ch = infix[i];
        char prev = infix[i-1];
        char next = infix[i+1];
        
        if (isOperand(ch)) {
            if (!isValid(ch, prev, next))
                return false;
        } else if (ch == '(') {
            if (isValid(ch, prev, next))
                nParentheses++;
            else
                return false;
        } else if (ch == ')') {
            if (isValid(ch, prev, next)) {
                nParentheses--;
                if (nParentheses < 0)
                    return false;
            } else {
                return false;
            }
        } else if (isOperator(ch)) {
            if (!isValid(ch, prev, next))
                return false;
        } else {
            // something invalid passed
            return false;
        }
    }
    
    // Check last char
    // Last char of infix can only be an operand or a ')'
    if (infix[infix.size()-1] == ')') {
        // () is not valid syntax
        if (infix[infix.size()-2] == '(')
            return false;
        nParentheses--;
        if (nParentheses != 0)
            return false;
    } else if (isOperand(infix[infix.size()-1])) {
        if (!isOperator(infix[infix.size()-2]))
            return false;
    } else {
        return false;
    }
    
    return true;   // we only reach this if valid infix
}

string toPostfix(string infix) {    // "invalid" returned if invalid syntax
    removeSpaces(infix);
    if (!validateInfix(infix))
        return "invalid";
    
    string postfix = "";
    stack<char> operatorStack;
    
    for (int i = 0; i != infix.size(); i++) {
        char ch = infix[i];
        
        // case operand:
        if (isOperand(ch)) {
            postfix += ch;
        }
        
        // case '(':
        else if (ch == '(') {
            operatorStack.push('(');
        }
        
        // case ')':
        else if (ch == ')') {
            // pop stack until matching '('
            while (operatorStack.top() != '(') {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        
        // case operator:
        else if (isOperator(ch)) {
            while (!operatorStack.empty() && operatorStack.top() != '('
                   && (precedence(ch) <= precedence(operatorStack.top()))) {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(ch);
        } else {
            // We've encountered an invalid character of some sorts
            return "invalid";
        }
    }
    
    // append rest of stack to postfix
    while (!operatorStack.empty()) {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    
    return postfix;
}

////////////////////////////////
// main()
////////////////////////////////
int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
                            pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
      // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
                            pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                            pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                            pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate("(a+i)-o / u", m, pf, answer) == 0 &&
                            pf == "ai+ou/-" && answer == 9);
    
    answer = 999;
    assert(evaluate("o / u    -u", m, pf, answer) == 0 &&
                            pf == "ou/u-" && answer == -4);
    assert(evaluate("o / (u-u)", m, pf, answer) == 3 &&
                            pf == "ouu-/" && answer == -4);
    
    answer = 999;
    assert(evaluate("(((((((((((o+u))*i))+a)))))))+e)", m, pf, answer) == 1 && answer == 999);
    assert(evaluate("((((((((((((o+u))*i))+a)))))))+e)", m, pf, answer) == 0 &&
           pf == "ou+i*a+e+" && answer == 30);
    cout << "Passed all tests" << endl;
}
