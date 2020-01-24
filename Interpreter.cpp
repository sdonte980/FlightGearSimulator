#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Interpreter.h"
#include "Command.h"
#include "CommandTypes.h"
#include "FuncCommand.h"
#include "ConditionCommandTypes.h"
#include <map>


using namespace std;


Interpreter::Interpreter(string fileName) {
    this->fileName = fileName;
    tokens= lexer();
    mapCommands();
    parser();
}

//This function gets a char and returns if it's in the operator vector.
bool Interpreter::inVec(char c) {
    for (char i : this->operatorVec) {
        if (c == i) {
            return true;
        }
    }
    return false;
}

//This function reads from "file_name" file and split it to vector of string tokens.
vector<string> Interpreter::lexer() {
    vector<string> tokens;
    vector<string> lines = this->getLinesFromFile();
    for (int k = 0; k < lines.size(); k++) {
        string newWord;
        int i, j,size = strlen(lines[k].c_str());
        for (i = 0; i < size; i++) {
            if (lines[k][i] == '\t') {
                continue;
            }

            if (lines[k][i] == '"') {
                newWord += lines[k][i];
                j = i + 1;
                while (lines[k][j] != '"') {
                    newWord += lines[k][j];
                    j++;
                }
                newWord += lines[k][j];
                i = j;
            } else if (lines[k][i] == '-' && lines[k][i + 1] == '>') {

                if (newWord.length() > 0) {
                    tokens.push_back(newWord);
                    newWord = "";
                }

                newWord += lines[k][i];
                newWord += lines[k][i + 1];
                tokens.push_back(newWord);
                newWord = "";
                i += 1;
            } else if (inVec(lines[k][i]) == 1) {

                if (newWord.length() > 0) {
                    tokens.push_back(newWord);
                    newWord = "";
                }
                if (inVec(lines[k][i + 1]) == 1) {
                    newWord += lines[k][i];
                    newWord += lines[k][i + 1];
                    tokens.push_back(newWord);
                    newWord = "";
                    j = i + 2;
                    // creates the <-
                } else if (lines[k][i + 1] == '-') {
                    newWord += lines[k][i];
                    newWord += lines[k][i + 1];
                    tokens.push_back(newWord);
                    newWord = "";
                    i += 1;
                    continue;
                } else {

                    newWord += lines[k][i];
                    tokens.push_back(newWord);
                    newWord = "";
                    j = i + 1;
                }

                for (j; lines[k][j] != '{' && j < size; j++) {
                    if (lines[k][j] != ' ') {
                        newWord += lines[k][j];
                    }
                }
                tokens.push_back(newWord);
                newWord = "";

                if (lines[k][j] == '{') {
                    newWord += lines[k][j];
                    tokens.push_back(newWord);
                    newWord = "";
                    j++;
                }
                i = j;
            } else if ((lines[k][i] != '(') && (lines[k][i] != ')')
                       && (lines[k][i] != ',') && (lines[k][i] != ' ')) {
                newWord += lines[k][i];
            } else {
                if(newWord.length() > 0) {
                    tokens.push_back(newWord);
                    newWord = "";
                }
            }
        }
        if (newWord.empty() == 0) {
            tokens.push_back(newWord);
        }
    }
    return tokens;
}

//This function gets a string and returns true if it's in the symbolTable map.
bool Interpreter::isInSymbolTable(string symbol){
    return (symbolTable.find(symbol) != symbolTable.end()) ;
}

//This function parse the tokens and execute the commands by order.
void Interpreter::parser() {
    int index = 0;
    while (index < tokens.size()) {
        string currentToken = tokens[index];
        if (isInSymbolTable(currentToken)) {
            currentToken = tokens[index+1];
            index++;
        }
        map<string, Command *>::iterator itr = commands.find(currentToken);
        if (itr != commands.end()) {
            Command *c = itr->second;
            vector<string> parameters = getParameters(index);
            int skip =  c->execute(index);
            index += skip;
        }
        index++;
    }
}

Interpreter::~Interpreter() {

}

//This function reads the file and turn it to vector of strings
vector<string> Interpreter::getLinesFromFile() {
    vector<string> lines;
    ifstream file;
    string line;
    file.open(fileName, ifstream::in);
    if (!file.is_open()) {
        cout << "Cannot open file!" << endl;
        exit(1);
    } else {
        while (getline(file, line)) {
            if (line.length() != 0) {
                lines.push_back(line);
            }
        }
        file.close();
    }
    return lines;
}

//This function get's position of token and returns vector of strings
//as the parameters for the execution.
vector<string> Interpreter::getParameters(int position) {
    map<string, Command *>::iterator itr;
    map<string, Command *> commandsToScope;
    string command = tokens[position];
    vector<string> parameters;
    bool scope = false;
    position++;
    if (command.compare("var")==0&&tokens[position+1].compare("=")==0){
        parameters={tokens[position],tokens[position+1],tokens[position+2]};
        return parameters;
    }
    if (command.compare("while")==0 ||(command.compare("takeoff")==0&&tokens[position].compare("var")==0)){
        int i = position;
        int scope =1;
        while (tokens[i].compare("{")!=0){
            parameters.push_back(tokens[i]);
            i++;
        }
        parameters.push_back(tokens[i]);
        i++;
        while (scope !=0){
            if (tokens[i].compare("{")==0){
                scope++;
            } else if (tokens[i].compare("}")==0){
                scope--;
            }
            parameters.push_back(tokens[i]);
            i++;
        }
        return parameters;
    }
    if (command.compare("takeoff")==0){
        parameters={tokens[position]};
        return parameters;
    }
    if (command.compare("Print")==0){
        return {tokens[position]};
    }
    if (command.compare("=")==0){
        return {tokens[position-2],tokens[position]};
    }
    if (command.compare("Sleep")==0){
        return {tokens[position]};
    }
    while (position < tokens.size()) {
        itr = commands.find(tokens[position]);

        if (tokens[position].compare("{") == 0) {
            scope = true;
            continue;
        }
        if (scope && itr != commands.end()) {
            commandsToScope.insert({tokens[position], itr->second});
            commands.erase(itr);
        }
        if ((itr != commands.end() && !scope) || (scope && tokens[position].compare("}"))) {
            break;
        } else {
            parameters.push_back(tokens[position]);
        }
        position++;
    }
    return parameters;
}

//This function init the map of commands.
void Interpreter::mapCommands() {
    commands["openDataServer"]= new OpenServerCommand;
    commands["connectControlClient"]= new ConnectCommand;
    commands["var"]= new DefineVarCommand(&symbolTable);
    commands["while"]= new LoopCommand;
    commands["="]= new UpdateVarCommand(&symbolTable);;
    commands["Sleep"]= new SleepCommand;
    commands["Print"]= new PrintCommand;
    commands["if"]= new IfCommand;
    commands["takeoff"]= new FuncCommand;
}