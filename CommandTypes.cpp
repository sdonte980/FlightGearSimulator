#include "CommandTypes.h"
#include "Command.h"
#include "Interpreter.h"
#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include "Expression.h"
#include "ExpressionTypes.h"
#include <sstream>
#include <arpa/inet.h>

using namespace std;

//This queue responsible for the commands sent to the simulation.
extern queue<string> pushCommandToServer;

//This function sets the values of data from simulation to outPutVals Map.
void updateOutputVals(vector <double> data){
    outputVals["airspeed"].setValue(data[0]);
    outputVals["alt"].setValue(data[4]);
    outputVals["heading"].setValue(data[3]);
    outputVals["pitch"].setValue(data[8]);
    outputVals["roll"].setValue(data[7]);
    outputVals["rpm"].setValue(data[35]);
}

OpenServerCommand::OpenServerCommand() {
}

int OpenServerCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    if (parameters.size() != 1) {
        cout << "wrong parameters for openServer Command" << endl;
        return 0;
    }
    int sourcePort = atoi(parameters[0].c_str());
    createServer(sourcePort);
    return parameters.size() ;
}

void getInfo(int client_socket){
    char buffer[1024] = {0};
    vector <double> info;
    //The loop of reading data from the simulation.
    while(running){
        mutexLock.lock();
        read(client_socket,buffer,1024);
        string buff=buffer;
        size_t pos=0;
        string delimiter = ",";
        int i=0;
        while ((pos = buff.find(delimiter)) != string::npos) {
            string subString = buff.substr(0,pos);
            info.push_back(stod(subString));
            buff.erase(0, pos + delimiter.length());
        }
        info.push_back(stod(buff.substr(0, pos)));
        memset(buffer,0,1024);
        updateOutputVals(info);
        mutexLock.unlock();
        info.clear();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

//create server
void OpenServerCommand::createServer(int sourcePort) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "failed creating the socket" << endl;
        return;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(sourcePort);

    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cerr << "Couldn't bind the socket to an IP" << endl;
        return;
    }
    // listen
    if (listen(socketfd, 5) == -1) {
        cerr << "Err during listening command" << endl;
        return;
    }
    cout << "Server is now listening..." << endl;
    // accept
    int client_socket = accept(socketfd, (struct sockaddr *) &address, (socklen_t *) &address);
    if (client_socket == -1) {
        cerr << "Error trying to accept the client" << std::endl;
        return;
    }
    cout << "Server is Connected" << endl;

    //Thead for getting information from simulation.
    thread t1(getInfo,client_socket);
    t1.detach();
}

OpenServerCommand::~OpenServerCommand() {

}


ConnectCommand::ConnectCommand(){

}

void runServer(int client_socket){
    //This is the loop for sending commands to the simulation.
    while (true) {
        while (!pushCommandToServer.empty()) {
            mutexLock.lock();
            string command = pushCommandToServer.front() + "\r\n";
            pushCommandToServer.pop();
            char sendCommend[command.size() + 1];
            strcpy(sendCommend, command.c_str());
            int is_sent = send(client_socket, sendCommend, strlen(sendCommend), 0);
            if (is_sent == -1) {
                cout << "Error sending message" << endl;
            }
            mutexLock.unlock();
        }
    }
}

int ConnectCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cerr << "Could not create a socket" << endl;
        exit(1);
    }
    string tmp = parameters[0].substr(1,parameters[0].length()-2);
    char localHostAddress[tmp.length() + 1];
    strcpy(localHostAddress, tmp.c_str());
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(localHostAddress);
    address.sin_port = htons(stoi(parameters[1]));
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        cerr << "Could't connect to host server" << endl;
        exit(1);
    } else {
        cout << "Client is now connected to server" << endl;
    }
    //This is the thread repsonsible for sending commands to simulation
    thread runServerThread(runServer,client_socket);
    runServerThread.detach();
    return 2;
}

ConnectCommand::~ConnectCommand() {

}


DefineVarCommand::DefineVarCommand(map<string, Variable*>* symbolTable) {
    this->symbolTable = symbolTable;
}

//This function resposible for filling the outputvals and inputvals map.
int DefineVarCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    Variable newVar = Variable();
    if (parameters[1].compare("=") == 0) {
        double val = outputVals[parameters[2]].getValue();
        newVar.setValue(val);
        inputVals[parameters[0]] = newVar;
        return parameters.size();
    }
    string sim = parameters[3].substr(1, parameters[3].length() - 2);
    newVar.setSim(sim);
    newVar.setValue(0);
    if (parameters[1].compare("->")) {
        outputVals.emplace(parameters[0], newVar);
    } else if (parameters[1].compare("<-")) {
        inputVals.emplace(parameters[0], newVar);
    }
    return parameters.size();
}

DefineVarCommand::~DefineVarCommand() {

}


UpdateVarCommand::UpdateVarCommand(map<string, Variable*>* symbolTable) {
    this->symbolTable = symbolTable;
}

//This function push string commands to queue to be sent to the simulator
int UpdateVarCommand::execute(int position) {
   vector<string> parameters=Interpreter::getParameters(position);
    try {
   inputVals[parameters[0]].setValue(stod(parameters[1]));
    }
    catch (invalid_argument){
        ExpressionInterpreter ei;
       Expression *expression = ei.interpret(parameters[1]);
       double expValue = expression->calculate();
        inputVals[parameters[0]].setValue(expValue);
    }
    string commad = "set " + inputVals[parameters[0]].getSim() + " " +to_string(inputVals[parameters[0]].getValue()) + "\r\n";
    pushCommandToServer.push(commad);
    return parameters.size() -1 ;
}

UpdateVarCommand::~UpdateVarCommand() {

}


SleepCommand::SleepCommand() {

}

//This function makes the thread sleep certain time.
int SleepCommand::execute(int position) {
    vector<string> parameters=Interpreter::getParameters(position);
    this_thread::sleep_for(chrono::milliseconds(stoi(parameters[0])));
    return parameters.size();
}

SleepCommand::~SleepCommand() {

}


PrintCommand::PrintCommand() {

}

//This function prints vars or numbers or text
int PrintCommand::execute(int position) {
    vector<string> parameters = Interpreter::getParameters(position);
    string toPrint = parameters[0];
    double value = -999;
    if (Command::isInOutMap(toPrint)){
        value = outputVals[toPrint].getValue();
    } else if (Command::isInInputMap(toPrint)){
        value = inputVals[toPrint].getValue();
    }
    if (value == -999){
        if (toPrint[0]=='"'){
            toPrint = toPrint.substr(1,toPrint.length()-2);
        }
        cout<<toPrint<<endl;
    }
   else
       cout<<value<<endl;
    if (toPrint.compare("done")==0){
        running=false;
    }
    return 1;
}
PrintCommand::~PrintCommand() {
}