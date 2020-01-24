#include <string>
#include <string.h>
#include <vector>
#include "CommandTypes.h"
#include "ExpressionTypes.h"
#include "Expression.h"
#include "Variable.h"

extern map<string, Variable>inputVals;
extern map<string, Variable>outputVals;

using namespace std;

bool isOperator(char c);
Expression* makeBinaryExp(string s, Expression* e1, Expression* e2);

BinaryOperator::BinaryOperator(Expression *right, Expression *left) {
    this->left = left;
    this->right = right;
}


Plus::Plus(Expression* exp1, Expression* exp2) : BinaryOperator(right, left) {
    this->right = exp1;
    this->left = exp2;
}
double Plus::calculate() {
    return right->calculate() + left->calculate();
}
Plus::~Plus() {
    if (right != NULL)
    {
        delete(this->right);
    }

    if (left != NULL)
    {
        delete(this->left);
    }
}

Minus::Minus(Expression* exp1, Expression* exp2) : BinaryOperator(right, left) {
    this->right = exp1;
    this->left = exp2;
}

double Minus::calculate() {
    return right->calculate() - left->calculate();
}

Minus::~Minus() {
    if (right != NULL)
    {
        delete(this->right);
    }

    if (left != NULL)
    {
        delete(this->left);
    }
}

Mul::Mul(Expression* exp1, Expression* exp2) : BinaryOperator(right, left) {
    this->right = exp1;
    this->left = exp2;
}

double Mul::calculate() {
    return right->calculate() * left->calculate();
}

Mul::~Mul() {
    if (right != NULL)
    {
        delete(this->right);
    }

    if (left != NULL)
    {
        delete(this->left);
    }
}

Div::Div(Expression* exp1, Expression* exp2) : BinaryOperator(right, left) {
    this->right = exp1;
    this->left = exp2;
}

double Div::calculate() {
    if (left->calculate() == 0)
    {
        throw "cannot divide by 0";
    } else {
        return right->calculate() / left->calculate();
    }
}

Div::~Div() {
    if (right != NULL)
    {
        delete(this->right);
    }

    if (left != NULL)
    {
        delete(this->left);
    }
}
bool isOperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/')
    {
        return true;
    } else {
        return false;
    }
}
bool isNumber(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i]<'0'||str[i]>'9') return false;
    }
    if (str.length()==0)return false;
    return true;
}

Expression* ExpressionInterpreter:: interpret(string str){
    if (str.compare("")==0) return NULL;
    string left="",right="";
    int side = 0, bracets = 0;
    string oper;
    for (int i=0;i<str.length();i++){
        if (str[i]=='(') bracets++;
        else if (str[i]==')') bracets--;
        if (isOperator(str[i])&&bracets==0&&left.length()>0){
            oper = str[i];
            i++;
            side=1;
        }
        if (side==0){
            left+=str[i];
        }
        else {
            right += str[i];
        }
    }
    if (left[0]=='(') left = left.substr(1,left.length()-2);
    if (right[0]=='(') right = right.substr(1,right.length()-2);
    Expression* lExp;
    Expression* rExp;

    if (isNumber(left)){
        lExp = new Value(stod(left));
    }else{
        bool negetive = false;
        double val;
        if (left[0]=='-'){
            left = left.substr(1,left.length()-1);
            negetive=true;
        }
        if (Command::isInInputMap(left)){
             val = inputVals[left].getValue();
             if (negetive) val*=-1;
            lExp = new Value(val);
        } else if (Command::isInOutMap(left)){
            val = outputVals[left].getValue();
            if (negetive) val*=-1;
            lExp = new Value(val);
        }
        else  {
            lExp = interpret(left);
        }
    }
    if (isNumber(right)){
         rExp = new Value(stod(right));
    }else{
        bool negetive = false;
        double val;
        if (left[0]=='-'){
            left = left.substr(1,left.length()-1);
            negetive=true;
        }
        if (Command::isInInputMap(right)){
            val = inputVals[right].getValue();
            if (negetive) val*=-1;
            rExp = new Value(val);
        } else if (Command::isInOutMap(right)){
            val = outputVals[right].getValue();
            if (negetive) val*=-1;
            rExp = new Value(val);
        } else {
            lExp = interpret(right);
        }
    }
    return makeBinaryExp(oper,lExp,rExp);
}

Expression* makeBinaryExp(string s, Expression* e1, Expression* e2) {
    if (s.compare("+") == 0)
    {
        return new Plus(e1, e2);
    } else if (s.compare("-") == 0)
    {
        return new Minus(e1, e2);
    } else if (s.compare("*") == 0)
    {
        return new Mul(e1, e2);
    } else
    {
        return new Div(e1, e2);
    }
}