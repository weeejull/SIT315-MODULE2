// NAME = VIJUL
// ROLL NUMBER = 2210994860
// MODULE 2
// TASK M2T3D - TRAFFIC CONTROL SIMULATOR
// PROGRAM CODE - CONSUMER

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

#define NUM_LIGHTS 12
#define MEASUREMENTS 13

// FUNCTION TO CALCULATE TRAFFIC VOLUME BASED ON INPUT ARRAY
void calculateTrafficVolume(int array[NUM_LIGHTS][MEASUREMENTS]) {
    cout << "CALCULATING...\t";
    for (int i = 0; i < NUM_LIGHTS; i++) {
        array[i][0] = 0;
        for (int j = 1; j < MEASUREMENTS; j++) {
            array[i][0] += array[i][j];
        }
        array[i][0] = array[i][0] / (MEASUREMENTS - 1);  // INTEGER DIVISION FOR SIMPLICITY
    }
    cout << "DONEE" << endl;
}

int main() {
    int Array[NUM_LIGHTS][MEASUREMENTS];
    int arraySize = sizeof(Array[0]) / sizeof(Array[0][0]);  // CORRECT ARRAY SIZE CALCULATION
    long rawTime;

    int lightNum;
    int trafficVolume;

    fstream log("log.txt", ios::in);
    if (!log.is_open()) {
        cout << "ERROR!!! : LOG FILE NOT FOUND..." << endl;  // DISPLAY ERROR IF FILE OPENING FAILS
    } else {
        cout << "RETRIEVING INITIAL DATA FROM log.txt...\t";
        for (int i = 0; i < NUM_LIGHTS; i++) {
            for (int j = 1; j < arraySize; j++) {
                Array[i][j] = 0;
            }
        }

        for (int k = 0; k < 12; k++) {
            for (int i = 0; i < NUM_LIGHTS; i++) {
                log >> rawTime >> lightNum >> trafficVolume;

                for (int j = arraySize - 1; j > 0; j--) {
                    Array[lightNum - 1][j] = Array[lightNum - 1][j - 1];
                }
                Array[lightNum - 1][1] = trafficVolume;
            }
        }
        log.close();  // CLOSE THE FILE AFTER READING
        cout << "COMPLETE" << endl;  // DISPLAY SUCCESS MESSAGE
    }

    calculateTrafficVolume(Array);  // CALL FUNCTION TO CALCULATE TRAFFIC VOLUME

    return 0;  // RETURN 0 TO INDICATE SUCCESSFUL EXECUTION
}
