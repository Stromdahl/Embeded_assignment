#include <iostream>
#include <fstream>
#include <bitset>


class SensorString{
public:
    SensorString(std::string* _sensorString): sensorString(_sensorString){}

    void getNextSensor(int* outSensorNr, bool* outState){
        //Returns the value of the next sensor
        std::string *sensorNrString = getNext();
        std::string *sensorStateString = getNext();

        if(sensorNrString == nullptr || sensorStateString == nullptr){
            outSensorNr = nullptr;
            outState = nullptr;
            return;
        }
        *outSensorNr = std::stoi(*sensorNrString);
        *outState = (*sensorStateString == "True") ? true:false;
    }

private:

    std::string* getNext(){
        //Returns a pointer to substring from beginning of the string to the delimiter
        std::string* result;
        std::string delimiter = ";";
        size_t pos = sensorString->find(delimiter);

        if(pos == std::string::npos)
            return nullptr;

        *result = sensorString->substr(0, pos);
        sensorString->erase(0, pos + delimiter.length());
        return result;
    }

    std::string* sensorString;
};

class Sensors{
public:

    Sensors(){
        values = new uint8_t[30];
    }

    ~Sensors(){
        delete values;
    }

    void getValuesFromString(std::string* valuesString){
        SensorString sensorString(valuesString);

        int *sensorNr = nullptr;
        bool* sensorState = nullptr;

        while (true) {
            sensorString.getNextSensor(sensorNr, sensorState);

            if(sensorNr == nullptr || sensorState == nullptr)
                return;

            setSensorState(*sensorNr, *sensorState);
        }
    }

    void setSensorState(int index, bool state){
        int byteIndex = index - 1 / 8;
        int bitIndex = index - 1 % 8;

        if (state)
            values[byteIndex] |= 1UL << bitIndex;
        else
            values[byteIndex] &= ~(1UL << bitIndex);
    }

private:
    uint8_t *values;
public:
    uint8_t *getValues() const {
        return values;
    }
};

std::string read_from_file(const char* path){
    std::ifstream file(path);
    std::string line;
    if (file.is_open()) {
        std::getline(file,line);
        file.close();
    }
    return line;
}

std::string bin_str8(unsigned char n) {
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

int main(){
    std::cout << "Start" << std::endl;

    std::string content;
    content = read_from_file("sensor_values.txt");

    Sensors sensors;

    sensors.getValuesFromString(&content);


    // uint8_t* values = new uint8_t[30];

    // valueStringToBinary(values, content);

    // // for(int i = 0; i < 236; i++){
    // //     bool state = (i % 8 == 0);
    // //     setSensorState(values, i, state);
    // // }

    //printSensors(sensors.getValues());

    std::cout << "Hello world" << std::endl;
    return 0;
}