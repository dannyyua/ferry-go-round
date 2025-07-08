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
// *)
//******************************************************************
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

// The Vehicle namespace acts as a static class for managing Vehicle data.
namespace Vehicle {
    //-----------
    void initialize(); // Initializes the vehicle data source.
    //-----------
    void shutdown(); // Shuts down the vehicle data source.

    //-----------
    // Corresponds to OCD "isValidVehicle()".
    // Checks if a vehicle with the given license plate exists in the database.
    bool isValidVehicle(const std::string& vehiclePlate);

    //-----------
    // Corresponds to OCD "createVehicle()".
    // Adds a new vehicle record to the database.
    void createVehicle(const std::string& vehicleInfo);
}

#endif // VEHICLE_H
