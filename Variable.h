
#ifndef SIMULATOR_VARIABLE_H
#define SIMULATOR_VARIABLE_H
#include "Expression.h"
#include <string>
using namespace std;

class Variable : public Expression {
    double value;
    string sim;

public:
    Variable() {}
    void setSim(string sim);
    void setValue(double value);
    const string getSim();
    double getValue();
    double calculate();
    Variable& operator++();
    Variable& operator--();
    Variable& operator+=(double val);
    Variable& operator-=(double val);
    Variable& operator++(int num);
    Variable& operator--(int num);
    ~Variable(){}
};
#endif //SIMULATOR_VARIABLE_H