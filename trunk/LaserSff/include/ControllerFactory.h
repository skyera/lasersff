#ifndef RCAM_CONTROLLER_FACTORY_H
#define RCAM_CONTROLLER_FACTORY_H

#include <wx/wx.h>
#include <boost/shared_ptr.hpp>
namespace rcam {

class Controller;

class ControllerFactory
{
public:
    static boost::shared_ptr<Controller> CreateController(const wxString& name);
private:
    ControllerFactory();
};

}

#endif