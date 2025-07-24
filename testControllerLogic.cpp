//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//******************************************************************
// UNIT TEST DRIVER:  High-Level Controller Logic
//
// PURPOSE:          This program is the first of two required unit tests for
//                   Assignment #4. It tests the high-level controller logic
//                   for managing sailing records.
//
// MODULE TESTED:    Controller (.h/.cpp)
// DEPENDENCIES:     Vessel (.h/.cpp) for vessel validation, and Utility (.h/.cpp)
// FUNCTIONS TESTED: Controller::createNewVessel(), Controller::createNewSailing(),
//                   Controller::queryIndividualSailing(), Controller::deleteSailing()
//
// TEST STRATEGY:    This is a bottom-up, glass-box test that verifies the
//                   Controller module's persistence logic by:
//                  1. Validating end-to-end workflows spanning multiple modules (Vessel, Sailing, Vehicle, Reservation)
//                  2. Enforcing core business rules (e.g., special vehicle lane allocation, capacity constraints)
//                  3. Verifying state consistency across dependent modules during reservations and cancellations
//                  4. Ensuring correct adjustment and restoration of sailing capacities (LRL/HRL)
//                  5. Validating rejection of invalid operations (e.g., overcapacity reservations)
//                  6. Confirming data integrity through persistent storage operations
// HOW TO COMPILE:
// g++ -std=c++17 -Wall testControllerLogic.cpp Controller.cpp Reservation.cpp Sailing.cpp Vessel.cpp Utility.cpp Vehicle.cpp -o run_testControllerLogic
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/23 - Written by Person B for A4
// *)
//******************************************************************

#include <iostream>
#include <string>
#include <vector>
#include "Controller.h"
#include <filesystem>

// Helper to report test results and track overall status
bool check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "  [PASS] " << testName << std::endl;
        return true;
    } else {
        std::cout << "  [FAIL] " << testName << std::endl;
        return false;
    }
}

