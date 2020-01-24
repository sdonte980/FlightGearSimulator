#ifndef SIMULATOR_INTERPRETER_H
#define SIMULATOR_INTERPRETER_H

#include <string>
#include <map>
#include "Command.h"
#include "Variable.h"

using namespace std;
extern map<string, Variable>inputVals;
extern map<string, Variable>outputVals;
extern vector<string> tokens;
extern map<string,Command*> commands;

class Interpreter {
    string fileName;
    map<string, Variable*> symbolTable;
public:
    Interpreter(string fileName);
    bool isInSymbolTable(string symbol);
    void parser();
    static vector<string> getParameters(int position);
    ~Interpreter();

private:
    vector<char> operatorVec = {'<', '>', '!', '='};
    vector<string> lexer();
    vector<string> getLinesFromFile();
    void mapCommands();
    bool inVec(char c);
};

#endif //SIMULATOR_INTERPRETER_H