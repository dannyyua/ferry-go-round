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
        char otherInfo[101];       // Fixed-length 100 chars + null
        
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
    void createVehicle(const std::string& vehicleInfo);
}

#endif // VEHICLE_H