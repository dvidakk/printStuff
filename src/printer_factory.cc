#include "printer/factory.h"
#ifdef _WIN32
#include "printer/WINPrinter.h"
#else
#include "printer/CUPSPrinter.h"
#endif

Printer* PrinterFactory::createPrinter() {
#ifdef _WIN32
    return new WindowsPrinter();
#else
    return new CUPSPrinter();
#endif
}