#include "Variable.h"


Variable& Variable::operator++() {
    this->value = ++(this->value);
    return *this;
}

Variable& Variable::operator--() {
    this->value = --(this->value);
    return *this;
}

Variable& Variable::operator+=(double val) {
    this->value = this->value + val;
    return *this;
}

Variable& Variable::operator-=(double val) {
    this->value = this->value - val;
    return *this;
}

Variable& Variable::operator++(int num) {
    return ++*this;
}
Variable& Variable::operator--(int num) {
    return --*this;
}
double Variable::calculate() {
    return this->value;
}
void Variable::setValue(double value){
    this->value =value;
}

const string Variable::getSim() {
    return this->sim;
}
double Variable::getValue(){
    return this->value;
}
void Variable::setSim(string sim){
    this->sim=sim;
}