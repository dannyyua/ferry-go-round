// Sailing.h
//******************************************************************
// DEFINITION MODULE: Sailing
//
// PURPOSE:          The data model for sailings. Declares the functions
//                   for all sailing-related data operations, abstracting
//                   the underlying data source.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
//   Rev. 1.1 - 2025/07/08 
//     - Add 3 Capacity functions (check if exist, decrease capacity, increase capacity)
// *)
//******************************************************************
#ifndef SAILING_H
#define SAILING_H

#include <string>
#include <vector> // Often used for returning multiple records

// The Sailing namespace acts as a static class for managing Sailing data.
namespace Sailing {
    //-----------
    void initialize();
    //-----------
    void shutdown();

    //-----------
    // Corresponds to Sailing management from OCD"
    bool hasCapacity(const std::string& sailingID, float requiredLength);
    //-----------
    // Decrease in lane capacity
    void decreaseLRL(const std::string& sailingID, float length);
    //-----------
    // Increase in lane capacity
    void increaseLRL(const std::string& sailingID, float length);

    //-----------
    // Corresponds to OCD "createSailing()".
    void createSailing(const std::string& sailingInfo);

    //-----------
    // Corresponds to OCD "deleteSailing()".
    void deleteSailing(const std::string& sailingID);
    
    //-----------
    // Corresponds to OCD "isValidSailing()".
    bool isValidSailing(const std::string& sailingID);

    //-----------
    // Corresponds to OCD "getSailings()". In a real app, this would return data.
    void getSailings(int offset);

    //-----------
    // Corresponds to OCD "getSailing()".
    void getSailing(const std::string& sailingID);
}

#endif // SAILING_H
