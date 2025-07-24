// Sailing.cpp (Written to match the team's Utility module)
//******************************************************************
// IMPLEMENTATION FILE: Sailing
//
// PURPOSE:          Implements sailing data operations by calling the
//                   generic CRUD functions provided by the Utility module.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/23 - Implementation using the defined Utility interface
// *)
//******************************************************************
#include "Sailing.h"
#include "Utility.h" // Relies on the Utility module for all file I/O
#include "Vessel.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <optional>

// The "using namespace" directives are needed because the team's other
// .cpp files use them. This keeps the style consistent.
using namespace Sailing;
using namespace Utility;

void Sailing::init() {
    // Per the design, all file initialization is handled by Utility::init().
    // This function remains as a required part of the module's lifecycle interface.
    std::cout << "MODEL/Sailing: Initialized." << std::endl;
}

void Sailing::shutdown() {
    // All file shutdown is handled by Utility::shutdown().
    std::cout << "MODEL/Sailing: Shut down." << std::endl;
}

std::optional<SailingEntity> Sailing::getSailing(const std::string& sailingID) {
    int position = 0;
    while (true) {
        // Reads each record sequentially using the generic Utility function.
        auto record = readRecord<SailingEntity>(position);
        if (!record.has_value()) {
            return {}; // Reached end of file, sailing not found.
        }
        if (strcmp(record->sailingID, sailingID.c_str()) == 0) {
            return record; // Found the matching sailing.
        }
        position++;
    }
}

bool Sailing::isValidSailing(const std::string& sailingID) {
    // A sailing is considered valid if the getSailing function finds it.
    return getSailing(sailingID).has_value();
}

void Sailing::createSailing(const std::string& vesselID, const std::string& sailingID) {
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
    createRecord(newSailing);
}

void Sailing::deleteSailing(const std::string& sailingID) {
    // The Utility::deleteRecord function requires the object itself to find and delete.
    auto sailingOpt = getSailing(sailingID);
    if (sailingOpt.has_value()) {
        SailingEntity toDelete = *sailingOpt;
        deleteRecord(toDelete);
    } else {
        std::cerr << "ERROR: Attempted to delete non-existent sailing '" << sailingID << "'" << std::endl;
    }
}

// Internal helper function to find a sailing, update it, and save it back.
namespace {
    void updateCapacity(const std::string& sailingID, double lrlChange, double hrlChange) {
        auto recordOpt = getSailing(sailingID);
        if (recordOpt.has_value()) {
            SailingEntity updatedRecord = *recordOpt;
            updatedRecord.LRL += lrlChange;
            updatedRecord.HRL += hrlChange;
            // The team's Utility::updateRecord is a delete-then-create operation.
            updateRecord(updatedRecord);
        }
    }
}

void Sailing::decreaseLRL(const std::string& sailingID, double length) {
    updateCapacity(sailingID, -length, 0.0);
}

void Sailing::increaseLRL(const std::string& sailingID, double length) {
    updateCapacity(sailingID, length, 0.0);
}

void Sailing::decreaseHRL(const std::string& sailingID, double length) {
    updateCapacity(sailingID, 0.0, -length);
}

void Sailing::increaseHRL(const std::string& sailingID, double length) {
    updateCapacity(sailingID, 0.0, length);
}

std::vector<SailingEntity> Sailing::getSailings(int offset) {
    std::vector<SailingEntity> allSailings;
    int position = 0;
    while (true) {
        auto record = readRecord<SailingEntity>(position);
        if (!record.has_value()) {
            break; // End of file
        }
        allSailings.push_back(*record);
        position++;
    }
    // Note: A real implementation would handle the 'offset' for pagination.
    return allSailings;
}