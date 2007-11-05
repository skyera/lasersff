#ifndef RCAM_DAQ_BOARD_FACTORY_H
#define RCAM_DAQ_BOARD_FACTORY_H

#include "DaqBoard.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace rcam {

class DaqBoardFactory
{
public:
    static boost::shared_ptr<DaqBoard> CreateDaqboard(const std::string& name);
};
}

#endif