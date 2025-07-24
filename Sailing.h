// Sailing.h (FINAL CORRECTED VERSION)
//******************************************************************
// DEFINITION MODULE: Sailing
//
// PURPOSE:          Data model for sailings. Defines the SailingEntity
//                   structure and declares functions for data operations.
//
// (* Revision History:
//   Rev. 1.2 - 2025/07/23 - Final version for A4.
// *)
//******************************************************************
#ifndef SAILING_H
#define SAILING_H

#include <string>
#include <vector>
#include <optional>
#include <cstring>

namespace Sailing {

    #pragma pack(push, 1)
    struct SailingEntity {
        char sailingID[21];
        char vesselID[21];
        double LRL;
        double HRL;
        
        bool operator==(const SailingEntity& other) const {
            return strcmp(sailingID, other.sailingID) == 0;
        }
    };
    #pragma pack(pop)

    void init();
    void shutdown();
    bool isValidSailing(const std::string& sailingID);
    void createSailing(const std::string& vesselID, const std::string& sailingID);
    void deleteSailing(const std::string& sailingID);
    std::optional<SailingEntity> getSailing(const std::string& sailingID);
    std::vector<SailingEntity> getSailings(int offset);
    void decreaseLRL(const std::string& sailingID, double length);
    void increaseLRL(const std::string& sailingID, double length);
    void decreaseHRL(const std::string& sailingID, double length);
    void increaseHRL(const std::string& sailingID, double length);
}

#endif // SAILING_H
