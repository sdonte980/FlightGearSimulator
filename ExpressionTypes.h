#ifndef PROJECT_EX1_H
#define PROJECT_EX1_H

#include "Expression.h"
#include <queue>
#include <string>
#include <stack>

using namespace std;


class BinaryOperator: public Expression {

protected:
    Expression *right, *left;

public:
    BinaryOperator(Expression *right, Expression *left);
    virtual double calculate() = 0;
};

class UnaryOperator: public Expression {

protected:
    Expression *exp;

public:
    UnaryOperator(Expression *exp);
    virtual double calculate() = 0;
};

class Plus: public BinaryOperator {
public:
    Plus(Expression* exp1, Expression* exp2);
    double calculate();
    ~Plus();

};

class Minus: public BinaryOperator {
public:
    Minus(Expression* exp1, Expression* exp2);
    double calculate();
    ~Minus();
};

class Mul: public BinaryOperator {
public:
    Mul(Expression* exp1, Expression* exp2);
    double calculate();
    ~Mul();
};

class Div: public BinaryOperator {
public:
    Div(Expression* exp1, Expression* exp2);
    double calculate();
    ~Div();
};

class Value: public Expression {
private:
    const double val;

public:
    Value(double value) : val(value) {};
    double calculate();
};


class ExpressionInterpreter {
public:
    Expression* interpret(string tokens);
};


#endif