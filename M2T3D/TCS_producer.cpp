// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2
// TASK M2T3D
// TRAFFIC CONTROL SIMULATOR
// THIS IS THE PROGRAM CODE - PRODUCER

#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

#define NUM_LIGHTS 12

int main() {
    string output;  // STRING THAT STORES THE LOG ENTRY

    for (int i = 1; i <= NUM_LIGHTS; i++) {
        time_t rawTime;
        time(&rawTime);  // GETS THE CURRENT TIME

        output += to_string(rawTime) + " ";  // APPENDS THE TIMESTAMP TO OUTPUT STRING
        output += to_string(i) + " ";        // APPENDS THE LIGHT NUMBER TO OUTPUT STRING

        int cars = rand() % 10 + 1;  // RANDOM NUMBER FROM 1 TO 10
        output += to_string(cars) + " \n";  // APPENDS THE NUMBER OF CARS TO OUTPUT STRING WITH LINE BREAK
    }
    fstream log("log.txt", ios::app);  // OPENS THE LOG FILE IN APPEND MODE
    if (!log.is_open()) {
        cout << "Error: Log file not found" << endl;  // DISPLAYS ERROR IF FILE OPENING FAILS
    } else {
        log << output;  // WRITES THE OUTPUT STRING TO LOG FILE
        log.close();    // CLOSES THE FILE AFTER WRITING
        cout << output; // DISPLAYS THE GENERATED LOG ENTRY
        cout << "log.txt has been updated." << endl;  // DISPLAYS THE SUCCESS MESSAGE
    }
    return 0;  
}
