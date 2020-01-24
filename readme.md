# Course Advanced-Programming 1 - 89210 : FlightSimulator (FlightGear)
**Introduction:**

As part of the course we were required to create an Interpreter that allows remote control flight simulator.  
This project contains the following elements:
- Use of design and architecture templates.
- Communication
And client-server architecture.
- Use of data structures and database.
- Streaming data (files and communications).
- Comparison, selection and implementation of algorithms within the system we create.
- Parallel programming using Threads.
- Event-oriented programming, desktop application with GUI.
- Event-oriented programming, eb application REST style.
- Mobile Application (Android).

## Installation + Configuration (linux users):

Download the application from official website [FlightGear](https://www.flightgear.org/download/) and install.

Take from this repository the ```generic_small.xml``` file and put in on Desktop.
We need to insert this file to **flightgear/Protocol** folder for do it
Open the terminal(linux) and insert this:
```linux-terminal
sudo cp generic_small.xml ../../../usr/share/games/flightgear/Protocol/
```
Open the  **FlightGear Simulator** and select **Settings** in main screen
and inside **Additional Settings** command-line insert:
```c++
--telnet=socket,in,10,127.0.0.1,5402,tcp--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small
```
Now open terminal(linux) and copy this data for compile the program:
```
g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```
**Make sure your input txt file at the same folder with your .cpp project files**
```./a.out file_name.txt```
Run cpp project that found in this repository
And Press "Fly!" on FlightGear simulator.
## Authors:
**Keren Shina**

**Alex Weiser**

## Link
https://github.com/sdonte980/FlightGearSimulator
https://github.com/kerenshina/flight_simulator

