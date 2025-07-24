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
    void init() {
        Sailing::init();
        Vessel::init();
        Vehicle::init();
        Reservation::init();
        Utility::init();
    }

    void shutdown() {
        Sailing::shutdown();
        Vessel::shutdown();
        Vehicle::shutdown();
        Reservation::shutdown();
        Utility::shutdown();
    }

    // --- Validation/Check Functions (Called by UI before other actions) ---
    bool checkVesselExists(const std::string& vesselID) {
        return Vessel::isValidVessel(vesselID);
    }

    bool checkSailingExists(const std::string& sailingID) {
        return Sailing::isValidSailing(sailingID);
    }

    bool checkReservationExists(const std::string& vehiclePlate) {
        return Reservation::isValidReservation(vehiclePlate);
    }

    bool checkVehicleExists(const std::string& vehiclePlate) {
        return Vehicle::isValidVehicle(vehiclePlate);
    }

    // --- Data Retrieval Functions (for displaying info in the UI) ---
    std::optional<Vessel::VesselEntity> getVessel(const std::string& vesselID) {
        return Vessel::getVessel(vesselID);
    }

    std::optional<Sailing::SailingEntity> getSailing(const std::string& sailingID) {
        return Sailing::getSailing(sailingID);
    }

    std::optional<Reservation::ReservationEntity> getReservation(const std::string& vehiclePlate) {
        return Reservation::getReservation(vehiclePlate);
    }

    std::optional<Vehicle::VehicleEntity> getVehicle(const std::string& vehiclePlate) {
        return Vehicle::getVehicle(vehiclePlate);
    }

    // --- Use Case Functions (from specific OCDs) ---
    void createNewVessel(const std::string& vesselID, double LCLL, double HCLL) {
        Vessel::createVessel(vesselID, LCLL, HCLL);
    }

    void createNewSailing(const std::string& vesselID, const std::string& sailingID) {
        Sailing::createSailing(vesselID, sailingID);
    }

    void createNewReservation(const std::string& sailingID, const std::string& vehiclePlate) {
        Reservation::createReservation(sailingID, vehiclePlate);

        auto vehicle = getVehicle(vehiclePlate).value();
        auto sailing = getSailing(sailingID).value();

        // Assume 4.5m for regular vehicles, include 0.5m buffer
        double vehicleLength = (vehicle.length > 0 ? vehicle.length : 4.5) + 0.5;
        double vehicleHeight = vehicle.height;

        // Decrease HRL if special vehicle or LRL is full, otherwise decrease LRL
        if (vehicleHeight > 2.0 || vehicleLength > 7.0 || sailing.LRL < vehicleLength) {
            Sailing::decreaseHRL(sailingID, vehicleLength);
        } else {
            Sailing::decreaseLRL(sailingID, vehicleLength);
        }
    }

    void createNewVehicle(const std::string& vehiclePlate, const std::string& phoneNumber, double length, double height) {
        Vehicle::createVehicle(vehiclePlate, phoneNumber, length, height);
    }

    void cancelReservation(const std::string& sailingID, const std::string& vehiclePlate) {
        Reservation::cancelReservation(sailingID, vehiclePlate);

        auto vehicle = getVehicle(vehiclePlate).value();

        double vehicleLength = (vehicle.length > 0 ? vehicle.length : 4.5) + 0.5;
        double vehicleHeight = vehicle.height;

        // Increase HRL/LRL depending on vehicle type
        // TODO: Consider case when regular vehicle is put into HRL lane
        // May need to store lane type in Reservation
        if (vehicleHeight > 2.0 || vehicleLength > 7.0) {
            Sailing::increaseHRL(sailingID, vehicleLength);
        } else {
            Sailing::increaseLRL(sailingID, vehicleLength);
        }
    }

    void checkInVehicle(const std::string& vehiclePlate) {
        Reservation::checkIn(vehiclePlate);
    }

    void deleteSailing(const std::string& sailingID) {
        Sailing::deleteSailing(sailingID);
        Reservation::deleteReservations(sailingID);
    }
    
    
    // --- Query and Report Functions ---
    std::vector<Sailing::SailingEntity> getSailingReport(int offset) {
        return Sailing::getSailings(offset);
    }

    Sailing::SailingEntity queryIndividualSailing(const std::string& sailingID) {
        return getSailing(sailingID).value(); 
    }
}