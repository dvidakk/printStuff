#ifndef WINDOWS_PRINTER_H
#define WINDOWS_PRINTER_H

#include "common.h"

class WindowsPrinter : public Printer {
public:
    void sendCustomPrintJob(const std::string &printerName, const std::string &jobContent, const std::string &jobName) override;
};

#endif // WINDOWS_PRINTER_H