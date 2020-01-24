#ifndef EXPRESSIONS_VALUE_H
#define EXPRESSIONS_VALUE_H

#include "Expression.h"

using namespace std;

class Value: public Expression {
private:
    const double val;

public:
    Value(double value) ;
    double calculate();
};
#endif //EXPRESSIONS_VALUE_H
