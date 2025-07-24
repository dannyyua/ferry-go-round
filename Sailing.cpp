// Sailing.cpp 
//******************************************************************
// IMPLEMENTATION FILE: Sailing
//
// PURPOSE:          Implements sailing data operations by calling the
//                   generic CRUD functions in the Utility module.
//
// (* Revision History:
//   Rev. 1.1 - 2025/07/23 - Corrected calls to Utility functions to use position.
//   Rev. 1.0 - 2025/07/07 - Initial implementation.
// *)
//******************************************************************
#include "Sailing.h"
#include "Utility.h"
#include "Vessel.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <optional>

namespace Sailing {

    void init() {
        std::cout << "MODEL/Sailing: Initialized." << std::endl;
    }

    void shutdown() {
        std::cout << "MODEL/Sailing: Shut down." << std::endl;
    }

    // Internal helper function to find a record's position.
    namespace {
        int findRecordPosition(const std::string& sailingID) {
            int position = 0;
            while (true) {
                auto record = Utility::readRecord<SailingEntity>(position);
                if (!record.has_value()) return -1;
                if (strcmp(record->sailingID, sailingID.c_str()) == 0) return position;
                position++;
            }
        }
    }

    std::optional<SailingEntity> getSailing(const std::string& sailingID) {
        int position = findRecordPosition(sailingID);
        return (position != -1) ? Utility::readRecord<SailingEntity>(position) : std::nullopt;
    }

    bool isValidSailing(const std::string& sailingID) {
        return findRecordPosition(sailingID) != -1;
    }

    void createSailing(const std::string& vesselID, const std::string& sailingID) {
        auto vesselOpt = Vessel::getVessel(vesselID);
        if (!vesselOpt.has_value()) {
            std::cerr << "ERROR: Cannot create sailing for non-existent vessel '" << vesselID << "'" << std::endl;
            return;
        }
        
        SailingEntity newSailing = {};
        strncpy(newSailing.sailingID, sailingID.c_str(), 20);
        newSailing.sailingID[20] = '\0';
        strncpy(newSailing.vesselID, vesselID.c_str(), 20);
        newSailing.vesselID[20] = '\0';
        newSailing.LRL = vesselOpt->LCLL;
        newSailing.HRL = vesselOpt->HCLL;

        Utility::createRecord(newSailing);
    }

    void deleteSailing(const std::string& sailingID) {
        int position = findRecordPosition(sailingID);
        if (position != -1) {
            // THE FIX IS HERE: Call deleteRecord with the integer position.
            Utility::deleteRecord<SailingEntity>(position);
        } else {
            std::cerr << "ERROR: Attempted to delete non-existent sailing '" << sailingID << "'" << std::endl;
        }
    }

    // Internal helper for updating capacity.
    namespace {
        void updateCapacity(const std::string& sailingID, double lrlChange, double hrlChange) {
            int position = findRecordPosition(sailingID);
            if (position != -1) {
                if (auto recordOpt = getSailing(sailingID)) {
                    SailingEntity updatedRecord = *recordOpt;
                    updatedRecord.LRL += lrlChange;
                    updatedRecord.HRL += hrlChange;
                    // THE FIX IS HERE: Call updateRecord with position and object.
                    Utility::updateRecord(position, updatedRecord);
                }
            }
        }
    }

    void decreaseLRL(const std::string& sailingID, double length) {
        updateCapacity(sailingID, -length, 0.0);
    }

    void increaseLRL(const std::string& sailingID, double length) {
        updateCapacity(sailingID, length, 0.0);
    }

    void decreaseHRL(const std::string& sailingID, double length) {
        updateCapacity(sailingID, 0.0, -length);
    }

    void increaseHRL(const std::string& sailingID, double length) {
        updateCapacity(sailingID, 0.0, length);
    }

    std::vector<SailingEntity> getSailings(int offset) {
        std::vector<SailingEntity> allSailings;
        int position = offset;
        while (true) {
            auto record = Utility::readRecord<SailingEntity>(position);
            if (!record.has_value()) break;
            allSailings.push_back(*record);
            position++;
        }
        return allSailings;
    }
}
