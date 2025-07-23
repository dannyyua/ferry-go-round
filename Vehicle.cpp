// Vehicle.cpp
//******************************************************************
// DEFINITION MODULE: Vehicle
//
// PURPOSE: Implements vehicle data operations including:
//          - Validating vehicle license plates
//          - Creating new vehicle records
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/22
// *)
//******************************************************************
#include "Vehicle.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

#pragma pack(push, 1) // Ensure no padding between struct members
struct VehicleRecord {
    char plate[21];            // Fixed-length 20 chars + null terminator
    char otherInfo[101];       // Fixed-length 100 chars + null terminator
};
#pragma pack(pop) // Restore default packing

const char* VEHICLE_FILE = "vehicles.dat";
const int RECORD_SIZE = sizeof(VehicleRecord);

static fstream vehicleFile; // Persistent file handle

void Vehicle::initialize() {
    // Open or create file in binary read/write mode
    vehicleFile.open(VEHICLE_FILE, 
                    ios::in | ios::out | ios::binary | ios::app);
    
    // If file doesn't exist, create it
    if (!vehicleFile) {
        vehicleFile.clear();
        vehicleFile.open(VEHICLE_FILE, ios::out | ios::binary);
        vehicleFile.close();
        vehicleFile.open(VEHICLE_FILE, 
                        ios::in | ios::out | ios::binary);
    }
    
    // Ensure read/write position at start
    vehicleFile.seekg(0, ios::beg);
    vehicleFile.seekp(0, ios::beg);
}

void Vehicle::shutdown() {
    if (vehicleFile.is_open()) {
        vehicleFile.close();
    }
}

bool Vehicle::isValidVehicle(const string& vehiclePlate) {
    VehicleRecord rec;
    vehicleFile.seekg(0, ios::beg);
    
    while (vehicleFile.read(reinterpret_cast<char*>(&rec), RECORD_SIZE)) {
        if (strcmp(rec.plate, vehiclePlate.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

void Vehicle::createVehicle(const string& vehicleInfo) {
    // Parse vehicle info (simplified example)
    VehicleRecord newRec;
    memset(&newRec, 0, RECORD_SIZE);
    
    // Example parsing - in real code use proper parsing
    size_t pos = vehicleInfo.find(',');
    string plate = vehicleInfo.substr(0, pos);
    string info = (pos != string::npos) ? vehicleInfo.substr(pos + 1) : "";
    
    strncpy(newRec.plate, plate.c_str(), 20);
    strncpy(newRec.otherInfo, info.c_str(), 100);
    
    // Append to end of file
    vehicleFile.seekp(0, ios::end);
    vehicleFile.write(reinterpret_cast<const char*>(&newRec), RECORD_SIZE);
    vehicleFile.flush();
}