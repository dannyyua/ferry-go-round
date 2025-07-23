//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*******************************
// Utility.cpp
//
// Utility module that provides common functions for file handling and data management.
//
// Rev 1.0 - 2025-07-22
//     - Initial version
//*******************************

#include "Utility.h"
#include "Vessel.h"
#include "Sailing.h"
#include "Vehicle.h"
#include "Reservation.h"
#include <filesystem>

namespace Utility {
    // Open files for each entity type.
    void init() {
        auto flags = std::fstream::in | std::fstream::out | std::fstream::binary;

        fileHandles[SAILING] = std::fstream("Data/Sailings", flags);
        fileHandles[VESSEL] = std::fstream("Data/Vessels", flags);
        fileHandles[VEHICLE] = std::fstream("Data/Vehicles", flags);
        fileHandles[RESERVATION] = std::fstream("Data/Reservations", flags);

        if (!fileHandles[SAILING].good() ||
            !fileHandles[VESSEL].good() ||
            !fileHandles[VEHICLE].good() ||
            !fileHandles[RESERVATION].good()) {
            throw std::runtime_error("Failed to open one or more data files.");
        }
    }

    // Close all file handles.
    void shutdown() {
        for (auto& [_, fileHandle] : fileHandles) {
            fileHandle.close();
        }
    }

    // Helper function for fetching file handles based on entity type.
    template <typename T>
    std::fstream& getFile() {
        if (std::is_same_v<T, Sailing::SailingEntity>) {
            return fileHandles[SAILING];
        } else if (std::is_same_v<T, Vessel::VesselEntity>) {
            return fileHandles[VESSEL];
        } else if (std::is_same_v<T, Vehicle::VehicleEntity>) {
            return fileHandles[VEHICLE];
        } else if (std::is_same_v<T, Reservation::ReservationEntity>) {
            return fileHandles[RESERVATION];
        } else {
            throw std::runtime_error("Data filename not defined for type.");
        }
    }

    // Helper function to get the file path of an entity type.
    template <typename T>
    std::string getFilePath() {
        if (std::is_same_v<T, Sailing::SailingEntity>) {
            return "Data/Sailings";
        } else if (std::is_same_v<T, Vessel::VesselEntity>) {
            return "Data/Vessels";
        } else if (std::is_same_v<T, Vehicle::VehicleEntity>) {
            return "Data/Vehicles";
        } else if (std::is_same_v<T, Reservation::ReservationEntity>) {
            return "Data/Reservations";
        } else {
            throw std::runtime_error("Data file path not defined for type.");
        }
    }

    // Write a record into the data file.
    template <typename T>
    int createRecord(T& object) {
        std::fstream& file = getFile<T>();

        file.seekp(0, std::ios::end);
        file.write(reinterpret_cast<char*>(&object), sizeof(object));
        file.clear();

        return 0;
    }

    // Get a record from data by position.
    template <typename T>
    std::optional<T> readRecord(int position) {
        std::fstream& file = getFile<T>();

        file.seekg(position * sizeof(T), std::ios::beg);
        T record;
        file.read(reinterpret_cast<char*>(&record), sizeof(T));

        if (!file.good()) {
            file.clear();
            return {};
        } else {
            file.clear();
            return record;
        }
    }

    // Update a record by deleting and then creating it again.
    template <typename T>
    int updateRecord(T& object) {
        deleteRecord<T>(object);
        createRecord<T>(object);

        return 0;
    }

    // Deletes a record by replacing it with the last record in the file.
    // File is truncated to remove the last record.
    template <typename T>
    int deleteRecord(T& object) {
        std::fstream& file = getFile<T>();

        T lastRecord;
        file.seekg(-sizeof(T), std::ios::end);
        file.read(reinterpret_cast<char*>(&lastRecord), sizeof(T));

        int position = 0;

        while (true) {
            std::optional<T> record = readRecord<T>(position); 

            if (!record.has_value()) {
                return 1;
            }

            if (record.value() == object) {
                file.seekp(position * sizeof(T), std::ios::beg);
                file.write(reinterpret_cast<char*>(&lastRecord), sizeof(T));
                file.clear();

                std::filesystem::resize_file(getFilePath<T>(), std::filesystem::file_size(getFilePath<T>()) - sizeof(T));

                return 0;
            }

            position++;
        }

        return 1;
    }
}