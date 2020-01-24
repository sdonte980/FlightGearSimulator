#ifndef SIMULATOR_CONDITION_COMMAND_TYPES_H
#define SIMULATOR_CONDITION_COMMAND_TYPES_H


#include "Command.h"
#include "ConditionParser.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;


class IfCommand : public ConditionParser {
    map<string, Command*> commands;
public:
    IfCommand() {}
    int execute(int position);
    ~IfCommand();

};

class LoopCommand : public ConditionParser {
public:
    LoopCommand() {}
    int execute(int position);
    ~LoopCommand();

private:
    bool createCondition(string var1, string operatorStr,string var2);
};

#endif