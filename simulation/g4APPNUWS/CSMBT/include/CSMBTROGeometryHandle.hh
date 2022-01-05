// interface to manage the geometries of the CSMBT readout
//
// Original author G. Tassielli
//

#ifndef CSMBTROGeometryHandle_hh
#define CSMBTROGeometryHandle_hh

#include "PXSTbsROGeometryHandle.hh"

//#include "CSMBtracker.hh"

namespace csmbt {

class CSMBtracker;

class CSMBTROGeometryHandle : public pxstbs::ROGeometryHandle {

	friend class CSMBTMaker;

protected:
	CSMBTROGeometryHandle(CSMBtracker *csmbt=0x0);

public:
	~CSMBTROGeometryHandle() {}

	void  SelectLadder(int Layer, int PhiSec, int Ladder) override;
    void  SelectRO(int Layer, int PhiSec, int Ladder, int fChFstSd, int fChSndSd, int fSubShell=0) override;
    void  SelectRODet(unsigned long det) override;
    void  SelectRO(int chambID, int channelID) override;
    unsigned long computeDet(int Layer, int PhiSec, int Ladder, int ChFstSd,int ChSndSd, int subShell=0) override;
    unsigned long computeDet(int chambID, int channelID) override;
    int computeChannelID(int ChFstSd,int ChSndSd, int subShell=0) override;

protected:
    const CSMBtracker *_csmbt;

};

}

#endif
