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
//   Rev. 1.1 - 2025/07/23
//     - Data model for vehicles using Utility CRUD operations
// *)
//******************************************************************
#include "Vehicle.h"
#include <cstring>

using namespace Vehicle;
using namespace Utility;

// No file handles - managed by Utility

void Vehicle::init() {}

void Vehicle::shutdown() {}

bool Vehicle::isValidVehicle(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<VehicleEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->plate, vehiclePlate.c_str()) == 0) {
            return true;
        }
        position++;
    }
    return false;
}

void Vehicle::createVehicle(const std::string& vehicleInfo) {
    // Parse vehicle info
    size_t pos = vehicleInfo.find(',');
    std::string plate = vehicleInfo.substr(0, pos);
    std::string info = (pos != std::string::npos) ? 
                      vehicleInfo.substr(pos + 1) : "";
    
    // Create entity
    VehicleEntity newEntity;
    memset(&newEntity, 0, sizeof(VehicleEntity));
    strncpy(newEntity.plate, plate.c_str(), 20);
    strncpy(newEntity.otherInfo, info.c_str(), 100);
    
    // Create record via Utility
    createRecord(newEntity);
}