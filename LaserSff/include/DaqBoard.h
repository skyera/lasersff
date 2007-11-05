#ifndef RCAM_DAQ_BOARD_H
#define RCAM_DAQ_BOARD_H

namespace rcam {

enum {SHUTTER_OPEN, SHUTTER_CLOSE, FINISHED, NOT_FINISHED};

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