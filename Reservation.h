// Reservation.h
//******************************************************************
// DEFINITION MODULE: Reservation
//
// PURPOSE:          The data model for reservations. Declares the functions
//                   for all reservation-related data operations, such as
//                   creating, cancelling, and validating.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07 - Generated for Group 15
// *)
//******************************************************************
#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

// The Reservation namespace acts as a static class for managing Reservation data.
namespace Reservation {
    //-----------
    void init();
    //-----------
    void shutdown();

    //-----------
    // Corresponds to OCD "createReservation()".
    void createReservation(const std::string& reservationInfo);

    //-----------
    // Corresponds to OCD "cancelReservation()".
    void cancelReservation(const std::string& sailingID, const std::string& vehiclePlate);

    //-----------
    // Corresponds to OCD "deleteReservationsBySailingId()".
    void deleteReservationsBySailingId(const std::string& sailingID);
    
    //-----------
    // Corresponds to OCD "isValidReservation()".
    bool isValidReservation(const std::string& vehiclePlate);

    //-----------
    // Corresponds to OCD "checkin()".
    void checkIn(const std::string& vehiclePlate);
}

#endif // RESERVATION_H