#include <windows.h>
#include <iostream>

#include "printer/WINPrinter.h"

void WindowsPrinter::sendCustomPrintJob(const std::string &printerName, const std::string &jobContent, const std::string &jobName) {
    HANDLE hPrinter;
    DOC_INFO_1 docInfo;
    DWORD dwJob;
    DWORD dwBytesWritten;

    std::cout << "Attempting to open printer: " << printerName << std::endl;
    if (OpenPrinter(const_cast<char*>(printerName.c_str()), &hPrinter, NULL)) {
        std::cout << "Printer opened successfully." << std::endl;
        docInfo.pDocName = const_cast<char*>("My Document");
        docInfo.pOutputFile = NULL;
        docInfo.pDatatype = const_cast<char*>("RAW");

        std::cout << "Starting document..." << std::endl;
        dwJob = StartDocPrinter(hPrinter, 1, (LPBYTE)&docInfo);
        if (dwJob > 0) {
            std::cout << "Document started successfully." << std::endl;
            if (StartPagePrinter(hPrinter)) {
                std::cout << "Writing to printer..." << std::endl;
                //std::string fullJob = "Sample Document\n\n" + jobContent + "\f"; // Add text to the job and form feed
                if (WritePrinter(hPrinter, (LPVOID)jobContent.c_str(), jobContent.size(), &dwBytesWritten)) {
                    std::cout << "Print job sent successfully." << std::endl;
                } else {
                    std::cerr << "Failed to write to printer. Error: " << GetLastError() << std::endl;
                }
                if (!EndPagePrinter(hPrinter)) {
                    std::cerr << "Failed to end page. Error: " << GetLastError() << std::endl;
                }
            } else {
                std::cerr << "Failed to start page. Error: " << GetLastError() << std::endl;
            }
            if (!EndDocPrinter(hPrinter)) {
                std::cerr << "Failed to end document. Error: " << GetLastError() << std::endl;
            }
        } else {
            std::cerr << "Failed to start document. Error: " << GetLastError() << std::endl;
        }
        ClosePrinter(hPrinter);
    } else {
        std::cerr << "Failed to open printer. Error: " << GetLastError() << std::endl;
    }
}