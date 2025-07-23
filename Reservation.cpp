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
// *)
//******************************************************************

#include "Reservation.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <io.h>  // For _chsize_s on Windows
#else
#include <unistd.h> // For truncate on POSIX systems
#endif

using namespace std;

#pragma pack(push, 1) // Ensure no padding between struct members
struct ReservationRecord {
    char sailingID[21];        // Fixed-length 20 chars + null terminator
    char vehiclePlate[21];     // Fixed-length 20 chars + null terminator
    bool checkedIn;            // 1 byte
};
#pragma pack(pop) // Restore default packing

const char* RESERVATION_FILE = "reservations.dat";
const int RECORD_SIZE = sizeof(ReservationRecord);

static fstream resFile; // Persistent file handle

// Helper function to find record position
static streampos findRecordPosition(const string& sailingID, const string& vehiclePlate) {
    ReservationRecord rec;
    resFile.seekg(0, ios::beg);
    
    while (resFile.read(reinterpret_cast<char*>(&rec), RECORD_SIZE)) {
        if (strcmp(rec.sailingID, sailingID.c_str()) == 0 && 
            strcmp(rec.vehiclePlate, vehiclePlate.c_str()) == 0) {
            return resFile.tellg() - static_cast<streamoff>(RECORD_SIZE);
        }
    }
    return -1; // Not found
}

// Cross-platform file truncation helper
static void truncateFile(const char* filename, streampos size) {
    #ifdef _WIN32
        // Windows implementation
        FILE* file = fopen(filename, "rb+");
        if (file) {
            int fd = _fileno(file);
            if (_chsize_s(fd, size) != 0) {
                perror("File truncation failed");
            }
            fclose(file);
        }
    #else
        // POSIX implementation
        if (truncate(filename, size) != 0) {
            perror("File truncation failed");
        }
    #endif
}

void Reservation::init() {
    // PURPOSE: Open persistent connection to reservation data file
    // Opens or creates file in binary read/write mode
    resFile.open(RESERVATION_FILE, 
                ios::in | ios::out | ios::binary | ios::app);
    
    // If file doesn't exist, create it
    if (!resFile) {
        resFile.clear();
        resFile.open(RESERVATION_FILE, ios::out | ios::binary);
        resFile.close();
        resFile.open(RESERVATION_FILE, 
                    ios::in | ios::out | ios::binary);
    }
    
    // Ensure read/write position at start
    resFile.seekg(0, ios::beg);
    resFile.seekp(0, ios::beg);
}

void Reservation::shutdown() {
    // PURPOSE: Close persistent file connection
    if (resFile.is_open()) {
        resFile.close();
    }
}

void Reservation::createReservation(const string& reservationInfo) {
    // PURPOSE: Add new reservation record to end of file
    // Parse reservation info (simplified example)
    ReservationRecord newRec;
    memset(&newRec, 0, RECORD_SIZE);
    
    // Example parsing - in real code use proper parsing
    size_t pos = reservationInfo.find(',');
    string sailID = reservationInfo.substr(0, pos);
    string plate = reservationInfo.substr(pos + 1);
    
    strncpy(newRec.sailingID, sailID.c_str(), 20);
    strncpy(newRec.vehiclePlate, plate.c_str(), 20);
    newRec.checkedIn = false;
    
    // Append to end of file
    resFile.seekp(0, ios::end);
    resFile.write(reinterpret_cast<const char*>(&newRec), RECORD_SIZE);
    resFile.flush(); // Ensure immediate persistence
}

void Reservation::cancelReservation(const string& sailingID, const string& vehiclePlate) {
    // PURPOSE: Delete reservation using efficient swap-and-truncate method
    streampos pos = findRecordPosition(sailingID, vehiclePlate);
    if (pos == -1) return; // Record not found
    
    // Get last record in file
    resFile.seekg(0, ios::end);
    streampos endPos = resFile.tellg();
    int numRecords = endPos / RECORD_SIZE;
    
    if (numRecords == 0) return;
    
    // If deleting last record, just truncate file
    if (pos == endPos - RECORD_SIZE) {
        resFile.close();
        truncateFile(RESERVATION_FILE, pos);
        resFile.open(RESERVATION_FILE, 
                    ios::in | ios::out | ios::binary);
        return;
    }
    
    // Read last record
    ReservationRecord lastRec;
    resFile.seekg(-RECORD_SIZE, ios::end);
    resFile.read(reinterpret_cast<char*>(&lastRec), RECORD_SIZE);
    
    // Overwrite target record with last record
    resFile.seekp(pos, ios::beg);
    resFile.write(reinterpret_cast<const char*>(&lastRec), RECORD_SIZE);
    
    // Truncate file by one record
    resFile.close();
    truncateFile(RESERVATION_FILE, endPos - RECORD_SIZE);
    resFile.open(RESERVATION_FILE, 
                ios::in | ios::out | ios::binary);
}

void Reservation::deleteReservations(const string& sailingID) {
    // PURPOSE: Bulk delete reservations by sailing ID
    vector<ReservationRecord> validRecords;
    ReservationRecord rec;
    
    // Collect all records except those matching sailingID
    resFile.seekg(0, ios::beg);
    while (resFile.read(reinterpret_cast<char*>(&rec), RECORD_SIZE)) {
        if (strcmp(rec.sailingID, sailingID.c_str()) != 0) {
            validRecords.push_back(rec);
        }
    }
    
    // Rewrite file with filtered records
    resFile.close();
    resFile.open(RESERVATION_FILE, 
                ios::out | ios::binary | ios::trunc);
    
    for (const auto& record : validRecords) {
        resFile.write(reinterpret_cast<const char*>(&record), RECORD_SIZE);
    }
    resFile.flush();
}

bool Reservation::isValidReservation(const string& vehiclePlate) {
    // PURPOSE: Check if vehicle has existing reservation
    ReservationRecord rec;
    resFile.seekg(0, ios::beg);
    
    while (resFile.read(reinterpret_cast<char*>(&rec), RECORD_SIZE)) {
        if (strcmp(rec.vehiclePlate, vehiclePlate.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

void Reservation::checkIn(const string& vehiclePlate) {
    // PURPOSE: Update reservation status to checked-in
    ReservationRecord rec;
    resFile.seekg(0, ios::beg);
    
    while (resFile.read(reinterpret_cast<char*>(&rec), RECORD_SIZE)) {
        if (strcmp(rec.vehiclePlate, vehiclePlate.c_str()) == 0) {
            // Update checked-in status
            rec.checkedIn = true;
            resFile.seekp(-RECORD_SIZE, ios::cur);
            resFile.write(reinterpret_cast<const char*>(&rec), RECORD_SIZE);
            resFile.flush();
            return;
        }
    }
}
