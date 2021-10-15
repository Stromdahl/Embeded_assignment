// basic file operations
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

#define NUM_SENSORS 236
#define FILE_PATH "sensor_values.txt"

float getRandom(){
    return (float) rand()/RAND_MAX;
}

void write_to_file(const char* path, std::string values){
    std::ofstream file;
    file.open(path);
    file << values;
    file.close();
}

std::string toString(const bool sensorValues[]){
    std::string result;
    for (int i = 0; i < 236; i++){
        result += std::to_string(i+1) + ";";
        result += (sensorValues[i]) ? "True;": "False;";
    }
    return result;
}


void changeValues(bool sensorValues[], int numberOfChanges){
    for(int i = 0; i < numberOfChanges; i++){
        int sensorToBeChanged =  getRandom() * 236;
        sensorValues[sensorToBeChanged] = !sensorValues[sensorToBeChanged];

    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main () {
    bool sensorValues[236];

    for(bool & sensorValue : sensorValues){
        sensorValue = getRandom() > .5;
    }

    while(true){
        std::string values = toString(sensorValues);
        write_to_file(FILE_PATH, values);
        changeValues(sensorValues, 5);
        usleep(1000000);
    }
    return 0;
}

#pragma clang diagnostic pop