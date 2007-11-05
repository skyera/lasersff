#include "Controller.h"

using namespace rcam;

Controller::Controller():m_init(false)
{

}

Controller::~Controller()
{

}

bool Controller::IsInited()
{
    return m_init;
}