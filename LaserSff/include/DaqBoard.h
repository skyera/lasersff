#ifndef RCAM_DAQ_BOARD_H
#define RCAM_DAQ_BOARD_H

namespace rcam {

enum {SHUTTER_OPEN = 0, SHUTTER_CLOSE = 1, FINISHED = 0, NOT_FINISHED = 1};

class DaqBoard {
public:
    DaqBoard(){}
    virtual ~DaqBoard(){}
    
    virtual bool Init() = 0;
    virtual int ReadShutterSignal() = 0;
    virtual int ReadFinishSignal() = 0;
    virtual bool SetLaserPower(int percent) = 0;

};

}

#endif