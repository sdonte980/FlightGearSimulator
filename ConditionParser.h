#ifndef SIMULATOR_CONDITION_PARSER_H
#define SIMULATOR_CONDITION_PARSER_H

#include <string>
#include <vector>
#include "Command.h"

using namespace std;

class ConditionParser : public Command {

public:
    virtual int execute(int position) = 0;
};

#endif //SIMULATOR_CONDITION_PARSER_H
