#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "printer/factory.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <cups/cups.h>
#endif

std::vector<std::string> listPrinters() {
    std::vector<std::string> printers;

#ifdef _WIN32
    DWORD needed, returned;
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, NULL, 0, &needed, &returned);
    BYTE* buffer = new BYTE[needed];
    if (EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, buffer, needed, &needed, &returned)) {
        PRINTER_INFO_1* printerInfo = (PRINTER_INFO_1*)buffer;
        for (DWORD i = 0; i < returned; ++i) {
            printers.push_back(printerInfo[i].pName);
        }
    } else {
        std::cerr << "Failed to enumerate printers. Error: " << GetLastError() << std::endl;
    }
    delete[] buffer;
#else
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    if (num_dests > 0) {
        for (int i = 0; i < num_dests; ++i) {
            printers.push_back(dests[i].name);
        }
    } else {
        std::cerr << "Failed to enumerate printers using CUPS." << std::endl;
    }
    cupsFreeDests(num_dests, dests);
#endif

    std::cout << "Printers found: " << printers.size() << std::endl;
    return printers;
}

std::string selectPrinter(const std::vector<std::string>& printers) {
    std::cout << "Available printers:\n";
    for (size_t i = 0; i < printers.size(); ++i) {
        std::cout << i + 1 << ". " << printers[i] << "\n";
    }

    int choice;
    std::cout << "Select a printer (1-" << printers.size() << "): ";
    std::cin >> choice;

    if (choice < 1 || static_cast<size_t>(choice) > printers.size()) {
        std::cerr << "Invalid choice\n";
        return "";
    }

    return printers[choice - 1];
}

void printToSelectedPrinter(const std::string& printerName, const std::string& job) {
    std::cout << "Printing to: " << printerName << std::endl;
    Printer* printer = PrinterFactory::createPrinter();
    if (printer) {
        printer->sendCustomPrintJob(printerName, job, "Test Print Job");
        delete printer;
    } else {
        std::cerr << "Failed to create printer object\n";
    }
}

int main() {
    std::cout << "Listing printers..." << std::endl;
    std::vector<std::string> printers = listPrinters();
    if (printers.empty()) {
        std::cerr << "No printers found\n";
        return 1;
    }

    std::cout << "Selecting printer..." << std::endl;
    std::string selectedPrinter = selectPrinter(printers);
    if (selectedPrinter.empty()) {
        return 1;
    }

    std::cout << "Creating postscript file..." << std::endl;
    std::ofstream file("test.ps");
    if (file.is_open()) {
        file << "%!PS\n";
        file << "/Courier findfont 12 scalefont setfont\n";
        file << "72 720 moveto\n";
        file << "(Hello, World!) show\n";
        file << "showpage\n";
        file.close();
        std::cout << "Postscript file created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create postscript file\n";
        return 1;
    }

	// send the postscript file to the selected printer
	std::ifstream fileStream("test.ps");
	std::string job((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
    
	std::cout << "Printing job:\n" << job << std::endl;
    std::cout << "Sending print job..." << std::endl;
    printToSelectedPrinter(selectedPrinter, job);

    return 0;
}