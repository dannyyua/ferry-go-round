// UserInterface.cpp
//******************************************************************
// IMPLEMENTATION MODULE: UserInterface
//
// PURPOSE: Implements the view layer of the Ferry System application.
//          Displays menus, validates user input, and delegates
//          operations to Controller. Each input step loops locally
//          so retry stays at that step.
// 
// Rev 1.2 - 2025/07/24 Revised function calls for printing
// Rev 1.1 - 2025/07/23 Revised looping logic and UI text now includes expected format
// Rev 1.0 - 2025/07/22 Initial
//******************************************************************

#include "UserInterface.h"
#include "Controller.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

namespace UserInterface {

    // Returns true if input is a valid double and sets `outVal`
    bool tryParseDouble(const string& input, double& outVal) {
        try {
            size_t idx;
            outVal = stod(input, &idx);

            // Entire string must be valid (e.g., "123abc" is not)
            return idx == input.length();
        } catch (...) {
            return false;
        }
    }

    // Vessel Management
    static void handleVesselManagement() {
        while (true) {
            cout<<"\n============== Vessel Management ==============\n"
                <<"1) Create New Vessel Record\n"
                <<"0) Return to main menu\n"
                <<"\nEnter selection ";

            int choice; 
            if (!(cin >> choice)) { 
                cin.clear(); 
                cin.ignore(10000, '\n'); 
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }
            
            if (choice == 0) {
                return;
            }   
            
            if (choice == 1) {
                cout << "\n====== CREATE NEW VESSEL RECORD ======\n";
                
                cout << "\nEnter Vessel ID (max 20 chars) [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                string vesselID;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (Controller::checkVesselExists(userInput)) {
                        cout << "\nError: Vessel ID \"" << userInput << "\" already exists.\n"
                            << "Please enter a different ID or [0 to return to main menu]: ";
                    } else {
                        validInput = true;
                        vesselID = userInput;
                    }
                }
                cout << "\nEnter High Ceiling Lane Length (HCLL) in meters (1 - 3600) [or 0 to return to main menu]: ";
                userInput = "";
                validInput = false;
                double hcll = 0.0;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    } else {
                        bool validDouble = tryParseDouble(userInput, hcll);
                        if (!validDouble) {
                            cout << "\nInvalid HCLL. Please enter a number between 1 and 3600 [or 0 to return to main menu]: ";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                            continue;
                        } else {
                            if (hcll < 1 || hcll > 3600) {
                                cout << "\nInvalid HCLL. Please enter a number between 1 and 3600 [or 0 to return to main menu]: ";
                                continue;
                            } else {
                                validInput = true;
                            }
                        }
                    }
                }
                cout << "\nEnter Low Ceiling Lane Length (LCLL) in meters (1 - 3600) [or 0 to return to main menu]: ";
                userInput = "";
                validInput = false;
                double lcll = 0.0;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    } else {
                        bool validDouble = tryParseDouble(userInput, lcll);
                        if (!validDouble) {
                            cout << "\nInvalid LCLL. Please enter a number between 1 and 3600 [or 0 to return to main menu]: ";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                            continue;
                        } else {
                            if (lcll < 1 || lcll > 3600) {
                                cout << "\nInvalid LCLL. Please enter a number between 1 and 3600 [or 0 to return to main menu]: ";
                                continue;
                            } else {
                                validInput = true;
                            }
                        }
                    }
                }
                cout << "\nYou entered the following:\n"
                    << "\n  Vessel ID: " << vesselID
                    << "\n  HCLL: " << hcll << " m"
                    << "\n  LCLL: " << lcll << " m";
                
                userInput = "";
                validInput = false;
                while (!validInput) {
                    cout << "\nConfirm creation of this vessel? (Y/N): ";
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Restarting vessel creation process...\n";
                        validInput = true;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            Controller::createNewVessel(vesselID, lcll, hcll);
                            cout << "\nNew vessel \"" << vesselID << "\" has been successfully created."
                                << "\nReturning to main menu...\n";
                            validInput = true;
                            return;
                        } else {
                            cout << "\nInvalid choice, please try again...";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
        }
    }

    // Sailing Management
    static void handleSailingManagement() {
        while (true) {
            cout<<"\n============== Sailing Management ==============\n"
                <<"1) Create New Sailing\n"
                <<"2) Delete Sailing\n"
                <<"0) Return to main menu\n"
                <<"\nEnter selection: ";
            
            int choice; 
            if (!(cin >> choice)) { 
                cin.clear(); 
                cin.ignore(10000, '\n'); 
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }
            
            if (choice == 0) {
                return;
            } 
            if (choice == 1) {
                cout << "\n====== CREATE NEW SAILING ======\n";
                
                cout << "\nEnter vessel ID to assign sailings to [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                string vesselID;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (!Controller::checkVesselExists(userInput)) {
                        cout << "\nError: Vessel ID \"" << userInput << "\" does not exist.\n"
                            << "Please enter a valid vessel ID or 0 to return to main menu: ";
                    } else {
                        validInput = true;
                        vesselID = userInput;
                    }
                }
                cout << "\nEnter new sailing ID in format ttt-dd-mm (e.g., YVR-21-06) [or 0 to return to main menu]: ";
                userInput = "";
                validInput = false;
                string sailingID;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (Controller::checkSailingExists(userInput)) {
                        cout << "\nError: sailing ID \"" << userInput << "\" already exists.\n"
                            << "Please enter a valid and unique ID [or 0 to return to main menu]: ";
                    } else {
                        validInput = true;
                        sailingID = userInput;
                    }
                }
                cout << "\nYou entered the following:\n"
                    << "\n  Vessel ID: " << vesselID 
                    << "\n  Sailing ID: " << sailingID << "\n";
                
                userInput = "";
                validInput = false;
                while (!validInput) {
                    cout << "\nConfirm creation of this sailing? (Y/N): ";
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Restarting sailing creation process...\n";
                        validInput = true;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            Controller::createNewSailing(vesselID, sailingID);
                            cout << "\nSailing \"" << sailingID << "\" has been successfully scheduled for vessel \""
                                << vesselID << "\"."
                                << "\nReturning to main menu...\n";
                            validInput = true;
                            return;
                        } else {
                            cout << "\nInvalid choice, please try again...";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
            if (choice == 2) {
                cout << "\n====== Delete Sailing ======\n";
                string userInput;
                bool validInput = false;
                string sailingID;
                cout << "\nEnter sailing ID for deletion [or 0 to return to main menu]: ";
                while(!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (!Controller::checkSailingExists(userInput)) {
                        cout << "\nError: sailing ID " << userInput << " does not exist.\n";
                        cout << "Enter sailing ID for deletion [or 0 to return to main menu]: ";
                    } else {
                        validInput = true;
                        sailingID = userInput;
                    }
                }
                
                userInput = "";
                validInput = false;
                while (!validInput) {
                    cout << "\nYou are about to delete sailing ID: " << sailingID << ". Confirm action? (Y/N): ";
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Restarting deleting process...\n";
                        validInput = true;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            Controller::deleteSailing(sailingID);
                            cout << "\nSailing \"" << sailingID << "\" has been deleted. Reservations associated with this sailing have also been deleted."
                                << "\nReturning to main menu...\n";
                            validInput = true;
                            return;
                        } else {
                            cout << "\nInvalid choice, please try again...\n";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
        }
    }

    // Reservation Management
    static void handleReservationManagement() {
        while (true) {
            cout<<"\n============== Reservation Management ==============\n"
                <<"1) Create New Reservation\n"
                <<"2) Cancel Reservation\n"
                <<"3) Check-In Vehicle\n"
                <<"0) Return to main menu\n"
                <<"\nEnter selection: ";
            
            int choice; 
            if (!(cin >> choice)) {  
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }
            
            if (choice == 0) {
                return;
            } 
            if (choice == 1) {
                cout << "\n====== CREATE NEW RESERVATION ======\n";
                
                cout << "\nEnter Sailing ID [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                string sailingID;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (!Controller::checkSailingExists(userInput)) {
                        cout << "\nError: Sailing ID \"" << userInput << "\" does not exist.\n"
                            << "Please enter a valid Sailing ID [or 0 to return to main menu]: ";
                    } else {
                        validInput = true;
                        sailingID = userInput;
                    }
                }
                cout << "\nEnter vehicle license plate [or 0 to return to main menu]: ";
                userInput = "";
                validInput = false;
                string licensePlate;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    } else {
                        licensePlate = userInput;
                        validInput = true;
                    }
                }
                cout << "\nChecking if vehicle record exists...";
                userInput = "";
                double height;
                double length;
                string heightInput;
                string lengthInput; 
                if (!Controller::checkVehicleExists(licensePlate)) {
                    cout << "\nEnter customer phone number [or 0 to return to main menu]: ";
                    userInput = "";
                    validInput = false;
                    string phoneNumber;
                    while (!validInput) {
                        cin >> userInput;
                        if (userInput == "0") {
                            return;
                        } else {
                            phoneNumber = userInput;
                            validInput = true;
                        }
                    }
                    bool validheight = false;                                             
                    cout << "\nEnter vehicle height in meters (0.1 - 9.9) [or 0 to return to main menu]: ";
                    while (!validheight) {
                        cin >> heightInput;
                        if (!tryParseDouble(heightInput, height)) {
                            cout << "\nInvalid height. Please enter a value between 0.1 and 9.9 [or 0 to return to main menu]: ";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                            continue;
                        } else {
                            if (height < 0.1 || height > 9.9) {
                                cout << "\nInvalid height. Please enter a value between 0.1 and 9.9 [or 0 to return to main menu]: ";
                                continue;
                            } else {
                                validheight = true;
                            }
                        }
                    }
                    bool validlength = false;
                    cout << "\nEnter vehicle length in meters (0.1 - 999.9) [or 0 to return to main menu]: ";
                    while (!validlength) {
                        cin >> lengthInput;
                        if (!tryParseDouble(lengthInput, length)) {
                            cout << "\nInvalid length. Please enter a value between 0.1 and 999.9 [or 0 to return to main menu]: ";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                            continue;
                        } else {
                            if (length < 0.1 || length > 999.9) {
                                cout << "\nInvalid length. Please enter a value between 0.1 and 999.9 [or 0 to return to main menu]: ";
                                continue;
                            } else {
                                validlength = true;
                            }
                        }
                    }
                    Controller::createNewVehicle(licensePlate, phoneNumber, length, height);     
                }
                double lrl = Sailing::getSailing(sailingID).value().LRL;
                double hrl = Sailing::getSailing(sailingID).value().HRL;
                cout << "\nRemaining capacity for sailing " << sailingID << ":"
                     << "\nHRL: " << hrl << " m"
                     << "\nLRL: " << lrl << " m\n";
                cout << "\nChecking availability based on vehicle type and dimensions...\n";
                bool isSpaceAvailable = false;
                bool reservationExists = Controller::checkReservationExists(licensePlate);
                if (length < lrl || length < hrl) {
                    isSpaceAvailable = true;
                }
                if (!reservationExists) {
                    if (isSpaceAvailable) {
                        userInput = "";
                        validInput = false;
                        cout << "\nYou are about to reserve a space for vehicle " << licensePlate 
                             << " on sailing " << sailingID << "."
                             << "\nHeight: " << heightInput << " m"
                             << "\nLength: " << lengthInput << " m\n"
                             << "\nConfirm reservation? (Y/N): ";
                        while (!validInput) {
                            cin >> userInput;
                            if (userInput == "n" || userInput == "N") {
                                cout << "\nOperation cancelled. Restarting reservation process...\n";
                                validInput = true;
                            } else {
                                if (userInput == "y" || userInput == "Y") {
                                    Controller::createNewReservation(sailingID, licensePlate);
                                    double temp = length + 0.5;
                                    cout << "\nReservation confirmed."
                                        << "\n  Total reserved space: " << temp << " m"
                                        << " (" << lengthInput << " m + 0.5 m buffer)\n" 
                                        << "\nReturning to main menu...\n";
                                    validInput = true;
                                    return;
                                } else {
                                    cout << "\nInvalid choice, please try again...\n";
                                    cin.clear(); 
                                    cin.ignore(10000, '\n');
                                }
                            }
                        }
                    } else {
                        cout << "\nError: Not enough space remaining to reserve a spot for this vehicle.";
                        cout << "\nReservation cannot be completed. Restarting reservation process...\n";
                    }
                } else {
                    cout << "\nError: Vehicle already has a reservation.";
                    cout << "\nReservation cannot be completed. Restarting reservation process...\n";
                }   
            }
            if (choice == 2) {
                cout << "\n====== CANCEL RESERVATION ======\n";
                
                cout << "\nEnter Sailing ID [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                string sailingID;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (!Controller::checkSailingExists(userInput)) {
                        cout << "\nError: Sailing ID \"" << userInput << "\" does not exist.\n"
                             << "Please enter a valid Sailing ID [or 0 to return to main menu]: ";
                    } else {
                        validInput = true;
                        sailingID = userInput;
                    }
                }
                cout << "\nEnter vehicle license plate [or 0 to return to main menu]: ";
                userInput = "";
                validInput = false;
                string licensePlate;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    } else {
                        licensePlate = userInput;
                        if (!Controller::checkReservationExists(licensePlate)) {
                            cout << "\nError: No reservation found for vehicle \""
                            << licensePlate << "\" on sailing \"" 
                            << sailingID << "\"."
                            << "\nPlease enter a valid license plate [or 0 to return to main menu]: "; 
                        } else {
                           validInput = true; 
                        }
                    }
                }
                userInput = "";
                validInput = false;
                cout << "\nYou are about to cancel the reservation for vehicle " << licensePlate 
                     << " on sailing " << sailingID << ".\n"
                     << "\nConfirm cancellation? (Y/N): ";
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Restarting reservation cancellation process...\n";
                        validInput = true;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            Controller::deleteSailing(sailingID);
                            cout << "\nReservation for vehicle " << licensePlate <<" has been cancelled."
                                 << "\nRemaining capacity updated.\n" 
                                 << "\nReturning to main menu...\n";
                            validInput = true;
                            return;
                        } else {
                            cout << "\nInvalid choice, please try again...\n";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
            if (choice == 3) {
                cout << "\n============== CHECK-IN VEHICLE ==============\n";
                
                cout << "\nEnter vehicle license plate [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                string licensePlate;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    } else {
                        licensePlate = userInput;
                        if (!Controller::checkReservationExists(licensePlate)) {
                            cout << "\nError: No reservation found for vehicle \"" << licensePlate 
                                 << "\".\n" 
                                 << "\nEnter License Plate Number for check-in [or 0 to return to main menu]"; 
                        } else {
                           validInput = true; 
                        }
                    }
                }
                userInput = "";
                validInput = false;
                double length = Controller::getVehicle(licensePlate).value().length;
                double height = Controller::getVehicle(licensePlate).value().height;
                cout << "\nVehicle: " << licensePlate 
                     << "\nLength: " << length << " m"
                     << "\nHeight: " << height << " m\n"
                     << "You are about to check-in for vehicle " << licensePlate << ". Confirm action? (Y/N): ";

                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Restarting the check-in process...\n";
                        validInput = true;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            Controller::checkInVehicle(licensePlate);
                            cout << "\nVehicle " << licensePlate <<" has checked-in successfully."
                                 << "\nReturning to main menu...\n";
                            validInput = true;
                            return;
                        } else {
                            cout << "\nInvalid choice, please try again...\n";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
        }    
    }

    static void handleReportsAndInquiries() {
        while (true) {
            cout<<"\n============== Reports & Inquiries==============\n"
                <<"1) Print Sailing Report\n"
                <<"2) Query Individual Sailing\n"
                <<"0) Return to Main Menu\n"
                <<"\nEnter selection: ";
            
            int choice; 
            if (!(cin >> choice)) {  
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }
            
            if (choice == 0) {
                return;
            } 
            if (choice == 1) {
                int page=0;
                while (true) {
                    auto list = Controller::getSailingReport(page);
                    if (list.empty()) { 
                        cout<<"No more sailings.\n"; 
                        break; 
                    }
                    cout <<"\nPage " << (page+1) <<"\n";
                    for (auto& e: list) {
                        cout << e.sailingID << " | " << e.vesselID << " | LRL = " << e.LRL << " | HRL = " << e.HRL << "\n";
                    }
                    cout << "N = Next, P = Prev, E = Exit: "; 
                    char opt; 
                    cin >> opt; 
                    cin.ignore(10000, '\n');
                    if ( opt == 'N' || opt == 'n' ) page++;
                    else if ( opt=='P'|| opt=='p' ) page = ( page > 0 ? page-1 : 0 );
                    else if ( opt == 'E' || opt == 'e' ) break;
                }

            }
            if (choice == 2) {
                cout << "\n============== Reports & Inquiries ==============\n";
                
                cout << "\nEnter sailing ID for query [or 0 to return to main menu]: ";
                string userInput;
                bool validInput = false;
                while (!validInput) {
                    cin >> userInput;
                    if (userInput == "0") {
                        return;
                    }
                    if (!Controller::checkSailingExists(userInput)) {
                        cout << "\nError: Sailing ID \"" << userInput << "\" does not exist.\n"
                             << "Please enter a valid Sailing ID [or 0 to return to main menu]: ";
                        continue;
                    } else {
                        auto info = Controller::queryIndividualSailing(userInput);
                        cout << "Sailing ID: " << info.sailingID 
                             << "\nVessel Name: " << info.vesselID 
                             << "\nHRL Remaining: " << info.HRL 
                             << "\nLRL Remaining: " << info.LRL 
                             << "\n";
                    }
                    cout << "\nDo you wish to query another sailing? (Y/N): ";
                    cin >> userInput;
                    if (userInput == "n" || userInput == "N") {
                        cout << "\nOperation cancelled. Returning to the main menu...\n";
                        return;
                    } else {
                        if (userInput == "y" || userInput == "Y") {
                            validInput = false;
                        } else {
                            cout << "\nInvalid choice, please try again...\n";
                            cin.clear(); 
                            cin.ignore(10000, '\n');
                        }
                    }
                }
            }
        }
    }
    void begin_input() {
        Controller::init();
        while (true) {
            cout<<"\n============== Main Menu ==============\n"
                <<"1) Vessel Management\n"
                <<"2) Sailing Management\n"
                <<"3) Reservation Management\n"
                <<"4) Reports & Inquiries\n"
                <<"0) Exit Application\n"
                <<"Select: ";

            int choice; 
            if (!(cin >> choice)) {  
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }
            if (choice == 0) { 
                Controller::shutdown(); 
                return;
            } 
            if (choice == 1) {
                handleVesselManagement();
            } 
            if (choice == 2) { 
                handleSailingManagement();
            } 
            if (choice == 3) {
                handleReservationManagement();
            }
            if (choice == 4) {
                handleReportsAndInquiries();
            }
        }
    }
}
