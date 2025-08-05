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
            cout << "\n============== Vessel Management ==============\n"
                 << "1) Create New Vessel Record\n"
                 << "0) Return to main menu\n"
                 << "\nEnter selection: ";

            string choice_input;
            // Use getline to read the menu choice to avoid input buffer issues later
            getline(cin, choice_input); 
            int choice = -1;
            try {
                choice = stoi(choice_input); // Convert the string to a number
            } catch (...) {
                cout << "\nInvalid choice, please enter a number.\n";
                continue;
            }

            if (choice == 0) {
                return; // Exit to the previous menu
            }   
            
            if (choice == 1) {
                cout << "\n====== CREATE NEW VESSEL RECORD ======\n";
                
                string vesselID;
                while (true) {
                    cout << "Enter Vessel ID (max 20 chars) [or 0 to return]: ";
                    // Use getline to correctly read names with spaces
                    getline(cin, vesselID);

                    if (vesselID == "0") {
                        // User chose to cancel, break the inner loop
                        break; 
                    }
                    if (vesselID.empty()) {
                        cout << "\nError: Vessel ID cannot be empty.\n";
                        continue;
                    }
                    if (vesselID.length() > 20) {
                        cout << "\nError: Vessel ID cannot be longer than 20 characters.\n";
                        continue;
                    }
                    if (Controller::checkVesselExists(vesselID)) {
                        cout << "\nError: Vessel ID \"" << vesselID << "\" already exists.\n";
                        continue;
                    }
                    
                    // If all checks pass, the input is valid.
                    break;
                }
                
                // If user entered '0' in the previous step, return to the vessel menu
                if (vesselID == "0") {
                    cout << "\nOperation cancelled.\n";
                    continue; 
                }

                double hcll;
                while (true) {
                    cout << "Enter High Ceiling Lane Length (HCLL) in meters (1.0 - 3600.0) [or 0 to return]: ";
                    string input;
                    getline(cin, input);
                    if (input == "0") {
                        hcll = 0.0; // Signal cancellation
                        break;
                    }
                    if (tryParseDouble(input, hcll) && hcll >= 1.0 && hcll <= 3600.0) {
                        break; // Valid input
                    } else {
                        cout << "\nInvalid input. Please enter a number in the specified range.\n";
                    }
                }
                if (hcll == 0.0) {
                    cout << "\nOperation cancelled.\n";
                    continue;
                }

                double lcll;
                while (true) {
                    cout << "Enter Low Ceiling Lane Length (LCLL) in meters (1.0 - 3600.0) [or 0 to return]: ";
                    string input;
                    getline(cin, input);
                    if (input == "0") {
                        lcll = 0.0; // Signal cancellation
                        break;
                    }
                    if (tryParseDouble(input, lcll) && lcll >= 1.0 && lcll <= 3600.0) {
                        break; // Valid input
                    } else {
                        cout << "\nInvalid input. Please enter a number in the specified range.\n";
                    }
                }
                if (lcll == 0.0) {
                    cout << "\nOperation cancelled.\n";
                    continue;
                }

                cout << "\nYou entered the following:\n"
                     << "  Vessel ID: " << vesselID << "\n"
                     << "  HCLL: " << fixed << setprecision(1) << hcll << " m\n"
                     << "  LCLL: " << fixed << setprecision(1) << lcll << " m";
                
                string confirmation;
                while (true) {
                    cout << "\nConfirm creation of this vessel? (Y/N): ";
                    getline(cin, confirmation);
                    if (confirmation == "y" || confirmation == "Y") {
                        Controller::createNewVessel(vesselID, lcll, hcll);
                        cout << "\nNew vessel \"" << vesselID << "\" has been successfully created.\n";
                        break; 
                    } else if (confirmation == "n" || confirmation == "N") {
                        cout << "\nOperation cancelled.\n";
                        break;
                    } else {
                        cout << "\nInvalid choice, please enter Y or N.\n";
                    }
                }
            } else {
                cout << "\nInvalid selection. Please try again.\n";
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
            cin >> choice;
            if (cin.fail()) { 
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "\nInvalid choice, please try again...\n";
                continue; 
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            
            if (choice == 0) {
                return;
            } 
            else if (choice == 1) {
                cout << "\n====== CREATE NEW SAILING ======\n";
                
                string vesselID;
                while (true) {
                    cout << "Enter vessel ID to assign sailings to [or 0 to return]: ";
                    // FIX: Use getline to read the entire line, including spaces
                    getline(cin, vesselID); 

                    if (vesselID == "0") break; // Exit loop to cancel

                    if (Controller::checkVesselExists(vesselID)) {
                        break; // Valid vessel ID, proceed
                    } else {
                        cout << "\nError: Vessel ID \"" << vesselID << "\" does not exist.\n";
                    }
                }
                if (vesselID == "0") {
                    cout << "\nOperation cancelled.\n";
                    continue; // Go back to sailing menu
                }

                string sailingID;
                while (true) {
                    cout << "Enter new sailing ID (e.g., YVR-21-06) [or 0 to return]: ";
                    getline(cin, sailingID); // FIX: Use getline
                    if (sailingID == "0") break; // Exit loop to cancel

                    if (Controller::checkSailingExists(sailingID)) {
                        cout << "\nError: sailing ID \"" << sailingID << "\" already exists.\n";
                    } else if (sailingID.empty()) {
                        cout << "\nError: Sailing ID cannot be empty.\n";
                    } else {
                        break; // Valid sailing ID, proceed
                    }
                }
                if (sailingID == "0") {
                    cout << "\nOperation cancelled.\n";
                    continue;
                }

                cout << "\nYou entered the following:\n"
                     << "  Vessel ID: " << vesselID << "\n"
                     << "  Sailing ID: " << sailingID << "\n";
                
                string confirmation;
                while (true) {
                    cout << "\nConfirm creation of this sailing? (Y/N): ";
                    getline(cin, confirmation); // FIX: Use getline
                    if (confirmation == "n" || confirmation == "N") {
                        cout << "\nOperation cancelled.\n";
                        break; // Exit confirmation loop
                    } else if (confirmation == "y" || confirmation == "Y") {
                        Controller::createNewSailing(vesselID, sailingID);
                        cout << "\nSailing \"" << sailingID << "\" has been successfully scheduled for vessel \""
                             << vesselID << "\".\n";
                        break; // Exit confirmation loop
                    } else {
                        cout << "\nInvalid choice, please enter Y or N.\n";
                    }
                }
            }
            else if (choice == 2) {
                cout << "\n====== DELETE SAILING ======\n";
                
                string sailingID;
                while(true) {
                    cout << "Enter sailing ID for deletion [or 0 to return]: ";
                    getline(cin, sailingID); // FIX: Use getline
                    if (sailingID == "0") break;

                    if (Controller::checkSailingExists(sailingID)) {
                        break;
                    } else {
                        cout << "\nError: sailing ID \"" << sailingID << "\" does not exist.\n";
                    }
                }
                if (sailingID == "0") {
                    cout << "\nOperation cancelled.\n";
                    continue;
                }
                
                string confirmation;
                while (true) {
                    cout << "\nYou are about to delete sailing ID: " << sailingID << ". This will also delete all associated reservations. Confirm action? (Y/N): ";
                    getline(cin, confirmation); // FIX: Use getline
                    if (confirmation == "n" || confirmation == "N") {
                        cout << "\nOperation cancelled.\n";
                        break;
                    } else if (confirmation == "y" || confirmation == "Y") {
                        Controller::deleteSailing(sailingID);
                        cout << "\nSailing \"" << sailingID << "\" has been deleted.\n";
                        break;
                    } else {
                        cout << "\nInvalid choice, please enter Y or N.\n";
                    }
                }
            } else {
                 cout << "\nInvalid selection. Please try again.\n";
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
                            //Controller::deleteSailing(sailingID);
                            Controller::cancelReservation(sailingID, licensePlate);
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
            cin >> choice;
            if (cin.fail()) {  
                cout << "\nInvalid choice. Please enter a number.\n";
                // Clear the error state from cin
                cin.clear(); 
                // Discard the rest of the invalid input line
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue; 
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
