#ifndef SIMULATOR_FUNC_COMMAND_H
#define SIMULATOR_FUNC_COMMAND_H

#include <string>
#include <vector>
#include <map>
#include "Command.h"

using namespace std;

class FuncCommand : public Command {
    map<string, Command*> commands;
public:
    FuncCommand() {}
    void getCommands(map<string, Command*> commands);
    int execute(int position);
    ~FuncCommand();
};

#endif //SIMULATOR_FUNC_COMMAND_H