#include "DaqBoardFactory.h"
#include "PCI1200.h"

using namespace rcam;

boost::shared_ptr<DaqBoard> DaqBoardFactory::CreateDaqboard(const std::string &name)
{
    if(name == "pci1200") {
        return boost::shared_ptr<DaqBoard>(new PCI1200);
    } else {
        assert(0);
        return boost::shared_ptr<DaqBoard>();
    }
}