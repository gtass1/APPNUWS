// interface to manage the geometries of the CSMV readout
//
// Original author G. Tassielli
//

#ifndef CSMVROGeometryHandle_hh
#define CSMVROGeometryHandle_hh

#include "PXSTbsROGeometryHandle.hh"

//#include "CSMVtracker.hh"

namespace csmv {

class CSMVtracker;

class CSMVROGeometryHandle : public pxstbs::ROGeometryHandle {

	friend class CSMVMaker;

protected:
	CSMVROGeometryHandle(CSMVtracker *csmv=0x0);

public:
	~CSMVROGeometryHandle() {}

	void  SelectLadder(int Layer, int PhiSec, int Ladder) override;
    void  SelectRO(int Layer, int PhiSec, int Ladder, int fChFstSd, int fChSndSd, int fSubShell=0) override;
    void  SelectRODet(unsigned long det) override;
    void  SelectRO(int chambID, int channelID) override;
    unsigned long computeDet(int Layer, int PhiSec, int Ladder, int ChFstSd,int ChSndSd, int subShell=0) override;
    unsigned long computeDet(int chambID, int channelID) override;
    int computeChannelID(int ChFstSd,int ChSndSd, int subShell=0) override;

protected:
    const CSMVtracker *_csmv;

};

}

#endif
