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

std::string generate_string_values(int nVal){
    std::string result;
    for (int i = 0; i < nVal; i++){
        result += std::to_string(i+1) + ";";
        result += (getRandom() > .5) ? "True;": "False;";
    }
    return result;
}

std::string generate_test_values(int nVal){
    std::string result;
    for (int i = 0; i < nVal; i++){
        int x = i / 8;

        result += std::to_string(i+1) + ";";
        result += (x % 2 == 0) ? "True;": "False;";
    }
    return result;
}

int main () {
    while(true){
        std::string values = generate_string_values(NUM_SENSORS);
        write_to_file(FILE_PATH, values);
        break;
        usleep(1000000);
    }
    return 0;
}