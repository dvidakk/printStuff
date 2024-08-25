#include <cups/cups.h>
#include <iostream>
#include <cstdio>

#include "printer/CUPSprinter.h"

void CUPSPrinter::sendCustomPrintJob(const std::string &printerName, const std::string &jobContent, const std::string &jobName) {
    // CUPS-specific implementation
    cups_dest_t *dests, *dest;
    int num_dests = cupsGetDests(&dests);
    dest = cupsGetDest(NULL, NULL, num_dests, dests);

    if (dest == NULL) {
        std::cerr << "Printer not found." << std::endl;
        cupsFreeDests(num_dests, dests);
        return;
    }

    cups_option_t *options = NULL;
    int num_options = 0;
    num_options = cupsAddOption(CUPS_COPIES, "1", num_options, &options);

    std::string tempFileName = "/tmp/cups_job.txt";
    FILE *tempFile = fopen(tempFileName.c_str(), "w");
    if (tempFile == NULL) {
        std::cerr << "Failed to create temporary file for print job." << std::endl;
        cupsFreeOptions(num_options, options);
        cupsFreeDests(num_dests, dests);
        return;
    }
    fwrite(jobContent.c_str(), 1, jobContent.size(), tempFile);
    fclose(tempFile);

    int job_id = cupsPrintFile(dest->name, tempFileName.c_str(), jobName.c_str(), num_options, options);
    if (job_id == 0) {
        std::cerr << "Failed to print job. Error: " << cupsLastErrorString() << std::endl;
    } else {
        std::cout << "Print job sent successfully. Job ID: " << job_id << std::endl;
    }

    cupsFreeOptions(num_options, options);
    cupsFreeDests(num_dests, dests);
    remove(tempFileName.c_str());
}