// Vessel.h
//******************************************************************
// DEFINITION MODULE: Vessel
//
// PURPOSE:          The data model for vessels. Declares the functions
//                   for initializing, shutting down, creating, and
//                   validating vessel data, representing the interface
//                   to the vessel "database table".
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
// *)
//******************************************************************
#ifndef VESSEL_H
#define VESSEL_H

#include <string>

// The Vessel namespace acts as a static class for managing Vessel data.
namespace Vessel {
    //-----------
    void initialize(); // Initializes the vessel data source.
    //-----------
    void shutdown(); // Shuts down the vessel data source.

    //-----------
    // Corresponds to OCD "createVessel()".
    // In a real implementation, vesselInfo would be a struct.
    void createVessel(const std::string& vesselInfo);

    //-----------
    // Corresponds to OCD "isValidVessel()".
    // Checks if a vessel with the given ID exists in the database.
    bool isValidVessel(const std::string& vesselID);
}

#endif // VESSEL_H
