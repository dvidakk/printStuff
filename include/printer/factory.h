#ifndef PRINTER_FACTORY_H
#define PRINTER_FACTORY_H

#include "./common.h"

class PrinterFactory {
public:
    static Printer* createPrinter();
};

#endif // PRINTER_FACTORY_H