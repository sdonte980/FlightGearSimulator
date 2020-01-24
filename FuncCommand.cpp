#include "Command.h"
#include "Interpreter.h"
#include "FuncCommand.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>


int FuncCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    return parameters.size();
}

void FuncCommand::getCommands(map<string, Command*> commands){
    this->commands = commands;
}

FuncCommand::~FuncCommand() {

}