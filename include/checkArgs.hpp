#ifndef _checkArgs_HPP_
#define _checkArgs_HPP_

#include "global.hh"

class ArgsHandler {
private:
    int numThreads;
    std::string filename;
    bool showHelp;

public:
    ArgsHandler(int argc, char **argv);
    int getNumThreads() const;
    std::string getFilename() const;
    bool shouldShowHelp() const;
};

ArgsHandler::ArgsHandler(int argc, char **argv) : numThreads(1), showHelp(false) {
    int opt;
    struct option longOptions[] = {
        {"file", required_argument, nullptr, 'f'},
        {"threads", required_argument, nullptr, 't'},
        {"help", no_argument, nullptr, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "f:t:h:", longOptions, NULL)) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'h':
                showHelp = true;
                break;
            default:
                showHelp = true;
        }
    }
}

int ArgsHandler::getNumThreads() const {
    return numThreads;
}

std::string ArgsHandler::getFilename() const {
    return filename;
}

bool ArgsHandler::shouldShowHelp() const {
    return showHelp;
}

#endif
