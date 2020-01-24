cmake_minimum_required(VERSION 3.15)
project(PG3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(PG3 main.cpp Command.h CommandTypes.cpp CommandTypes.h ConditionCommandTypes.cpp ConditionCommandTypes.h ConditionParser.h Expression.h ExpressionTypes.cpp ExpressionTypes.h FuncCommand.cpp FuncCommand.h Interpreter.cpp Interpreter.h Value.cpp Value.h Variable.cpp Variable.h Command.cpp)