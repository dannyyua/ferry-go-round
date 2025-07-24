// Vehicle.cpp
//******************************************************************
// DEFINITION MODULE: Vehicle
//
// PURPOSE: Implements vehicle data operations including:
//          - Validating vehicle license plates
//          - Creating new vehicle records
//          - Retrieving vehicle length, height and phone number
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/22
//   Rev. 1.1 - 2025/07/23
//     - Data model for vehicles using Utility CRUD operations
//   Rev. 1.2 - 2025/07/23
//     - Modified createVehicle to accept separate parameters
//     - Added getVehicleLength, getVehicleHeight, getVehiclePhone functions
// *)
//******************************************************************
#include "Vehicle.h"
#include <cstring>
#include <stdexcept>

using namespace Vehicle;
using namespace Utility;

// Compatibility placeholders
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

void Vehicle::createVehicle(const std::string& vehiclePlate, const std::string& phoneNumber, double length, double height) {
    // Validate dimensions
    if (length <= 0 || height <= 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }

    // Validate phone format (simplified)
    if (phoneNumber.empty() || phoneNumber.size() > 15) {
        throw std::invalid_argument("Phone number must be 1-15 characters");
    }

    // Create entity
    VehicleEntity newEntity;
    memset(&newEntity, 0, sizeof(VehicleEntity));
    strncpy(newEntity.plate, vehiclePlate.c_str(), 20);
    strncpy(newEntity.phone, phoneNumber.c_str(), 15);
    newEntity.length = length;
    newEntity.height = height;

    // Create record via Utility
    createRecord(newEntity);
}

double Vehicle::getVehicleLength(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<VehicleEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->plate, vehiclePlate.c_str()) == 0) {
            return record->length;
        }
        position++;
    }
    throw std::runtime_error("Vehicle not found: " + vehiclePlate);
}

double Vehicle::getVehicleHeight(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<VehicleEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->plate, vehiclePlate.c_str()) == 0) {
            return record->height;
        }
        position++;
    }
    throw std::runtime_error("Vehicle not found: " + vehiclePlate);
}

std::string Vehicle::getVehiclePhone(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<VehicleEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->plate, vehiclePlate.c_str()) == 0) {
            return std::string(record->phone);
        }
        position++;
    }
    throw std::runtime_error("Vehicle not found: " + vehiclePlate);
}

std::optional<VehicleEntity> Vehicle::getVehicle(const std::string& vehiclePlate){
    int position = 0;
    while (true) {
        auto record = readRecord<VehicleEntity>(position);
        if (!record.has_value()) break;

        if (strcmp(record->plate, vehiclePlate.c_str()) == 0) {
            return record;
        }
        position++;
    }
    return std::nullopt;
}