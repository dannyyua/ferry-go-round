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

// Utility functions
static bool promptConfirm(const string& message) {
    char resp;
    
    while (true) {
        cout << message << " (Y/N): ";
        
        if (!(cin >> resp)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; 
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if ( resp == 'Y' || resp=='y' ) return true;
        
        if ( resp == 'N' || resp =='n') return false;
        
        cout << "Please enter Y or N.\n";
    }
}
static bool promptRetryCancel() {
    int opt;
    
    while (true) {
        cout << "Enter 1 to retry or 0 to cancel: ";
        
        if (!(cin >> opt)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; 
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (opt == 1) return true;
        
        if (opt == 0) return false;
    }
}
static bool readBuffer(char* buf, size_t len, const string& prompt) {
    while (true) {
        cout << prompt;
        cin.getline(buf, len);
        
        if (cin.fail()) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; 
        }
        
        if (strlen(buf) == 0) continue;
        
        if (strlen(buf) == 1 && buf[0] == '0') return false;
        
        return true;
    }
}
static bool readDouble(double& v, double lo, double hi, const string& prompt) {
    while (true) {
        cout << prompt <<" [" << lo << "-" << hi << "] (0 to cancel): ";
        
        if (!(cin >> v)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid.\n"; 
            
            if (!promptRetryCancel()) {
                return false;
            }
            continue; 
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if ( v == 0 ) return false;
        
        if ( v < lo || v > hi) { 
            cout << "Out of range.\n"; 
            if (!promptRetryCancel()) {
                return false;           
            }  
            continue; 
        }
        
        return true;
    }
}

// Vessel Management
static void handleVesselManagement() {
    while (true) {
        cout<<"\n===== VESSEL MANAGEMENT =====\n"
            <<"1) Create New Vessel Record\n"
            <<"0) Return to Main Menu\n"
            <<"Select: ";

        int choice; 
        if (!(cin >> choice)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid.\n"; 
            
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 0) return;
        
        if (choice == 1) {
            // Vessel ID
            char vid[21];
            while (true) {
                if (!readBuffer(vid, sizeof(vid), "Enter Vessel ID (max20) or 0 to cancel: ")) break;
                
                if (!Controller::checkVesselExists(vid)) break;
                cout << "Already exists.\n";
                
                if (!promptRetryCancel()) { vid[0]='\0'; break; }
            }
            
            if (strlen(vid) == 0) continue;
            
            // HCLL
            double h;
            if (!readDouble(h, 0.1, 999.9, "Enter High Ceiling Length")) continue;
            
            // LCLL
            double l;
            if (!readDouble(l, 0.1, 999.9, "Enter Low Ceiling Length")) continue;
            cout << "Create vessel " << vid << " with H=" << h << " L=" << l <<"?\n";
            
            if (promptConfirm("Confirm")) {
                Controller::createNewVessel(vid, l, h);
                cout << "Created.\n";
            }
        } else cout << "Invalid choice.\n";
    }
}

// Sailing Management
static void handleSailingManagement() {
    while (true) {
        cout<<"\n===== SAILING MANAGEMENT =====\n"
            <<"1) Create New Sailing\n"
            <<"2) Delete Sailing\n"
            <<"0) Return to Main Menu\n"
            <<"Select: ";
        
        int choice; 
        if (!(cin >> choice)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid.\n"; 
            
            continue; 
         }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 0) return;
        
        if (choice == 1) {
            char vid[21], sid[21];
            // vessel ID
            while (true) {
                if (!readBuffer(vid, sizeof(vid), "Enter Vessel ID or 0 to cancel: ")) break;
                
                if (Controller::checkVesselExists(vid)) break;
                cout << "Not found.\n";
                
                if (!promptRetryCancel()) { vid[0]='\0'; break; }
            }
            
            if (strlen(vid) == 0) continue;
            // sailing ID
            while (true) {
                if (!readBuffer(sid, sizeof(sid), "Enter Sailing ID or 0 to cancel: ")) break;
                
                if (!Controller::checkSailingExists(sid)) break;
                cout << "Already exists.\n";
                
                if (!promptRetryCancel()) { sid[0]='\0'; break; }
            }
            
            if (strlen(sid) == 0) continue;
            cout << "Create sailing " << sid << " on vessel " << vid << "?\n";
            
            if (promptConfirm("Confirm")) {
                Controller::createNewSailing(vid, sid);
                cout << "Created.\n";
            }

        } else if (choice == 2) {
            char sid[21];
            while (true) {
                if (!readBuffer(sid, sizeof(sid), "Enter Sailing ID or 0 to cancel: ")) break;
                
                if (Controller::checkSailingExists(sid)) break;
                cout << "Not found.\n";
                
                if (!promptRetryCancel()) { sid[0]='\0'; break; }
            }
            
            if (strlen(sid) == 0) continue;
            cout << "Delete sailing " << sid << "?\n";
            
            if (promptConfirm("Confirm")) {
                Controller::deleteSailing(sid);
                cout << "Deleted.\n";
            }

        } else cout << "Invalid choice.\n";
    }
}

// Reservation Management
static void handleReservationManagement() {
    while (true) {
        cout<<"\n===== RESERVATION MANAGEMENT =====\n"
            <<"1) Create New Reservation\n"
            <<"2) Cancel Reservation\n"
            <<"3) Check-In Vehicle\n"
            <<"0) Return to Main Menu\n"
            <<"Select: ";
        
        int choice; 
        if (!(cin>>choice)) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid.\n"; 
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 0) return;
        
        if (choice == 1) {
            char sid[21], plate[21];
            // sailing ID
            while (true) {
                if (!readBuffer(sid, sizeof(sid), "Enter Sailing ID or 0 to cancel: ")) break;
                
                if (Controller::checkSailingExists(sid)) break;
                cout << "Not found.\n";
                
                if (!promptRetryCancel()) { sid[0]='\0'; break; }
            }
            
            if (strlen(sid) == 0) continue;
            // plate
            while (true) {
                if (!readBuffer(plate, sizeof(plate), "Enter Vehicle Plate or 0 to cancel: ")) break;
                
                if (Controller::checkVehicleExists(plate)) break;
                cout << "Not registered.\n Register now? \n";
                
                if (!promptConfirm("Register vehicle?")) { plate[0]='\0'; break; }
                // register vehicle inline
                char phone[16];
                readBuffer(phone, sizeof(phone), "Enter Phone or 0 to cancel: ");
                double length, height;
                
                readDouble(length,0.1,30.0,"Enter Vehicle Length");
                readDouble(height,0.1,6.0,"Enter Vehicle Height");
                
                Controller::createNewVehicle(plate, phone, length, height);
                cout << "Registered.\n";
                
                break;
            }

            if (strlen(plate) == 0) continue;

            // capacity check
            auto sail = Controller::getSailing(sid);
            auto veh = Controller::getVehicle(plate);

            double vlen = veh.value().length;

            bool special = veh.value().height > 2.0 || vlen > 7.0;
            double avail = special ? sail.value().HRL:sail.value().LRL;
            
            if (avail < vlen) { 
                cout << "No capacity.\n"; 
                continue; 
            }
            cout << "Reserve " << plate << " on " << sid << "?\n";
            
            if (promptConfirm("Confirm")) {
                Controller::createNewReservation(sid, plate);
                cout<<"Reserved.\n";
            }

        } else if (choice==2) {
            char sid[21], plate[21];
            
            readBuffer(sid,sizeof(sid),"Enter Sailing ID or 0 to cancel: ");
            readBuffer(plate,sizeof(plate),"Enter Plate or 0 to cancel: ");
            
            cout << "Cancel reservation " << plate << " on " << sid << "?\n";

            if (promptConfirm("Confirm")) {
                Controller::cancelReservation(sid, plate);
                cout << "Cancelled.\n";
            }

        } else if (choice==3) {
            char plate[21]; 
            readBuffer(plate,sizeof(plate),"Enter Plate or 0 to cancel: ");
            cout << "Check in " << plate << "?\n";
            
            if (promptConfirm("Confirm")) { 
                Controller::checkInVehicle(plate); 
                cout<<"Checked in.\n"; 
            }

        } else cout << "Invalid choice.\n";
    }
}

