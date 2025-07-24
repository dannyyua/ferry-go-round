// Utility.h (CORRECTED AND RE-ENGINEERED)
//******************************************************************
// DEFINITION MODULE: Utility
//
// PURPOSE:          A utility module that provides generic, templated
//                   CRUD (Create, Read, Update, Delete) operations for
//                   any fixed-size data structure.
//
// (* Revision History:
//   Rev. 1.1 - 2025/07/23 - Moved template definitions to header, removed
//                          fileHandles map, and simplified file I/O to be
//                          safer and standards-compliant.
//   Rev. 1.0 - 2025/07/22 - Initial version
// *)
//******************************************************************
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <type_traits> // For std::is_same_v

// Forward declarations of the data entity structs are required
namespace Vessel { struct VesselEntity; }
namespace Sailing { struct SailingEntity; }
namespace Vehicle { struct VehicleEntity; }
namespace Reservation { struct ReservationEntity; }

namespace Utility {
    
    // Non-template functions can be declared here.
    void init();
    void shutdown();
    // The full body of template functions MUST be in the header file.

    // Generic helper to get the correct file path for a given data type T.
    template <typename T>
    std::string getFilePath() {
        // 'if constexpr' is a compile-time if, which is very efficient.
        if constexpr (std::is_same_v<T, Vessel::VesselEntity>) { return "Data/Vessels.dat"; }
        if constexpr (std::is_same_v<T, Sailing::SailingEntity>) { return "Data/Sailings.dat"; }
        if constexpr (std::is_same_v<T, Vehicle::VehicleEntity>) { return "Data/Vehicles.dat"; }
        if constexpr (std::is_same_v<T, Reservation::ReservationEntity>) { return "Data/Reservations.dat"; }
        // This line will cause a compile-time error if an unknown type is used, which is a good safety feature.
        throw std::runtime_error("Data file path not defined for this entity type.");
    }

    // Creates a new record by appending it to the end of the file.
    template <typename T>
    void createRecord(const T& object) {
        std::string path = getFilePath<T>();
        // Open file in output, binary, and append mode. Creates file if it doesn't exist.
        std::ofstream file(path, std::ios::binary | std::ios::app);
        if (!file) {
            std::cerr << "ERROR: Could not open file for writing: " << path << std::endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(&object), sizeof(T));
    }

    // Reads a single record from a specific 0-indexed position.
    template <typename T>
    std::optional<T> readRecord(int position) {
        std::string path = getFilePath<T>();
        std::ifstream file(path, std::ios::binary);
        if (!file) return {}; // File doesn't exist yet, return empty optional.

        file.seekg(position * sizeof(T), std::ios::beg);
        T record;
        file.read(reinterpret_cast<char*>(&record), sizeof(T));

        if (file.gcount() < sizeof(T)) {
            return {}; // Didn't read a full record (most likely end of file).
        }
        return record;
    }

    // Updates a record at a specific 0-indexed position by overwriting it.
    template <typename T>
    void updateRecord(int position, const T& object) {
        std::string path = getFilePath<T>();
        // Open for both reading and writing to overwrite in place.
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;

        file.seekp(position * sizeof(T), std::ios::beg);
        file.write(reinterpret_cast<const char*>(&object), sizeof(T));
    }

    // Deletes a record by overwriting it with the last record and then truncating the file.
    template <typename T>
    bool deleteRecord(int position) {
        std::string path = getFilePath<T>();
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return false;

        // Find the total number of records in the file
        file.seekg(0, std::ios::end);
        long long totalSize = file.tellg();
        int recordCount = totalSize / sizeof(T);

        if (position < 0 || position >= recordCount) return false; // Invalid position

        // If it's not the last record, move the last one into its place
        if (position < recordCount - 1) {
            T lastRecord;
            file.seekg(-static_cast<long long>(sizeof(T)), std::ios::end);
            file.read(reinterpret_cast<char*>(&lastRecord), sizeof(T));
            
            file.seekp(position * sizeof(T), std::ios::beg);
            file.write(reinterpret_cast<const char*>(&lastRecord), sizeof(T));
        }

        file.close(); // MUST close the file before truncating
        
        // Truncate the file to be one record shorter.
        std::filesystem::resize_file(path, totalSize - sizeof(T));
        return true;
    }
}

#endif // UTILITY_H
