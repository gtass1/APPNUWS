// interface to manage the geometries of the CSMTT readout
//
// Original author G. Tassielli
//

#ifndef CSMTTROGeometryHandle_hh
#define CSMTTROGeometryHandle_hh

#include "PXSTbsROGeometryHandle.hh"

//#include "CSMTtracker.hh"

namespace csmtt {

class CSMTtracker;

class CSMTTROGeometryHandle : public pxstbs::ROGeometryHandle {

	friend class CSMTTMaker;

protected:
	CSMTTROGeometryHandle(CSMTtracker *csmtt=0x0);

public:
	~CSMTTROGeometryHandle() {}

	void  SelectLadder(int Layer, int PhiSec, int Ladder) override;
    void  SelectRO(int Layer, int PhiSec, int Ladder, int fChFstSd, int fChSndSd, int fSubShell=0) override;
    void  SelectRODet(unsigned long det) override;
    void  SelectRO(int chambID, int channelID) override;
    unsigned long computeDet(int Layer, int PhiSec, int Ladder, int ChFstSd,int ChSndSd, int subShell=0) override;
    unsigned long computeDet(int chambID, int channelID) override;
    int computeChannelID(int ChFstSd,int ChSndSd, int subShell=0) override;

protected:
    const CSMTtracker *_csmtt;

};

}

#endif
