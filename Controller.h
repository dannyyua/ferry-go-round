// Controller.h
//******************************************************************
// DEFINITION MODULE: Controller
//
// PURPOSE:          Acts as the central controller in a layered architecture.
//                   It receives requests from the UserInterface, processes them,
//                   enforces business rules, and orchestrates calls to the
//                   data model layer (Sailing, Vessel, etc.). This is the
//                   declaration of the controller's public interface.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
//   Rev. 1.1 - 2025/07/08 
//      - Add validation functions
//   Rev. 1.2 - 2025/07/22
//      - Add data retrieval functions, update some function parameters
// *)
//******************************************************************
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include "Sailing.h"
#include "Vessel.h"
#include "Vehicle.h"
#include "Reservation.h"

// The Controller namespace encapsulates all central application logic,
// making it a global, static class that doesn't need to be instantiated.
namespace Controller {

    // --- System Lifecycle Functions (from Start-up/Shutdown OCDs) ---
    //-----------
    void init();    // Initializes all lower-level modules and connects to the DB.
    //-----------
    void shutdown(); // Shuts down all lower-level modules and disconnects from the DB.

    // --- Validation/Check Functions (Called by UI before other actions) ---
    //-----------
    bool checkVesselExists(const std::string& vesselID);
    //-----------
    bool checkSailingExists(const std::string& sailingID);
    //-----------
    bool checkReservationExists(const std::string& vehiclePlate);
    //-----------
    bool checkVehicleExists(const std::string& vehiclePlate);

    // --- Data Retrieval Functions (for displaying info in the UI) ---
    //-----------
    std::optional<Vessel::VesselEntity> getVessel(const std::string& vesselID);
    //-----------
    std::optional<Sailing::SailingEntity> getSailing(const std::string& sailingID);
    //-----------
    std::optional<Reservation::ReservationEntity> getReservation(const std::string& vehiclePlate);
    //-----------
    std::optional<Vehicle::VehicleEntity> getVehicle(const std::string& vehiclePlate);

    // --- Use Case Functions (from specific OCDs) ---
    //-----------
    void createNewVessel(const std::string& vesselID, double LCLL, double HCLL);
    //-----------
    void createNewSailing(const std::string& vesselID, const std::string& sailingID);
    //-----------
    void createNewReservation(const std::string& sailingID, const std::string& vehiclePlate);
    //-----------
    void createNewVehicle(const std::string& vehiclePlate, const std::string& phoneNumber, double length, double height);
    //-----------
    void cancelReservation(const std::string& sailingID, const std::string& vehiclePlate);
    //-----------
    void checkInVehicle(const std::string& vehiclePlate);
    //-----------
    void deleteSailing(const std::string& sailingID);
    
    
    // --- Query and Report Functions ---
    //-----------
    std::vector<Sailing::SailingEntity> getSailingReport(int offset);
    //-----------
    Sailing::SailingEntity queryIndividualSailing(const std::string& sailingID);
}

#endif // CONTROLLER_H
