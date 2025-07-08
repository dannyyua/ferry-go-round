// main.cpp
//******************************************************************
// PROGRAM:            Ferry-Go-Round System
//
// PURPOSE:            The main entry point for the application. It follows the
//                     start-up and shutdown sequence defined in the Object
//                     Collaboration Diagrams (OCDs) by initializing and shutting
//                     down the central controller.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07 - Generated for Group 15
// *)
//******************************************************************

#include "Controller.h"
#include "UserInterface.h"
#include <iostream>

int main() {
    std::cout << "System Starting Up..." << std::endl;
    
    // OCD Start-up, step 1: Main initialises the controller.
    // The controller is responsible for initializing all other subsystems.
    Controller::init();
    
    // OCD Start-up, step 8: Main begins the input loop within the UI.
    // The program will now wait here until the user decides to exit.
    UserInterface::begin_input();
    
    // OCD Shutdown, step 1: The UI has returned control to main().
    // Now, main() tells the controller to begin the shutdown sequence.
    Controller::shutdown();
    
    std::cout << "\nSystem Shutdown Complete. Goodbye!" << std::endl;
    
    return 0; // Indicates successful execution
}