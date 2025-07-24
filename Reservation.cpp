// Reservation.cpp
//******************************************************************
// DEFINITION MODULE: Reservation
//
// PURPOSE: Implements reservation data operations including:
//          - Creating new reservations
//          - Canceling existing reservations
//          - Managing reservations by sailing ID
//          - Validating reservation status
//          - Handling check-in operations
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/22
//   Rev. 1.1 - 2025/07/23
//     - Refactored to use Utility functions for CRUD operations
// *)
//******************************************************************
#include "Reservation.h"
#include <cstring>
#include <vector>

using namespace Reservation;
using namespace Utility;

// No file handles - managed by Utility

void Reservation::init() {}

void Reservation::shutdown() {}

void Reservation::createReservation(const std::string& reservationInfo) {
    // Parse reservation info
    size_t pos = reservationInfo.find(',');
    std::string sailID = reservationInfo.substr(0, pos);
    std::string plate = reservationInfo.substr(pos + 1);
    
    // Create entity
    ReservationEntity newEntity;
    memset(&newEntity, 0, sizeof(ReservationEntity));
    strncpy(newEntity.sailingID, sailID.c_str(), 20);
    strncpy(newEntity.vehiclePlate, plate.c_str(), 20);
    newEntity.checkedIn = false;
    
    // Create record via Utility
    createRecord(newEntity);
}

void Reservation::cancelReservation(const std::string& sailingID, 
                                   const std::string& vehiclePlate) {
    // Create search entity
    ReservationEntity searchEntity;
    memset(&searchEntity, 0, sizeof(ReservationEntity));
    strncpy(searchEntity.sailingID, sailingID.c_str(), 20);
    strncpy(searchEntity.vehiclePlate, vehiclePlate.c_str(), 20);
    
    // Delete via Utility
    deleteRecord(searchEntity);
}

void Reservation::deleteReservations(const std::string& sailingID) {
    // Find all reservations for this sailing
    std::vector<ReservationEntity> toDelete;
    int position = 0;
    
    while (true) {
        auto record = readRecord<ReservationEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->sailingID, sailingID.c_str()) == 0) {
            toDelete.push_back(*record);
        }
        position++;
    }
    
    // Delete found records
    for (auto& entity : toDelete) {
        deleteRecord(entity);
    }
}

bool Reservation::isValidReservation(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<ReservationEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->vehiclePlate, vehiclePlate.c_str()) == 0) {
            return true;
        }
        position++;
    }
    return false;
}

void Reservation::checkIn(const std::string& vehiclePlate) {
    int position = 0;
    while (true) {
        auto record = readRecord<ReservationEntity>(position);
        if (!record.has_value()) break;
        
        if (strcmp(record->vehiclePlate, vehiclePlate.c_str()) == 0) {
            // Update and save
            ReservationEntity updated = *record;
            updated.checkedIn = true;
            updateRecord(updated);
            return;
        }
        position++;
    }
}