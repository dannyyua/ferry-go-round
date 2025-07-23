// Utility.h
//******************************************************************
// DEFINITION MODULE: Utility
//
// PURPOSE:          A utility module mainly used for writing/reading storage objects
//                   to/from persistent storage, as well as other utility functions.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/22
// *)
//******************************************************************
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <type_traits>
#include <stdexcept>
#include <unordered_map>
#include <fstream>
#include <optional>

// The Utility namespace provides an interface to the system's utility functions,
// including persistent storage.
namespace Utility {
    // List of entity types used by the system.
    enum EntityType {
        SAILING,
        VESSEL,
        VEHICLE,
        RESERVATION
    };

    // Map of file handles for each entity type.
    std::unordered_map<EntityType, std::fstream&> fileHandles;

    // Used for opening and closing file handles.
    //-----------
    void init();
    //-----------
    void shutdown();

    // CRUD operations for persistent storage.
    //-----------
    template <typename T>
    int createRecord(T& object);
    //-----------
    template <typename T>
    std::optional<T> readRecord(int position);
    //-----------
    template <typename T>
    int updateRecord(T& object);
    //-----------
    template <typename T>
    int deleteRecord(T& object);
}

#endif // UTILITY_H
