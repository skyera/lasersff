#ifndef RCAM_PARAMETERS_H
#define RCAM_PARAMETERS_H

#include <string>

namespace rcam {

class Parameters
{
public:
    Parameters();

    static const std::string AppName;
    static const std::string SerialPort;
    static const std::string ImagePath;
    static const std::string PowerPercent;
};

}

#endif