#include "Command.h"
#include "Interpreter.h"
#include "ConditionCommandTypes.h"
#include <string>
#include <map>
#include <vector>

extern map<string, Command *> commands;

int IfCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    return 0;
}

IfCommand::~IfCommand() {

}

//This function execute all the commands inside the while
int LoopCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
     int loopLines = 0, i;
     bool inScope = false;
     for (int i = 0; i < parameters.size() - 1; i++) {
         if (inScope) {
             loopLines++;
         }
         if (parameters[i].compare("{") == 0) {
             inScope = true;
         }
     }
     string var1, var2, operatorStr;
     var1 = parameters[0];
     operatorStr = parameters[1];
     var2 = parameters[2];
     while (createCondition(var1, operatorStr, var2)) {
         while (i < loopLines) {
             string currentToken = tokens[position + i + 5];
             map<string, Command *>::iterator itr = commands.find(currentToken);
             if (itr != commands.end()) {
                 Command *c = itr->second;
                 int skip = c->execute(position + i + 5);
                 i += skip;
             }
             i++;
         }
         i = 0;
     }
    return parameters.size();
}

LoopCommand::~LoopCommand() {

}

//This function gets a condition in string and returns the truth or false value
bool LoopCommand::createCondition(string var1, string operatorStr, string var2) {
    double value1, value2;
    if (Command::isInInputMap(var1)) {
        value1 = inputVals[var1].getValue();
    } else if (Command::isInOutMap(var1)) {
        value1 = outputVals[var1].getValue();
    } else {
        value1 = stod(var1);
    }
    if (Command::isInInputMap(var2)) {
        value2 = inputVals[var2].getValue();
    } else if (Command::isInOutMap(var2)) {
        value2 = outputVals[var2].getValue();
    } else {
        value2 = stod(var2);
    }
    if (operatorStr.compare("==") == 0) {
        return value1 == value2;
    }
    if (operatorStr.compare("<=") == 0) {
        return value1 <= value2;
    }
    if (operatorStr.compare(">=") == 0) {
        return value1 >= value2;
    }
    if (operatorStr.compare(">") == 0) {
        return value1 > value2;
    }
    if (operatorStr.compare("<") == 0) {
        return value1 < value2;
    }
    return false;
}