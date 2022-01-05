// interface to manage the geometries of the CSMBT readout
//
// Original author G. Tassielli
//

#include "CSMBTROGeometryHandle.hh"

#include "CSMBtracker.hh"

namespace csmbt {

CSMBTROGeometryHandle::CSMBTROGeometryHandle(CSMBtracker *csmbt) :
    _csmbt(csmbt)
{
}

void CSMBTROGeometryHandle::SelectLadder(int Layer, int PhiSec, int Ladder) {
  bool change = false;
  bool isFw = false;

  if (Layer!=_fLayer) {
    _fLayer = Layer;
    _lay = _csmbt->getLayer(_fLayer);
    change = true;
  }
  if (PhiSec!=_fPhiSec) {
    _fPhiSec = PhiSec;
    change = true;
  }
  if (Ladder!=_fLadder) {
    _fLadder = Ladder;
    change = true;
  }
  if (change) {
    isFw = (_lay->getLayerZone()==pxstbs::Layer::forward);
    if (isFw){ _fLadderID = _fLadder*_lay->nPhiSectors() + _fPhiSec;  }
    else { _fLadderID = _fPhiSec*_lay->nLaddersPerSector() + _fLadder; }
    _lad = _lay->getLadder(_fLadderID);

    HepGeom::Translate3D trackerCenter (_csmbt->x0(), _csmbt->y0(), _csmbt->z0());
    HepGeom::Translate3D layerCenter (0.0, _lay->getDetail()->yPosition(), _lay->getDetail()->zPosition());
    _matrx = trackerCenter * layerCenter * _lad->get3DTransfrom();
//    _matrx = _lad->get3DTransfrom();
    _invmatrx = _lad->get3DInvTransfrom();
  }

}


void CSMBTROGeometryHandle::SelectRO(int Layer, int PhiSec, int Ladder, int fChFstSd, int fChSndSd, int fSubShell) {
  bool change = false;
  bool isFw = false;
  if (Layer!=_fLayer) {
    _fLayer = Layer;
    _lay = _csmbt->getLayer(_fLayer);
    change = true;
  }
  if (PhiSec!=_fPhiSec) {
    _fPhiSec = PhiSec;
    change = true;
  }
  if (Ladder!=_fLadder) {
    _fLadder = Ladder;
    change = true;
  }
  if (change) {
    isFw = (_lay->getLayerZone()==pxstbs::Layer::forward);
    if (isFw){ _fLadderID = _fLadder*_lay->nPhiSectors() + _fPhiSec;  }
    else { _fLadderID = _fPhiSec*_lay->nLaddersPerSector() + _fLadder; }
    _lad = _lay->getLadder(_fLadderID);
  }

  if (fSubShell!=_fSubShell) {
    _fSubShell = fSubShell;
    change = true;
  }
  if (fChFstSd!=_fChFstSd) {
    _fChFstSd = fChFstSd;
    change = true;
  }
  if (fChSndSd!=_fChSndSd) {
    _fChSndSd = fChSndSd;
    change = true;
  }

  if (change) {
    _fROChanID = computeChannelID(_fChFstSd,_fChSndSd,_fSubShell); //_fChFstSd*_lad->nReadOutsSndSd() + _fChSndSd;
//std::cout<<"Layer "<<Layer<<" PhiSec "<<PhiSec<<" Ladder "<<Ladder<<" fChFstSd "<<fChFstSd<<" fChSndSd "<<fChSndSd<<std::endl;
//std::cout<<"nReadOutsSndSd "<<_lad->nReadOutsSndSd()<<std::endl;
//std::cout<<"_fROChanID "<<_fROChanID<<std::endl;
    HepGeom::Translate3D trackerCenter (_csmbt->x0(), _csmbt->y0(), _csmbt->z0());
    HepGeom::Translate3D layerCenter (0.0, _lay->getDetail()->yPosition(), _lay->getDetail()->zPosition());
    _matrx = trackerCenter * layerCenter * _lad->get3DTransfrom();
    //    _matrx = _lad->get3DTransfrom();
    _invmatrx = _lad->get3DInvTransfrom();
    _fROChMidLcl = _lad->getROChLoclMidPoint(_fROChanID);
    Local2Global(_fROChMidLcl,_fROChMidGbl);
  }

}

void CSMBTROGeometryHandle::SelectRODet(unsigned long det) {
  if (det<1e+17) {
        // Return the Layer
        int fLayer = det/1e+14;
        det -= ((fLayer)*1e+14);
        --fLayer;
//                _lay = pxstdetector->getLayer(fLayer);

        //Return the Ladder
        int fPhiSec = det/1e+12;
        det -= fPhiSec*1e+12;
        --fPhiSec;

        int fLadder = det/1e+10;
        det -= fLadder*1e+10;
        --fLadder;
//                fLadder += fPhiSec*(_lay->nLaddersPerSector());

        //Return the Channel RO
        int fSubShell = det/1e+8;
        det -= fSubShell*1e+8;
        --fSubShell;

        int fChFstSd = det/1e+4;
        det -= fChFstSd*1e+4;
        --fChFstSd;

        int fChSndSd = det;

        //Call the upper method
        /*ROGeometryHandle::*/SelectRO(fLayer,fPhiSec,fLadder,fChFstSd,fChSndSd,fSubShell);
  } else {
      exc::exceptionG4 e("GEOM","Waring",4);
      e<<"Requestesd Channel ID exceed the maximum allowed number\n";
      e.error();
  }

}

void CSMBTROGeometryHandle::SelectRO(int chambID, int channelID) {

//          _chambID=chambID;

  // Return the Layer
  int fLayer = chambID/1e+4;
  chambID -= ((fLayer)*1e+4);
  --fLayer;
  //                _lay = pxstdetector->getLayer(fLayer);

  //Return the Ladder
  int fPhiSec = chambID/1e+2;
  chambID -= fPhiSec*1e+2;
  --fPhiSec;

  int fLadder = chambID;
  --fLadder;

  //Return the channelID RO
  int fSubShell = channelID/1e+8;
  channelID -= fSubShell*1e+8;
  --fSubShell;

  int fChFstSd = channelID/1e+4;
  channelID -= fChFstSd*1e+4;
  --fChFstSd;

  int fChSndSd = channelID;


  /*ROGeometryHandle::*/SelectRO(fLayer,fPhiSec,fLadder,fChFstSd,fChSndSd,fSubShell);
}

unsigned long CSMBTROGeometryHandle::computeDet(int Layer, int PhiSec, int Ladder, int ChFstSd,int ChSndSd, int subShell) {
        unsigned long det = 0;
        if (subShell>98) {
          exc::exceptionG4 e("GEOM","Error",4);
          e<<"Requestesd Sub Ladder Shell ID exceed the maximum allowed number (99)\n";
          e.error();
        }
        det = ((Layer+1)*1e+14) + ((PhiSec+1)*1e+12) + ((Ladder+1)*1e+10) + ((subShell+1)*1e+8) + ((ChFstSd+1)*1e+4) + ChSndSd ;
        return det;
}

unsigned long CSMBTROGeometryHandle::computeDet(int chambID, int channelID) {
        unsigned long det = 0;
        det = ((chambID/*+1*/)*1e+10) + channelID ;
        return det;
}

int CSMBTROGeometryHandle::computeChannelID(int ChFstSd,int ChSndSd, int subShell) {
        int channelID = 0;
        if (subShell>98) {
          exc::exceptionG4 e("GEOM","Error",4);
          e<<"Requestesd Sub Ladder Shell ID exceed the maximum allowed number (99)\n";
          e.error();
        }
        channelID = (subShell+1)*1e+8 + (ChFstSd+1)*10000 + ChSndSd;
        return channelID;
}

}
