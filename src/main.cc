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

#include "cxxopts.hpp"

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

std::string selectPrinter(const std::vector<std::string>& printers, int choice) {
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


int main(int argc, char** argv) {
    cxxopts::Options options("PrinterCLI", "Command-line interface for printer operations");

    options.add_options()
        ("a,action", "Action to perform (list, print)", cxxopts::value<std::string>())
        ("p,printer", "Printer number (for print action)", cxxopts::value<int>())
        ("f,file", "Path to the file to print (for print action)", cxxopts::value<std::string>())
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!result.count("action")) {
        std::cerr << "Action is required" << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string action = result["action"].as<std::string>();

 if (action == "list") {
        std::cout << "Listing printers..." << std::endl;
        std::vector<std::string> printers = listPrinters();
        if (printers.empty()) {
            std::cerr << "No printers found\n";
            return 1;
        }

        std::cout << "Available printers:\n";
        for (size_t i = 0; i < printers.size(); ++i) {
            std::cout << i + 1 << ". " << printers[i] << "\n";
        }
    } else if (action == "print") {
        if (!result.count("printer") || !result.count("file")) {
            std::cerr << "Printer number and file path are required for print action\n";
            std::cout << options.help() << std::endl;
            return 1;
        }

        int printerChoice = result["printer"].as<int>();
        std::string filePath = result["file"].as<std::string>();

        std::cout << "Listing printers..." << std::endl;
        std::vector<std::string> printers = listPrinters();
        if (printers.empty()) {
            std::cerr << "No printers found\n";
            return 1;
        }

        std::cout << "Selecting printer..." << std::endl;
        std::string selectedPrinter = selectPrinter(printers, printerChoice);
        if (selectedPrinter.empty()) {
            return 1;
        }

        std::ifstream fileStream(filePath);
        if (!fileStream.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return 1;
        }

        std::string job((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
        fileStream.close();

        std::cout << "Printing job:\n" << job << std::endl;
        std::cout << "Sending print job..." << std::endl;
        printToSelectedPrinter(selectedPrinter, job);
    } else {
        std::cerr << "Unknown action: " << action << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    return 0;
}
