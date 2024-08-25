#ifndef PRINTER_H
#define PRINTER_H

#include <string>

class Printer {
public:
    virtual ~Printer() = default;
    virtual void sendCustomPrintJob(const std::string &printerName, const std::string &jobContent, const std::string &jobName) = 0;
};

#endif // PRINTER_H