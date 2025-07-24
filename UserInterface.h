// UserInterface.h
//******************************************************************
// DEFINITION MODULE: UserInterface
//
// PURPOSE:          The View layer of the application. It is responsible
//                   for displaying menus, prompting the user for input, and
//                   delegating all application logic to the Controller.
//                   This header declares the single function needed to
//                   start the interface.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
// *)
//******************************************************************
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Controller.h"

namespace UserInterface {

    //-----------
    // Begins the main input loop for the user. This function will
    // continuously display prompts and listen for commands until the
    // user types 'exit'. It acts as the primary entry point into the
    // user-facing part of the application, as per the OCDs.
    void begin_input();

}

#endif // USER_INTERFACE_H
