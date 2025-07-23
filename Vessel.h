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
#include <optional>

// The Vessel namespace acts as a static class for managing Vessel data.
namespace Vessel {
    // Domain entity representing a Vessel.
    struct VesselEntity {
        std::string vesselID;
        double LCLL;
        double HCLL;

        bool operator==(const VesselEntity& other) const {
            return vesselID == other.vesselID;
        }
    };

    //-----------
    void init(); // Initializes the vessel data source.
    //-----------
    void shutdown(); // Shuts down the vessel data source.

    //-----------
    // Corresponds to OCD "createVessel()".
    // Creates a new vessel record in the database.
    void createVessel(const std::string& vesselID, double LCLL, double HCLL);

    //-----------
    // Corresponds to OCD "getVessel()".
    // Retrieves a vessel record by its ID.
    std::optional<VesselEntity> getVessel(const std::string& vesselID);

    //-----------
    // Corresponds to OCD "isValidVessel()".
    // Checks if a vessel with the given ID exists in the database.
    bool isValidVessel(const std::string& vesselID);
}

#endif // VESSEL_H
