#ifndef PRINTER_H
#define PRINTER_H

#include <string>

class Printer {
public:
    virtual ~Printer() = default;

    // Method to send a print job
    virtual void sendPrintJob(const std::string &printerName, const std::string &job) = 0;

    // Factory method to create a Printer instance
    static Printer* create();
};

#endif // PRINTER_H