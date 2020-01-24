#ifndef SIMULATOR_COMMAND_TYPES_H
#define SIMULATOR_COMMAND_TYPES_H


#include "Command.h"
#include <string>
#include <vector>
#include <map>
#include "Variable.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

extern queue<string>clientCommands;
extern map<string, Variable>inputVals;
extern map<string, Variable>outputVals;
extern bool running;
extern mutex mutexLock;

class OpenServerCommand : public Command {
    void createServer(int sourcePort);
public:
    OpenServerCommand();
    int execute(int position);
    ~OpenServerCommand();
};

class ConnectCommand : public Command  {
public:
    ConnectCommand();
    int execute(int position);
    ~ConnectCommand();
};

class DefineVarCommand : public Command {
    map<string, Variable*>* symbolTable;
public:
    DefineVarCommand(map<string, Variable*>* symbolTable);
    int execute(int position);
    ~DefineVarCommand();
};

class UpdateVarCommand : public Command {
    map<string, Variable*>* symbolTable;
public:
    UpdateVarCommand(map<string, Variable*>* symbolTable);
    int execute(int position);
    ~UpdateVarCommand();
};

class SleepCommand : public Command  {
public:
    SleepCommand();
    int execute(int position);
    ~SleepCommand();
};

class PrintCommand : public Command  {
public:

    PrintCommand();
    int execute(int position);
    ~PrintCommand();
};
#endif //SIMULATOR_COMMAND_TYPES_H