// Reports & Inquiries
static void handleReportsAndInquiries() {
    while (true) {
        cout<<"\n===== REPORTS & INQUIRIES =====\n"
            <<"1) Print Sailing Report\n"
            <<"2) Query Individual Sailing\n"
            <<"0) Return to Main Menu\n"
            <<"Select: ";
        
        int choice; 
        if (!(cin >> choice )) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout<<"Invalid.\n";
             
            continue; 
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice==0) return;
        
        if (choice==1) {
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if ( opt == 'N' || opt == 'n' ) page++;
                else if ( opt=='P'|| opt=='p' ) page = ( page > 0 ? page-1 : 0 );
                else if ( opt == 'E' || opt == 'e' ) break;
            }

        } else if (choice==2) {
            while (true) {
                char sid[21];
                
                if ( !readBuffer(sid,sizeof(sid), "Enter Sailing ID or 0 to return: ")) break;
                
                if (!Controller::checkSailingExists(sid)) { 
                    cout<<"Not found.\n"; 
                    continue; 
                }
                
                auto info = Controller::queryIndividualSailing(sid);
                cout << "SailingID: " << info.sailingID << " Vessel: " << info.vesselID << " LRL = " << info.LRL << " HRL = " << info.HRL << "\n";
                
                if (!promptConfirm("Query another?")) break;
            }

        } else cout << "Invalid choice.\n";
    }
}

void begin_input() {
    Controller::init();
    while (true) {
        cout<<"\n============== MAIN MENU ==============\n"
            <<"1) Vessel Management\n"
            <<"2) Sailing Management\n"
            <<"3) Reservation Management\n"
            <<"4) Reports & Inquiries\n"
            <<"0) Exit Application\n"
            <<"Select: ";

        int m; 
        if ( !( cin >> m )) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid.\n"; 
            continue; 
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (m==0) { Controller::shutdown(); break; }
        
        if (m==1) handleVesselManagement();
        
        else if (m==2) handleSailingManagement();
        
        else if (m==3) handleReservationManagement();
        
        else if (m==4) handleReportsAndInquiries();
        
        else cout<<"Invalid choice.\n";
    }
}

} // namespace UserInterface
