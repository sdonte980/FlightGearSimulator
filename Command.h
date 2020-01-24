#ifndef SIMULATOR_COMMAND_H
#define SIMULATOR_COMMAND_H
#include <map>
#include <string>
#include <vector>
#include "Variable.h"

using namespace std;

extern map<string, Variable>inputVals;
extern map<string, Variable>outputVals;

/**
 * Command Interface
 */
class Command {

public:
    Command() {}
    virtual int execute(int position) = 0;
    virtual ~Command() {}
    static bool isInInputMap(string str);
    static bool isInOutMap(string str);
};

#endif //SIMULATOR_COMMAND_H