// Vessel.h
//******************************************************************
// DEFINITION MODULE: Vessel
//
// PURPOSE:          The data model for vessels.
//
// (* Revision History:
//   Rev. 1.1 - 2025/07/23 - Changed VesselEntity to use fixed-size char array for binary I/O.
//   Rev. 1.0 - 2025/07/07
// *)
//******************************************************************
#ifndef VESSEL_H
#define VESSEL_H

#include <string>
#include <optional>
#include <cstring> // Required for strcmp

namespace Vessel {
    // This struct MUST use Plain Old Data (POD) types for binary file I/O.
    // std::string is not a POD type.
    #pragma pack(push, 1)
    struct VesselEntity {
        char vesselID[21]; // Use a fixed-size char array instead of std::string
        double LCLL;
        double HCLL;

        // Equality operator for Utility functions
        bool operator==(const VesselEntity& other) const {
            return strcmp(vesselID, other.vesselID) == 0;
        }
    };
    #pragma pack(pop)

    void init();
    void shutdown();
    void createVessel(const std::string& vesselID, double LCLL, double HCLL);
    std::optional<VesselEntity> getVessel(const std::string& vesselID);
    bool isValidVessel(const std::string& vesselID);
    // Added for the unit test
    void deleteVessel(const std::string& vesselID);
}

#endif 