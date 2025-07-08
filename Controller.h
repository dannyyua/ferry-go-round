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
// *)
//******************************************************************
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

// The Controller namespace encapsulates all central application logic,
// making it a global, static class that doesn't need to be instantiated.
namespace Controller {

    // --- System Lifecycle Functions (from Start-up/Shutdown OCDs) ---
    //-----------
    void initialize();    // Initializes all lower-level modules and connects to the DB.
    //-----------
    void shutdown(); // Shuts down all lower-level modules and disconnects from the DB.


    // --- Use Case Functions (from specific OCDs) ---
    //-----------
    void createNewVessel(const std::string& vesselInfo);
    //-----------
    void createNewSailing(const std::string& vesselID, const std::string& sailingInfo);
    //-----------
    void createNewReservation(const std::string& sailingID, const std::string& vehiclePlate, const std::string& customerPhone);
    //-----------
    void cancelReservation(const std::string& sailingID, const std::string& vehiclePlate);
    //-----------
    void checkInVehicle(const std::string& vehiclePlate);
    //-----------
    void deleteSailing(const std::string& sailingID);
    
    
    // --- Query and Report Functions ---
    //-----------
    void getSailingReport(); // Simplified to just trigger the report process
    //-----------
    void queryIndividualSailing(const std::string& sailingID);
    //-----------
    void printSailingReport();
}

#endif // CONTROLLER_H
