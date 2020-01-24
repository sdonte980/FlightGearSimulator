#include "Command.h"


//This function gets a string and checks if it exist in the InPutMap
 bool Command::isInInputMap(string str) {
    for(map<string, Variable>::const_iterator it = inputVals.begin();
        it != inputVals.end(); ++it)
    {
        if (it->first.compare(str)==0) {return true;}
    }
    return false;
}

//This function gets a string and checks if it exist in the OutPutMap
 bool Command::isInOutMap(string str) {
    for(map<string, Variable>::const_iterator it = outputVals.begin();
        it != outputVals.end(); ++it)
    {
        if (it->first.compare(str)==0) {return true;}
    }
    return false;
}
