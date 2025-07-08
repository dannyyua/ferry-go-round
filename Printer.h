// Printer.h
//******************************************************************
// DEFINITION MODULE: Printer
//
// PURPOSE:          A boundary module responsible for handling all
//                   printing operations. It abstracts the hardware-specific
//                   details of printing from the rest of the system.
//
// (* Revision History:
//   Rev. 1.0 - 2025/07/07
// *)
//******************************************************************
#ifndef PRINTER_H
#define PRINTER_H

#include <string>

// The Printer namespace provides an interface to the system's printing hardware.
namespace Printer {
    // Corresponds to OCD "printSailingReport()".
    // Sends the provided report content to the system's printer.
    void printSailingReport(const std::string& reportContent);
}

#endif // PRINTER_H
