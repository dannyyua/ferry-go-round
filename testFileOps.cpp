// testFileOps.cpp
//******************************************************************
// UNIT TEST DRIVER: Low-Level File Operations for Sailing Module
//
// PURPOSE:          This program is the first of two required unit tests for
//                   Assignment #4. It tests the low-level file write and
//                   read capabilities of the Sailing data module.
//
// MODULE TESTED:    Sailing (.h/.cpp)
// DEPENDENCIES:     Vessel (.h/.cpp) for vessel validation, and Utility (.h/.cpp)
// FUNCTIONS TESTED: Sailing::createSailing(), Sailing::getSailing(),
//                   Sailing::isValidSailing(), Sailing::deleteSailing()
//
// TEST STRATEGY:    This is a bottom-up, glass-box test that verifies the
//                   Sailing module's persistence logic by:
//                   1. Creating a prerequisite Vessel record.
//                   2. Writing two distinct Sailing records to a clean file.
//                   3. Reading them back and programmatically verifying their content.
//                   4. Deleting one record and confirming its removal.
//                   5. Printing a final "Pass" or "Fail" summary.
//
// HOW TO COMPILE:
// g++ -std=c++17 -Wall testFileOps.cpp Sailing.cpp Vessel.cpp Utility.cpp -o run_sailing_file_test
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/23 - Written by Person B for A4
// *)
//******************************************************************

#include "Sailing.h"   // The module we are testing
#include "Vessel.h"    // A dependency for creating a sailing
#include "Utility.h"   // A dependency for file I/O
#include <iostream>
#include <filesystem>
#include <cstring>

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
    std::cout << "===== Unit Test 1: Sailing Module File I/O Operations =====" << std::endl;
    bool allTestsPassed = true;

    // --- SETUP ---
    std::cout << "\n[SETUP] Preparing a clean test environment..." << std::endl;
    // Clear the data files to ensure the test is repeatable
    std::filesystem::remove("Data/Vessels.dat");
    std::filesystem::remove("Data/Sailings.dat");

    Utility::init();
    Vessel::init();
    Sailing::init();

    // Pre-condition: A vessel MUST exist before we can create a sailing for it.
    Vessel::createVessel("QUEEN", 400.0, 200.0);

    // --- TEST DATA ---
    const std::string sailing1_id = "YVR-01";
    const std::string sailing2_id = "TSA-02";
    const std::string vessel_id = "QUEEN";

    // --- TEST CASE 1: CREATE & READ ---
    std::cout << "\n[TEST CASE 1] Writing a record and reading it back..." << std::endl;
    Sailing::createSailing(vessel_id, sailing1_id);
    auto s1_opt = Sailing::getSailing(sailing1_id);
    allTestsPassed &= check(s1_opt.has_value(), "Record 1 can be found after creation");
    if (s1_opt) {
        allTestsPassed &= check(strcmp(s1_opt->sailingID, sailing1_id.c_str()) == 0, "Record 1 has correct ID");
        allTestsPassed &= check(strcmp(s1_opt->vesselID, vessel_id.c_str()) == 0, "Record 1 has correct Vessel ID");
        // Check if initial capacity was set correctly from the Vessel
        allTestsPassed &= check(s1_opt->LRL == 400.0, "Record 1 has correct initial LRL from Vessel");
    }

    // --- TEST CASE 2: WRITE a second record and verify ---
    std::cout << "\n[TEST CASE 2] Writing and verifying a second record..." << std::endl;
    Sailing::createSailing(vessel_id, sailing2_id);
    auto s2_opt = Sailing::getSailing(sailing2_id);
    allTestsPassed &= check(s2_opt.has_value(), "Record 2 can be found");

    // --- TEST CASE 3: DELETE a record and verify ---
    std::cout << "\n[TEST CASE 3] Deleting a record and verifying removal..." << std::endl;
    Sailing::deleteSailing(sailing1_id);
    allTestsPassed &= check(!Sailing::isValidSailing(sailing1_id), "Deleted record '" + sailing1_id + "' is no longer valid");
    allTestsPassed &= check(Sailing::isValidSailing(sailing2_id), "Other record '" + sailing2_id + "' still exists after deletion");
    
    // --- TEST CASE 4: VERIFY END OF FILE ---
    // After deleting one of two records, there should only be one record left.
    // Trying to read at position 1 should fail.
    std::cout << "\n[TEST CASE 4] Verifying file was truncated after delete..." << std::endl;
    auto record_at_pos1 = Utility::readRecord<Sailing::SailingEntity>(1);
    allTestsPassed &= check(!record_at_pos1.has_value(), "Reading at position 1 correctly fails after deletion");

    // --- SHUTDOWN ---
    Sailing::shutdown();
    Vessel::shutdown();
    Utility::shutdown();

    // --- FINAL VERDICT ---
    std::cout << "\n----------------------------------------------------" << std::endl;
    if (allTestsPassed) {
        std::cout << "Final Result: Sailing file operations test: Pass" << std::endl;
    } else {
        std::cout << "Final Result: Sailing file operations test: Fail" << std::endl;
    }
    std::cout << "----------------------------------------------------" << std::endl;

    return 0;
}