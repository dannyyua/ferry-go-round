//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*******************************
// Vessel.cpp
//
// Low-level Vessel module that manages vessel data.
//
// Rev 1.0 - 2025-07-22
//     - Initial version
//*******************************

#include "Vessel.h"
#include "Utility.h"

namespace Vessel {
    void init() {}

    void shutdown() {}

    // Creates a new vessel record in the database.
    void createVessel(const std::string& vesselID, double LCLL, double HCLL) {
        VesselEntity newVessel = { vesselID, LCLL, HCLL };

        Utility::createRecord<VesselEntity>(newVessel);
    }

    // Retrieves a vessel record by its ID.
    std::optional<VesselEntity> getVessel(const std::string& vesselID) {
        int position = 0;
        std::optional<VesselEntity> vessel = Utility::readRecord<VesselEntity>(position);

        while (vessel.has_value()) {
            if (vessel->vesselID != vesselID) {
                position++;
                vessel = Utility::readRecord<VesselEntity>(position);
            }
        }

        return vessel;
    }

    // Checks if a vessel with the given ID exists in the database.
    bool isValidVessel(const std::string& vesselID) {
        return getVessel(vesselID).has_value();
    }
}