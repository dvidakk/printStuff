#ifndef CUPS_PRINTER_H
#define CUPS_PRINTER_H

#include "common.h"

class CUPSPrinter : public Printer {
public:
    void sendCustomPrintJob(const std::string &printerName, const std::string &jobContent, const std::string &jobName) override;
};

#endif // CUPS_PRINTER_H