//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*******************************
// Controller.cpp
//
// Central controller that coordinates function calls to the lower-level modules
// Is triggered mainly from UserInterface.cpp
//
// Rev 1.0 - 2025-07-22
//     - Initial version
//*******************************

#include "Controller.h"
#include "Utility.h"

namespace Controller {

    // --- System Lifecycle Functions (from Start-up/Shutdown OCDs) ---
    void Controller::init() {
        Sailing::init();
        Vessel::init();
        Vehicle::init();
        Reservation::init();
        Utility::init();
    }

    void Controller::shutdown() {
        Sailing::shutdown();
        Vessel::shutdown();
        Vehicle::shutdown();
        Reservation::shutdown();
        Utility::shutdown();
    }

    // --- Validation/Check Functions (Called by UI before other actions) ---
    bool Controller::checkVesselExists(const std::string& vesselID) {
        return Vessel::isValidVessel(vesselID);
    }

    bool Controller::checkSailingExists(const std::string& sailingID) {
        return Sailing::isValidSailing(sailingID);
    }

    bool Controller::checkReservationExists(const std::string& vehiclePlate) {
        return Reservation::isValidReservation(vehiclePlate);
    }

    bool Controller::checkVehicleExists(const std::string& vehiclePlate) {
        return Vehicle::isValidVehicle(vehiclePlate);
    }

    // --- Data Retrieval Functions (for displaying info in the UI) ---
    Vessel::VesselEntity getVessel(const std::string& vesselID) {
        return Vessel::getVessel(vesselID);
    }

    Sailing::SailingEntity getSailing(const std::string& sailingID) {
        return Sailing::getSailing(sailingID);
    }

    Reservation::ReservationEntity getReservation(const std::string& vehiclePlate) {
        return Reservation::getReservation(vehiclePlate);
    }

    Vehicle::VehicleEntity getVehicle(const std::string& vehiclePlate) {
        return Vehicle::getVehicle(vehiclePlate);
    }

    // --- Use Case Functions (from specific OCDs) ---
    void Controller::createNewVessel(const std::string& vesselID, double LCLL, double HCLL) {
        Vessel::createVessel(vesselID, LCLL, HCLL);
    }

    void Controller::createNewSailing(const std::string& vesselID, const std::string& sailingID) {
        Sailing::createSailing(vesselID, sailingID);
    }

    void Controller::createNewReservation(const std::string& sailingID, const std::string& vehiclePlate) {
        auto vehicle = getVehicle(vehiclePlate);

        //TODO: figure out HRL and LRL stuff

        Reservation::createReservation(sailingID, vehiclePlate);
    }

    void Controller::createNewVehicle(const std::string& vehiclePlate, const std::string& phoneNumber, double length, double height) {
        Vehicle::createVehicle(vehiclePlate, phoneNumber, length, height);
    }

    void Controller::cancelReservation(const std::string& sailingID, const std::string& vehiclePlate) {
        //TODO: figure out HRL and LRL stuff
    }

    void Controller::checkInVehicle(const std::string& vehiclePlate) {
        //TODO: figure out HRL and LRL stuff
    }

    void Controller::deleteSailing(const std::string& sailingID) {
        Sailing::deleteSailing(sailingID);
        Reservation::deleteReservations(sailingID);
    }
    
    
    // --- Query and Report Functions ---
    std::vector<Sailing::SailingEntity> Controller::getSailingReport(int offset) {
        return Sailing::getSailings(offset);
    }

    Sailing::SailingEntity Controller::queryIndividualSailing(const std::string& sailingID) {
        return getSailing(sailingID); // Might be redundant
    }

    void Controller::printSailingReport() {
        // Might not need this
    }
}