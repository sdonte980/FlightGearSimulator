#include <mutex>
#include "Interpreter.h"
#include <queue>
bool running;
map<string, Variable>inputVals;
map<string, Variable>outputVals;
vector<string> tokens;
map<string,Command*> commands;
queue<string> pushCommandToServer;
mutex mutexLock;

int main() {
    running=true;
    Interpreter i = Interpreter("file_name.txt");
    return 0;
}
