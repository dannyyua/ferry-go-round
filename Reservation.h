// Reservation.h
//******************************************************************
// DEFINITION MODULE: Reservation
//
// PURPOSE:          The data model for reservations. Declares the functions
//                   for all reservation-related data operations, such as
//                   creating, cancelling, and validating.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
//   Rev. 1.1 - 2025/07/23
//     - Refactored to use Utility functions for CRUD operations
//   Rev. 1.2 - 2025/07/23
//     - Added getReservation function
// *)
//******************************************************************
#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <optional>
#include "Utility.h"

namespace Reservation {
    // Entity structure for persistent storage
    #pragma pack(push, 1)
    struct ReservationEntity {
        char sailingID[21];        // Fixed-length 20 chars + null
        char vehiclePlate[21];     // Fixed-length 20 chars + null
        bool checkedIn;            // 1 byte
        
        // Equality operator for CRUD operations
        bool operator==(const ReservationEntity& other) const {
            return strcmp(sailingID, other.sailingID) == 0 &&
                   strcmp(vehiclePlate, other.vehiclePlate) == 0;
        }
    };
    #pragma pack(pop)

    //----------- CRUD Interface -----------
    void init();
    //-----------
    void shutdown();
    //-----------
    // Corresponds to OCD "createReservation()".
    void createReservation(const std::string& sailingID, const std::string& vehiclePlate);
    //-----------
    // Corresponds to OCD "cancelReservation()".
    void cancelReservation(const std::string& sailingID, const std::string& vehiclePlate);
    //-----------
    // Corresponds to OCD "deleteReservations()".
    void deleteReservations(const std::string& sailingID);
    //-----------  
    // Corresponds to OCD "isValidReservation()".
    bool isValidReservation(const std::string& vehiclePlate);
    //-----------
    // Corresponds to OCD "checkIn()".
    void checkIn(const std::string& vehiclePlate);
    //-----------
    // New function to retrieve a reservation by vehicle plate
    std::optional<ReservationEntity> getReservation(const std::string& vehiclePlate);
}

#endif // RESERVATION_H