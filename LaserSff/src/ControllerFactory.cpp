#include "ControllerFactory.h"

#include "MultiFabController.h"

using namespace rcam;

boost::shared_ptr<Controller> ControllerFactory::CreateController(const wxString &name)
{
    boost::shared_ptr<Controller> ptr;
    if(name == _T("MultiFab")) {
        ptr = boost::shared_ptr<Controller>(new MultiFabController());
    } else {
    
    }
    
    return ptr;
}