// Vessel.cpp 
//*******************************
// Vessel.cpp
//
// Low-level Vessel module that manages vessel data.
//
// Rev 1.1 - 2025-07-23 - Fixed infinite loop in getVessel and added deleteVessel.
// Rev 1.0 - 2025-07-22 - Initial version
//*******************************

#include "Vessel.h"
#include "Utility.h"
#include <cstring> // For strncpy and strcmp
#include <iostream>

namespace Vessel {
    void init() {
        std::cout << "MODEL/Vessel: Initialized." << std::endl;
    }

    void shutdown() {
        std::cout << "MODEL/Vessel: Shut down." << std::endl;
    }

    void createVessel(const std::string& vesselID, double LCLL, double HCLL) {
        VesselEntity newVessel = {}; // Zero-initialize
        // Safely copy the std::string into the fixed-size char array
        strncpy(newVessel.vesselID, vesselID.c_str(), 20);
        newVessel.vesselID[20] = '\0'; // Ensure null-termination
        newVessel.LCLL = LCLL;
        newVessel.HCLL = HCLL;

        Utility::createRecord(newVessel);
    }

    std::optional<VesselEntity> getVessel(const std::string& vesselID) {
        int position = 0;
        while (true) {
            auto recordOpt = Utility::readRecord<VesselEntity>(position);
            if (!recordOpt.has_value()) {
                return {}; // Reached end of file, not found.
            }
            if (strcmp(recordOpt->vesselID, vesselID.c_str()) == 0) {
                return recordOpt; // Found it.
            }
            position++;
        }
    }
    
    bool isValidVessel(const std::string& vesselID) {
        return getVessel(vesselID).has_value();
    }
    
    // Added implementation for deleteVessel
    void deleteVessel(const std::string& vesselID) {
        int position = 0;
        while (true) {
            auto recordOpt = Utility::readRecord<VesselEntity>(position);
            if (!recordOpt.has_value()) {
                std::cerr << "ERROR: Cannot delete non-existent vessel '" << vesselID << "'" << std::endl;
                return;
            }
            if (strcmp(recordOpt->vesselID, vesselID.c_str()) == 0) {
                Utility::deleteRecord<VesselEntity>(position);
                return;
            }
            position++;
        }
    }
}