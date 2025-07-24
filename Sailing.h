// Sailing.h
//******************************************************************
// DEFINITION MODULE: Sailing
//
// PURPOSE:          The data model for sailings. Declares the functions
//                   for all sailing-related data operations and defines
//                   the SailingEntity structure for persistent storage.
//
// (* Revision History:
//   Rev. 1.2 - 2025/07/23 - Final version to align with Utility module
// *)
//******************************************************************
#ifndef SAILING_H
#define SAILING_H

#include <string>
#include <vector>
#include <optional>
#include <cstring> // For strcmp

namespace Sailing {

    // Entity structure for persistent, fixed-length binary storage.
    #pragma pack(push, 1)
    struct SailingEntity {
        char sailingID[21];
        char vesselID[21];
        double LRL;
        double HRL;
        
        // Equality operator is required by the Utility::deleteRecord function.
        bool operator==(const SailingEntity& other) const {
            return strcmp(sailingID, other.sailingID) == 0;
        }
    };
    #pragma pack(pop)

    //----------- Lifecycle Interface -----------
    void init();
    void shutdown();

    //----------- Data Manipulation & Query Interface -----------
    bool isValidSailing(const std::string& sailingID);
    void createSailing(const std::string& vesselID, const std::string& sailingID);
    void deleteSailing(const std::string& sailingID);
    
    std::optional<SailingEntity> getSailing(const std::string& sailingID);
    std::vector<SailingEntity> getSailings(int offset);

    //----------- Capacity Management Interface -----------
    void decreaseLRL(const std::string& sailingID, double length);
    void increaseLRL(const std::string& sailingID, double length);
    void decreaseHRL(const std::string& sailingID, double length);
    void increaseHRL(const std::string& sailingID, double length);
}

#endif // SAILING_H
