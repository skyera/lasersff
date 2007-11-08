#ifndef RCAM_LASERAPP_H
#define RCAM_LASERAPP_H

#include <wx/wx.h>
#include <boost/shared_ptr.hpp>
#include <wx/snglinst.h>

class LaserApp: public wxApp
{
public:
    virtual bool OnInit();
private:
    boost::shared_ptr<wxSingleInstanceChecker> m_checker;
};

#endif