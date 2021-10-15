#include <iostream>
#include <unistd.h>
#include <fstream>
#include <bitset>
#include <chrono>

std::string read_from_file(const char* path){
    std::ifstream file(path);
    std::string line;
    if (file.is_open()) {
        std::getline(file,line);
        file.close();
    }
    return line;
}

void setSensorState(uint8_t* sensorStates, int nr, bool state){
    int byteIndex = nr / 8; //Get witch byte to be accessed
    int bitIndex = nr % 8; //Get witch bit that will be set in the byte

    if (state)
        sensorStates[byteIndex] |= 1UL << bitIndex;
    else
        sensorStates[byteIndex] &= ~(1UL << bitIndex);
}

uint8_t* getValuesFromString(std::string* valuesString){
    auto* sensorStates = new uint8_t[30];

    std::string delimiter = ";";

    size_t pos;
    int nr;
    bool state;
    while (true) {
        //Get sensor nr from string
        if((pos = valuesString->find(delimiter)) == std::string::npos) break;
        nr = stoi(valuesString->substr(0, pos));
        valuesString->erase(0, pos + delimiter.length());

        //Get sensor state from string
        if((pos = valuesString->find(delimiter)) == std::string::npos) break;
        state = valuesString->substr(0, pos) == "True";
        valuesString->erase(0, pos + delimiter.length());

        setSensorState(sensorStates, nr-1, state);
    }
    return sensorStates;
}

std::string bin_str8(uint8_t n) {
    std::bitset<8> bs(n);
    return bs.to_string();
}

void printSensors(uint8_t* sensors){
    for(int i = 0; i<30; i++){
        if (i < 10)
            std::cout << "0";

        std::cout << i << ": " << bin_str8(sensors[i]) << std::endl;
    }
}


void compareSensorValues(const uint8_t* sensorValues1, const uint8_t* sensorValues2){
    std::cout << "Comparing... " << std::endl;
    uint8_t delta;
    for(int nr = 0; nr < 236; nr++){
        int byteIndex = nr / 8; //Get witch byte to be accessed
        int bitIndex = nr % 8; //Get witch bit that will be set in the byte

        uint8_t flag1 = sensorValues1[byteIndex] & (1 << bitIndex);
        uint8_t flag2 = sensorValues2[byteIndex] & (1 << bitIndex);

        if(flag1 != flag2) {
            std::cout << "Sensor nr " << nr + 1 << " changed from "
                      << (flag2 > 0 ? "false to true" : "true to false") << std::endl;
        }
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(){
    std::cout << "Start" << std::endl;

    std::string content;
    content = read_from_file("../sensor_values.txt");
    uint8_t* sensorValues1 = getValuesFromString(&content);

    // usleep(1000000);

    // content = read_from_file("sensor_values.txt");
    // uint8_t* sensorValues2 = getValuesFromString(&content);

    // compareSensorValues(sensorValues1, sensorValues2);

   while (true){
       usleep(1000000);

       std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

       content = read_from_file("../sensor_values.txt");
       uint8_t* sensorValues2 = getValuesFromString(&content);

       compareSensorValues(sensorValues1, sensorValues2);

       uint8_t* tmp = sensorValues1;
       sensorValues1 = sensorValues2;
       delete tmp;
       std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

       std::cout << "Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
       std::cout << "Time = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
   }
//    printSensors(sensorValues1);
//    std::cout << std::endl;
//    printSensors(sensorValues2);
    std::cout << "Hello world" << std::endl;
    return 0;
}
#pragma clang diagnostic pop
