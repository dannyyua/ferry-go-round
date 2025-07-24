// Utility.cpp
//*******************************
// Utility.cpp
//
// Utility module that provides common functions for file handling and data management.
//
// Rev 1.1 - 2025-07-23 - Moved all template code to header.
// Rev 1.0 - 2025-07-22 - Initial version
//*******************************

#include "Utility.h"
#include <iostream>
#include <filesystem> // Required for creating a directory

namespace Utility {

    // This function's job is to prepare the environment.
    void init() {
        // Create a "Data" directory if it doesn't already exist.
        // This prevents errors when the program tries to create files inside it.
        if (!std::filesystem::exists("Data")) {
            std::filesystem::create_directory("Data");
        }
        std::cout << "UTILITY: File system initialized. Data directory is ready." << std::endl;
    }

    void shutdown() {
        // In this new design, files are opened and closed per-operation,
        // so there are no global file handles to close. This function is
        // kept for architectural consistency.
        std::cout << "UTILITY: System shutdown." << std::endl;
    }

} // end namespace Utility