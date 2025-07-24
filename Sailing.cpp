// Sailing.cpp
//******************************************************************
// IMPLEMENTATION FILE: Sailing
//
// PURPOSE:          Implements sailing data operations by calling the
//                   generic CRUD functions in the Utility module.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/23 - Final implementation using Utility module
// *)
//******************************************************************
#include "Sailing.h"
#include "Utility.h"
#include "Vessel.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <optional>

// This file implements the functions declared in Sailing.h
namespace Sailing {

    void init() {
        // All file setup is handled by Utility::init(). This function is a
        // required part of the module lifecycle, called by main.
        std::cout << "MODEL/Sailing: Initialized." << std::endl;
    }

    void shutdown() {
        // All file shutdown is handled by Utility::shutdown().
        std::cout << "MODEL/Sailing: Shut down." << std::endl;
    }

    std::optional<SailingEntity> getSailing(const std::string& sailingID) {
        int position = 0;
        while (true) {
            // Read each record sequentially using the generic Utility function.
            auto record = Utility::readRecord<SailingEntity>(position);
            if (!record.has_value()) {
                return {}; // Reached end of file, sailing not found.
            }
            // Use strcmp for comparing C-style char arrays.
            if (strcmp(record->sailingID, sailingID.c_str()) == 0) {
                return record; // Found the matching sailing.
            }
            position++;
        }
    }

    bool isValidSailing(const std::string& sailingID) {
        // A sailing is considered valid if the getSailing function finds it.
        return getSailing(sailingID).has_value();
    }

    void createSailing(const std::string& vesselID, const std::string& sailingID) {
        // A sailing's initial capacity is based on its assigned vessel.
        auto vesselOpt = Vessel::getVessel(vesselID);
        if (!vesselOpt.has_value()) {
            // Cannot create a sailing without a valid vessel.
            std::cerr << "ERROR: Attempted to create sailing for non-existent vessel '" << vesselID << "'" << std::endl;
            return;
        }
        
        SailingEntity newSailing = {}; // Zero-initialize the struct
        strncpy(newSailing.sailingID, sailingID.c_str(), 20);
        newSailing.sailingID[20] = '\0'; // Ensure null termination
        strncpy(newSailing.vesselID, vesselID.c_str(), 20);
        newSailing.vesselID[20] = '\0';
        
        // Copy the vessel's max capacity as the initial remaining capacity.
        newSailing.LRL = vesselOpt->LCLL;
        newSailing.HRL = vesselOpt->HCLL;

        // Use the generic Utility function to write the new record to the file.
        Utility::createRecord(newSailing);
    }

    void deleteSailing(const std::string& sailingID) {
        // The Utility::deleteRecord function your team wrote requires the whole object.
        auto sailingOpt = getSailing(sailingID);
        if (sailingOpt.has_value()) {
            SailingEntity toDelete = *sailingOpt;
            Utility::deleteRecord(toDelete);
        } else {
            std::cerr << "ERROR: Attempted to delete non-existent sailing '" << sailingID << "'" << std::endl;
        }
    }

    namespace {
        void updateCapacity(const std::string& sailingID, double lrlChange, double hrlChange) {
            auto recordOpt = getSailing(sailingID);
            if (recordOpt.has_value()) {
                SailingEntity updatedRecord = *recordOpt;
                updatedRecord.LRL += lrlChange;
                updatedRecord.HRL += hrlChange;
                Utility::updateRecord(updatedRecord);
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
        int position = offset; // Start reading from the specified offset.
        while (true) {
            auto record = Utility::readRecord<SailingEntity>(position);
            if (!record.has_value()) {
                break; // End of file
            }
            allSailings.push_back(*record);
            position++;
        }
        return allSailings;
    }

} 
