//*******************************
// main.cpp
// Ferry System Main Controller
// Rev 1.0 2025-07-07 
//   - Initial implementation
// Rev 1.1 2025-07-08 
//   - Fixed function names, remove printer::shutdown()
//*******************************

#include <iostream>
#include <string>
#include "UserInterface.h"
#include "Vessel.h"
#include "Sailing.h"
#include "Reservation.h"
#include "Vehicle.h"
#include "Printer.h"
#include "Controller.h"

// Function prototypes
void initializeSystem();
void shutdownSystem();

int main() {
    try {
        // System initialization
        initializeSystem();
        
        // Start main application loop
        UserInterface::begin_input();
        
        // System cleanup
        shutdownSystem();
        
        std::cout << "System shutdown completed successfully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
}

void initializeSystem() {
    std::cout << "Initializing system." << std::endl;
    
    // Initialize all persistent data files
    Vessel::initialize();
    Sailing::initialize();
    Reservation::initialize();
    Vehicle::initialize();
    Controller::initialize();
    
    
    std::cout << "System initialization complete." << std::endl;
}

void shutdownSystem() {
    std::cout << "Shutting down system." << std::endl;
    
    // Close all data files
    Vehicle::shutdown();
    Reservation::shutdown();
    Sailing::shutdown();
    Vessel::shutdown();
    Controller::shutdown();

    
    std::cout << "System shutdown complete." << std::endl;
}

/* ========================================================================
 * TEAM CODING CONVENTION (Last updated: 2025-07-07)
 * 
 * 1. FILE ORGANIZATION
 *    - Header files (.h) contain only declarations and inline functions
 *    - Implementation files (.cpp) contain all function definitions
 *    - Each module must have its own .h and .cpp pair
 * 
 * 2. FILE HEADER
 *    Each file must start with:
 *    //*******************************
 *    // FileName.h or FileName.cpp
 *    // Brief description
 *    // Version x.y (YYYY-MM-DD)
 *    //*******************************
 * 
 * 3. VERSION HISTORY
 *    - Maintain at top of .cpp files, latest first:
 *    // Rev 1.0 - 2025-07-07
 *    //   - Fixed initialization bug in shutdown sequence
 *    // Rev 1.1 - 2025-07-08 
 *    //   - Initial implementation
 * 
 * 4. FUNCTION DECLARATIONS
 *    - Use //--- separator before each function
 *    - Parameters one per line with direction and units:
 *    //---
 *    float calculateFare(
 *        in    float   length,      // Vehicle length (meters)
 *        in    float   height,      // Vehicle height (meters)
 *        in    bool    isSpecial,   // Vehicle type flag
 *        out   int     &errorCode   // Error code (0 = success)
 *    );
 * 
 * 5. NAMING CONVENTIONS
 *    - Classes: PascalCase (e.g., ReservationManager)
 *    - Functions: camelCase (e.g., createNewReservation)
 *    - Variables: camelCase (e.g., remainingCapacity)
 *    - Constants: ALL_CAPS (e.g., MAX_LICENSE_LENGTH)
 *    - Files: PascalCase (e.g., UserInterface.h)
 * 
 * 6. FORMATTING
 *    - Indentation: 4 spaces (no tabs)
 *    - Braces: Same line for functions/classes, new line for control blocks
 *      void exampleFunction() {
 *          if (condition) 
 *          {
 *              // code
 *          }
 *      }
 * 
 * 7. ERROR HANDLING
 *    - Use return codes for expected errors
 *    - Use exceptions for unrecoverable errors
 *    - Always validate user input
 * 
 * 8. COMMENTING
 *    - Header comments for all functions describing purpose
 *    - Inline comments for complex logic
 *    - Avoid redundant comments (e.g., "increment i")
 * 
 * 9. MEMORY MANAGEMENT
 *    - Prefer stack allocation over heap
 *    - Use smart pointers if dynamic allocation needed
 *    - Always close files after use
 * 
 * 10. SECURITY
 *    - Validate all user inputs
 *    - Sanitize data before file operations
 *    - Use encrypted backups
 * ======================================================================== */