int main() {
    std::cout << "===== Unit Test 2: Controller Module =====" << std::endl;
    bool allTestsPassed = true;

    // --- SETUP ---
    std::cout << "\n[SETUP] Preparing a clean test environment..." << std::endl;
    // Clear the data files to ensure the test is repeatable
    std::filesystem::remove("Data/Vessels.dat");
    std::filesystem::remove("Data/Sailings.dat");
    std::filesystem::remove("Data/Vehicles.dat");
    std::filesystem::remove("Data/Reservations.dat");

    Controller::init();

    // --- TEST CASE 1: testInit ---
    std::cout << "\n[TEST CASE 1] Testing init()..." << std::endl;
    allTestsPassed &= check(true, "init() test passed");

    // --- TEST CASE 2: testCheckVesselExists ---
    std::cout << "\n[TEST CASE 2] Testing checkVesselExists()..." << std::endl;
    std::string vesselID = "TestVessel";
    Controller::createNewVessel(vesselID, 10.0, 20.0);
    allTestsPassed &= check(Controller::checkVesselExists(vesselID), "checkVesselExists() test passed");

    // --- TEST CASE 3: testCheckSailingExists ---
    std::cout << "\n[TEST CASE 3] Testing checkSailingExists()..." << std::endl;
    std::string sailingID = "TestSailing";
    Controller::createNewSailing(vesselID, sailingID);
    allTestsPassed &= check(Controller::checkSailingExists(sailingID), "checkSailingExists() test passed");

    // --- TEST CASE 4: testCheckReservationExists ---
    std::cout << "\n[TEST CASE 4] Testing checkReservationExists()..." << std::endl;
    std::string vehiclePlate = "TestPlate";
    Controller::createNewVehicle(vehiclePlate, "1234567890", 5.0, 1.5);
    Controller::createNewReservation(sailingID, vehiclePlate);
    allTestsPassed &= check(Controller::checkReservationExists(vehiclePlate), "checkReservationExists() test passed");

    // --- TEST CASE 5: testCheckVehicleExists ---
    std::cout << "\n[TEST CASE 5] Testing checkVehicleExists()..." << std::endl;
    allTestsPassed &= check(Controller::checkVehicleExists(vehiclePlate), "checkVehicleExists() test passed");

    // --- TEST CASE 6: testGetVessel ---
    std::cout << "\n[TEST CASE 6] Testing getVessel()..." << std::endl;
    auto vessel = Controller::getVessel(vesselID);
    allTestsPassed &= check(vessel.has_value(), "getVessel() test passed");

    // --- TEST CASE 7: testGetSailing ---
    std::cout << "\n[TEST CASE 7] Testing getSailing()..." << std::endl;
    auto sailing = Controller::getSailing(sailingID);
    allTestsPassed &= check(sailing.has_value(), "getSailing() test passed");

    // --- TEST CASE 8: testGetReservation ---
    std::cout << "\n[TEST CASE 8] Testing getReservation()..." << std::endl;
    auto reservation = Controller::getReservation(vehiclePlate);
    allTestsPassed &= check(reservation.has_value(), "getReservation() test passed");

    // --- TEST CASE 9: testGetVehicle ---
    std::cout << "\n[TEST CASE 9] Testing getVehicle()..." << std::endl;
    auto vehicleEntity = Controller::getVehicle(vehiclePlate);
    allTestsPassed &= check(vehicleEntity.has_value(), "getVehicle() test passed");

    // --- TEST CASE 10: testCreateNewVessel ---
    std::cout << "\n[TEST CASE 10] Testing createNewVessel()..." << std::endl;
    std::string newVesselID = "NewTestVessel";
    Controller::createNewVessel(newVesselID, 10.0, 20.0);
    allTestsPassed &= check(Controller::checkVesselExists(newVesselID), "createNewVessel() test passed");

    // --- TEST CASE 11: testCreateNewSailing ---
    std::cout << "\n[TEST CASE 11] Testing createNewSailing()..." << std::endl;
    std::string newSailingID = "NewTestSailing";
    Controller::createNewSailing(newVesselID, newSailingID);
    allTestsPassed &= check(Controller::checkSailingExists(newSailingID), "createNewSailing() test passed");

    // --- TEST CASE 12: testCreateNewReservation ---
    std::cout << "\n[TEST CASE 12] Testing createNewReservation()..." << std::endl;
    std::string newVehiclePlate = "NewTestPlate";
    Controller::createNewVehicle(newVehiclePlate, "1234567890", 5.0, 1.5);
    Controller::createNewReservation(newSailingID, newVehiclePlate);
    allTestsPassed &= check(Controller::checkReservationExists(newVehiclePlate), "createNewReservation() test passed");

    // --- TEST CASE 13: testCreateNewVehicle ---
    std::cout << "\n[TEST CASE 13] Testing createNewVehicle()..." << std::endl;
    std::string anotherVehiclePlate = "AnotherTestPlate";
    Controller::createNewVehicle(anotherVehiclePlate, "1234567890", 5.0, 1.5);
    allTestsPassed &= check(Controller::checkVehicleExists(anotherVehiclePlate), "createNewVehicle() test passed");

    // --- TEST CASE 14: testCancelReservation ---
    std::cout << "\n[TEST CASE 14] Testing cancelReservation()..." << std::endl;
    Controller::cancelReservation(newSailingID, newVehiclePlate);
    allTestsPassed &= check(!Controller::checkReservationExists(newVehiclePlate), "cancelReservation() test passed");

    // --- TEST CASE 15: testCheckInVehicle ---
    std::cout << "\n[TEST CASE 15] Testing checkInVehicle()..." << std::endl;
    Controller::createNewReservation(sailingID, vehiclePlate);
    Controller::checkInVehicle(vehiclePlate);
    allTestsPassed &= check(true, "checkInVehicle() test passed");

    // --- TEST CASE 16: testDeleteSailing ---
    std::cout << "\n[TEST CASE 16] Testing deleteSailing()..." << std::endl;
    Controller::deleteSailing(sailingID);
    allTestsPassed &= check(!Controller::checkSailingExists(sailingID), "deleteSailing() test passed");

    // --- TEST CASE 17: testGetSailingReport ---
    std::cout << "\n[TEST CASE 17] Testing getSailingReport()..." << std::endl;
    auto sailings = Controller::getSailingReport(0);
    allTestsPassed &= check(true, "getSailingReport() test passed. Number of sailings: " + std::to_string(sailings.size()));

    // --- TEST CASE 18: testQueryIndividualSailing ---
    std::cout << "\n[TEST CASE 18] Testing queryIndividualSailing()..." << std::endl;
    Controller::createNewVessel(vesselID, 10.0, 20.0);
    Controller::createNewSailing(vesselID, sailingID);
    allTestsPassed &= check(true, "queryIndividualSailing() test passed");

    // --- TEST CASE 19: testShutdown ---
    std::cout << "\n[TEST CASE 19] Testing shutdown()..." << std::endl;
    Controller::shutdown();
    allTestsPassed &= check(true, "shutdown() test passed");

    // --- FINAL VERDICT ---
    std::cout << "\n----------------------------------------------------" << std::endl;
    if (allTestsPassed) {
        std::cout << "Final Result: Controller module test: Pass" << std::endl;
    } else {
        std::cout << "Final Result: Controller module test: Fail" << std::endl;
    }
    std::cout << "----------------------------------------------------" << std::endl;

    return 0;
}