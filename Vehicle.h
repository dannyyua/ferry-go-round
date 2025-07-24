// Vehicle.h
//******************************************************************
// DEFINITION MODULE: Vehicle
//
// PURPOSE:          The data model for vehicles. Declares the functions
//                   for initializing, shutting down, and validating
//                   vehicle data. This module represents the interface to
//                   the vehicle "database table".
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
//   Rev. 1.1 - 2025/07/23
//     - Data model for vehicles using Utility CRUD operations
//   Rev. 1.2 - 2025/07/23
//     - Modified createVehicle to accept separate parameters
//     - Added getVehicleLength, getVehicleHeight, getVehiclePhone functions
// *)
//******************************************************************
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include "Utility.h"

namespace Vehicle {
    // Entity structure for persistent storage
    #pragma pack(push, 1)
    struct VehicleEntity {
        char plate[21];            // Fixed-length 20 chars + null
        char phone[16];            // Fixed-length 15 chars + null (e.g. +1-555-123-4567)
        double length;             // Vehicle length in meters (8 bytes)
        double height;             // Vehicle height in meters (8 bytes)
        
        // Equality operator for CRUD operations
        bool operator==(const VehicleEntity& other) const {
            return strcmp(plate, other.plate) == 0;
        }
    };
    #pragma pack(pop)

    //----------- CRUD Interface -----------
    void init();
    //-----------
    void shutdown();
    //-----------
    // Corresponds to OCD "isValidVehicle()".
    bool isValidVehicle(const std::string& vehiclePlate);
    //-----------
    // Corresponds to OCD "createVehicle()".
    void createVehicle(const std::string& vehiclePlate, const std::string& phoneNumber, double length, double height);
    //-----------
    double getVehicleLength(const std::string& vehiclePlate);
    //-----------
    double getVehicleHeight(const std::string& vehiclePlate);
    //-----------
    std::string getVehiclePhone(const std::string& vehiclePlate);
    //-----------
    std::optional<VehicleEntity> getVehicle(const std::string& vehiclePlate);
}

#endif // VEHICLE_H