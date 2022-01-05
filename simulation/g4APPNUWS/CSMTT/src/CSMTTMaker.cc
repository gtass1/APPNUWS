// CSMTT geometry maker
//
// Original author G. Tassielli
//

#include "SimpleConfig.hh"
#include "manageException.hh"
//#include "messagefacility/MessageLogger/MessageLogger.h"

#include "CLHEP/Vector/RotationY.h"
#include "CLHEP/Vector/RotationZ.h"

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

#include "CSMTTMaker.hh"
#include "CSMTTROGeometryHandle.hh"

#include "PXSTbsLayer.hh"
#include "PXSTbsLadderId.hh"

//#include "CSMTTAbsorber.hh"
//#include "CSMTtracker.hh"

//#ifndef __CINT__

using CLHEP::Hep3Vector;
using CLHEP::HepRotationY;
using CLHEP::HepRotationZ;

using namespace std;
//using namespace svx;

namespace csmtt {

//void cellPrinter( const Cell& s){
//        cout << s.Id() << endl;
//}
//
////void cellPrinter2( const Cell* s, int& i){
////        cout << s->Id() <<  " | "
////                        << s->hack << " "
////                        << ++i << endl;
////}
////
////void cellHacker( Cell* s, int& i){
////        s->hack = 2;
////}
//
//void layerPrinter( const SuperLayer& l){
//        cout << "    Layer: " << l.Id() << endl;
//}
//
// Constructor that gets information from the config file instead of
// from arguments.
CSMTTMaker::CSMTTMaker( crd::SimpleConfig const& config):
                                                    _center(){

        _isExternal     = false;
        _extFile        = config.getString("csmtt.extFile");
        if ( _extFile.size()>1 && ( _extFile.find_last_of(".gdml") || _extFile.find_last_of(".GDML") )!=0 ) _isExternal = true;

//        _z0             = config.getDouble("csmtt.z0");
        _geomType       = config.getInt("csmtt.geomType");

        _distIn=1.0e+6;
        _distOut=0.0;
        _halfLength=0.0;
        _halfWidth=0.0;

        loadTracker(config);

        loadAbsorber(config);

        cout<<"------------------- CCSMTT-------------------"<<endl;
        cout<<"distIn "<<_distIn<<" distOut "<<_distOut<<" leng "<<2.0*_halfLength<<endl;
        cout<<"nLayers "<<_nLayers<<endl;
        unsigned long totNumRO = 0;
        for (int il=0; il<_nLayers; ++il) {
          cout<<"Ly: "<<il<<endl;
          cout<<"InR "<<_LayersVertPos[il]<<" Thick "<<_LaddersThickness[il]<<endl;
          cout<<"nRoX "<<_nROsFstSdPerLadder[il]<<" nRoY "<<_nROsSndSdPerLadder[il]<<" nRo "<<_nROsPerLadder[il]<<endl;
          totNumRO +=  _nROsPerLadder[il]*_nLaddersPerFstSide[il]*_nLaddersPerSndSide[il];
        }

        cout<<"Tot Numb of Channels "<<totNumRO<<endl;
        cout<<"nAbsorber Layers "<<_nAbsorbLayers<<endl;

        // Do the real work.
        Build( );
        BuildAbsorber();
}

CSMTTMaker::~CSMTTMaker (){}

void CSMTTMaker::loadTracker( crd::SimpleConfig const& config ){

  _nLayers        = config.getInt("csmtt.nLayers");

  config.getVectorDouble("csmtt.LayersVertPos", _LayersVertPos, _nLayers);
  config.getVectorDouble("csmtt.LayersFstSidePos", _LayersFstSidePos, _nLayers);
  config.getVectorDouble("csmtt.LayersSndSidePos", _LayersSndSidePos, _nLayers);
  config.getVectorInt("csmtt.nLaddersPerFstSide", _nLaddersPerFstSide, _nLayers);
  config.getVectorInt("csmtt.nLaddersPerSndSide",_nLaddersPerSndSide,_nLayers);
  config.getVectorDouble("csmtt.LaddersFstSideDim", _LaddersFstSideDim, _nLayers);
  config.getVectorDouble("csmtt.LaddersDeadZoneFstSide", _LaddersDeadZoneFstSide, _nLayers);
  config.getVectorDouble("csmtt.SpaceBtwnLadFstSide", _SpaceBtwnLadFstSide, _nLayers);
  config.getVectorDouble("csmtt.LaddersSndSideDim", _LaddersSndSideDim, _nLayers);
  config.getVectorDouble("csmtt.LaddersDeadZoneSndSide", _LaddersDeadZoneSndSide, _nLayers);
  config.getVectorDouble("csmtt.SpaceBtwnLadSndSide", _SpaceBtwnLadSndSide, _nLayers);

  config.getVectorDouble("csmtt.LaddersTotThickness",_LaddersThickness,_nLayers);
  config.getVectorInt("csmtt.ROTypes",_ROTypes,_nLayers);
  config.getVectorDouble("csmtt.ROfirstSideDim",_ROfirstSideDim,_nLayers);
  config.getVectorDouble("csmtt.ROfirstSideInsul",_ROfirstSideInsul,_nLayers);
  config.getVectorDouble("csmtt.ROSecondSideDim",_ROSecondSideDim,_nLayers);
  config.getVectorDouble("csmtt.ROSecondSideInsul",_ROSecondSideInsul,_nLayers);
  config.getVectorDouble("csmtt.ROangle",_ROangle,_nLayers);

  char tmpVarName[50];

  if (_geomType==0) {

    double maxLength=0.0, minLength=1e+6;
    double maxWidth=0.0, minWidth=1e+6;

    _InvertLadderShellOrder = config.getBool("csmtt.InvertLadderShellOrder",false);

    for (int il=0; il<_nLayers; ++il) {

      if (_LayersVertPos[il]<_distIn) { _distIn=_LayersVertPos[il]; }
      double il_distOut=(_LayersVertPos[il]+_LaddersThickness[il]);
      if (il_distOut>_distOut) { _distOut=il_distOut; }
      double il_FstSideTotDim=_nLaddersPerFstSide[il]*_LaddersFstSideDim[il]+(_nLaddersPerFstSide[il]-1)*_SpaceBtwnLadFstSide[il];
      double il_SndSideTotDim=_nLaddersPerSndSide[il]*_LaddersSndSideDim[il]+(_nLaddersPerSndSide[il]-1)*_SpaceBtwnLadSndSide[il];
      il_FstSideTotDim+=_LayersFstSidePos[il];
      il_SndSideTotDim+=_LayersSndSidePos[il];
      if (il_FstSideTotDim>maxLength) { maxLength=il_FstSideTotDim; }
      if (il_SndSideTotDim>maxWidth) { maxWidth=il_SndSideTotDim; }
      if (_LayersFstSidePos[il]<minLength) { minLength=_LayersFstSidePos[il]; }
      if (_LayersSndSidePos[il]<minWidth) { minWidth=_LayersSndSidePos[il]; }

      //Ladders Parameters
      sprintf(tmpVarName,"csmtt.l%d.ld.nShells",il+1);
      const std::string nShlVarName(tmpVarName);
      _LaddersNmShells.push_back( config.getInt(nShlVarName) );

      sprintf(tmpVarName,"csmtt.l%d.ld.ShellsMaterial",il+1);
      const std::string ShlMatVarName(tmpVarName);
      std::vector< std::string > tmpldMat;
      config.getVectorString(ShlMatVarName,tmpldMat,_LaddersNmShells[il]);
      if (!_InvertLadderShellOrder) {
        _LaddersShellsMaterial.push_back( tmpldMat );
      } else {
        std::vector< std::string > tmpldMatInv(_LaddersNmShells[il]);
        for (int ish=0; ish<_LaddersNmShells[il]; ++ish) {
          tmpldMatInv[ish]=tmpldMat[_LaddersNmShells[il]-1-ish];
        }
        _LaddersShellsMaterial.push_back( tmpldMatInv );
      }

      sprintf(tmpVarName,"csmtt.l%d.ld.ShellsThickness",il+1);
      const std::string ShlThickVarName(tmpVarName);
      std::vector<double> tmpldThick;
      config.getVectorDouble(ShlThickVarName,tmpldThick,_LaddersNmShells[il]);
      if (!_InvertLadderShellOrder) {
        _LaddersShellsThick.push_back( tmpldThick );
      } else {
        std::vector<double> tmpldThickInv(_LaddersNmShells[il]);
        for (int ish=0; ish<_LaddersNmShells[il]; ++ish) {
          tmpldThickInv[ish]=tmpldThick[_LaddersNmShells[il]-1-ish];
        }
        _LaddersShellsThick.push_back( tmpldThickInv );
      }

      //readOuts Parameters
      sprintf(tmpVarName,"csmtt.l%d.ro.nShells",il+1);
      const std::string nROShlVarName(tmpVarName);
      _ReadoutsNmShells.push_back( config.getInt(nROShlVarName) );

      sprintf(tmpVarName,"csmtt.l%d.ro.ShellIds",il+1);
      const std::string ROShlIdsVarName(tmpVarName);
      std::vector<int> tmproIds;
      config.getVectorInt(ROShlIdsVarName,tmproIds,_ReadoutsNmShells[il]);
      if (!_InvertLadderShellOrder) {
        _ReadOutsShellIds.push_back( tmproIds );
      } else {
        std::vector<int> tmproIdsInv(_ReadoutsNmShells[il]);
        for (int ish=0; ish<_ReadoutsNmShells[il]; ++ish) {
          tmproIdsInv[ish]=_LaddersNmShells[il]-1-tmproIds[ish];
        }
        _ReadOutsShellIds.push_back( tmproIdsInv );
      }

    }
    _halfLength = (maxLength-minLength)*0.5;
    _halfWidth = (maxWidth-minWidth)*0.5;
    _halfThickness = (_distOut-_distIn)*0.5;
    _center.set(0.0,_distIn+_halfThickness,0.0);

    for (int il=0; il<_nLayers; ++il) {

      double FstdLadderDim = _LaddersFstSideDim[il]-2.0*_LaddersDeadZoneFstSide[il];
      unsigned int nROFstSdperLad = ((FstdLadderDim-_ROfirstSideInsul[il])/(_ROfirstSideDim[il]+_ROfirstSideInsul[il]));
      _nROsFstSdPerLadder.push_back( nROFstSdperLad );
      double SnddLadderDim = _LaddersSndSideDim[il]-2.0*_LaddersDeadZoneSndSide[il];
      unsigned int nROSndSdperLad = ((SnddLadderDim-_ROSecondSideInsul[il])/(_ROSecondSideDim[il]+_ROSecondSideInsul[il]));
      _nROsSndSdPerLadder.push_back( nROSndSdperLad );
      unsigned long nROperLad = nROFstSdperLad;
      nROperLad *= nROSndSdperLad;
      _nROsPerLadder.push_back( nROperLad );
      if (nROFstSdperLad>10000 || nROSndSdperLad>10000) {
        //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
        exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
        e<<"CSMTT: Maximum number of Channels allowed per X or Y per Ladder is 10000!\n";
        e.error();

      }
    }
  }

}

void CSMTTMaker::Build(){

  _lpst = unique_ptr<CSMTtracker>(new CSMTtracker());
  _lpst->_isExternal = _isExternal;
  _lpst->_geomType=_geomType;

  if (_nLayers<1) {
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMTT: Number of Layer is not acceptable\n";
    e.error();
  }

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMTT: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {

    _lpst->_nLayers = _nLayers;
    _lpst->_distIn = _distIn;
    _lpst->_distOut = _distOut;
    _lpst->_x0 = _center.x();
    _lpst->_y0 = _center.y();
    _lpst->_z0 = _center.z();
    _lpst->_zHalfLength = _halfLength;
    _lpst->_halfWidth = _halfWidth;
    _lpst->_halfThickness = _halfThickness;

    _lpst->_roChhnd.reset( new CSMTTROGeometryHandle(_lpst.get()) );

    pxstbs::Layer *_lr     = new pxstbs::Layer[_nLayers];

    boost::shared_ptr<pxstbs::Ladder> lad;

    //         -----------------------------

    for ( int ily=0; ily<_nLayers; ++ily ) {
      std::cout <<"Building CSMTT layer: "<<ily+1<<std::endl;

      _lr[ily]._id = pxstbs::LayerId(ily);
      double layHalfLength = _halfLength;
      _lr[ily]._detail.reset( new pxstbs::LayerDetail( _LayersVertPos[ily], _LayersVertPos[ily]+_LaddersThickness[ily], layHalfLength, _center.z(), _LayersVertPos[ily]+_LaddersThickness[ily]*0.5-_center.y() ) );
      _lr[ily]._nPhiSectors=_nLaddersPerSndSide[ily];  //The second side is stored inside the phi variable (used for cylindrical)
      _lr[ily]._nLaddersPerSector=_nLaddersPerFstSide[ily];
      _lr[ily]._layerOrnt = pxstbs::Layer::horizontal;


      long ldid=0;
      for (int isec=0; isec<_nLaddersPerSndSide[ily]; ++isec ) {

        ldid=(isec+1)*100;

        for( int ilad=0; ilad<_nLaddersPerFstSide[ily]; ++ilad ){
          //std::cout <<"Building ladder: "<<ilad+1<<" in Sector "<<isec+1<<std::endl;

          _lr[ily].addLadder(new pxstbs::Ladder());
          lad = _lr[ily]._ladders.back();
          lad->_detail.reset( new pxstbs::LadderDetail( _LaddersSndSideDim[ily], _LaddersFstSideDim[ily]*0.5, _LaddersShellsThick[ily], _LaddersShellsMaterial[ily] ) );
          lad->_id = pxstbs::LadderId(&_lr[ily]._id, ldid);
          if (_ROTypes[ily]==1) { lad->_ladderType=pxstbs::Ladder::pixel; }
          else if (_ROTypes[ily]==2) { lad->_ladderType=pxstbs::Ladder::strip; }
          //lad->_ladderGeomType=pxstbs::Ladder::spherical;
          lad->_ladderGeomType=pxstbs::Ladder::plane;

          double ladXpos = _LayersSndSidePos[ily]+_LaddersSndSideDim[ily]*0.5+isec*(_LaddersSndSideDim[ily]+_SpaceBtwnLadSndSide[ily]);
//          double ladYpos = _LayersVertPos[ily]+_LaddersThickness[ily]*0.5-_center.y();
          double ladZpos = _LayersFstSidePos[ily]+_LaddersFstSideDim[ily]*0.5+ilad*(_LaddersFstSideDim[ily]+_SpaceBtwnLadFstSide[ily]);

//          HepGeom::Translate3D ladRZpos (_lr[ily].getDetail()->InnerRadius()+0.5*lad->_detail->thickness(),
//                                         0.0,
//                                         ladZpos); //in case of cylindrical ladder remove the X translation
//          //                cout<<"iSec "<<isec<<" Angle "<<isec*_phiAngles[ily]<<endl;
//          HepGeom::RotateZ3D ladPutVert(-CLHEP::halfpi);
//          HepGeom::RotateZ3D ladPhipos (isec*_phiAngles[ily]);
//          HepGeom::Transform3D ladTransf ( ladPhipos * ladRZpos * ladPutVert );
          HepGeom::Translate3D ladTransf ( ladXpos, 0.0, ladZpos );
          lad->setPosMatrix ( ladTransf );

          lad->_ros.reset( new pxstbs::ReadOut() );
          lad->_ros->_detail.reset( new pxstbs::ReadOutDetail(
              _ROfirstSideDim[ily],_ROfirstSideInsul[ily],
              _ROSecondSideDim[ily],_ROSecondSideInsul[ily],
              _ROangle[ily],_ReadOutsShellIds[ily]
          )
          );
          lad->_nReadOuts=_nROsPerLadder[ily];
          lad->_nReadOutsFstSd=_nROsFstSdPerLadder[ily];
          lad->_nReadOutsSndSd=_nROsSndSdPerLadder[ily];

          ++ldid;
        }

      }

    }

    _lpst->_lr.reset(_lr);

//    for ( int ily=0; ily<_nLayers; ++ily ) {
//      Layer *iLy = _lpst->getLayer(ily);
//      cout<<iLy->Id()<<": Rin "<<iLy->getDetail()->InnerRadius()<<" ROut "<<iLy->getDetail()->OuterRadius()<<" HL "<<iLy->getDetail()->halfLength()<<" zPos "<<iLy->getDetail()->zPosition()<<" nLadder "<<iLy->nLadders()<<endl;
//      for (int ild=0; ild<iLy->nLadders(); ++ild) {
//        cout<<*iLy->getLadder(ild)<<endl;
//        for (unsigned long ird=0; ird<2.1*_nROsSndSdPerLadder[ily]; ++ird) {
//          cout<<iLy->getLadder(ild)->getROChLoclMidPoint(ird)<<endl;
//        }
//      }
//    }

  }

}

void CSMTTMaker::loadAbsorber( crd::SimpleConfig const& config ){

  char tmpVarName[50];

  _nAbsorbLayers   = config.getInt("csmtt.nAbsLayers",0);
  for (int il=0; il<_nAbsorbLayers; ++il) {
    //Absorber Layers Parameters
    sprintf(tmpVarName,"csmtt.abs.l%d.InRad",il+1);
    const std::string inRadVarName(tmpVarName);
    _AbsorbInRasius.push_back(config.getDouble(inRadVarName));

    if (_AbsorbInRasius[il]<_distIn) { _distIn=_AbsorbInRasius[il]; }

    sprintf(tmpVarName,"csmtt.abs.l%d.halfLength",il+1);
    const std::string halflVarName(tmpVarName);
    _AbsorbHalfLengths.push_back(config.getDouble(halflVarName));

    if (_AbsorbHalfLengths[il]>_halfLength) { _halfLength=_AbsorbHalfLengths[il]; }

    sprintf(tmpVarName,"csmtt.abs.l%d.nShells",il+1);
    const std::string nShlVarName(tmpVarName);
    _AbsorbNmShells.push_back(config.getInt(nShlVarName));

    sprintf(tmpVarName,"csmtt.abs.l%d.ShellsMaterial",il+1);
    const std::string AbShlMatVarName(tmpVarName);
    std::vector< std::string > tmpabMat;
    config.getVectorString(AbShlMatVarName,tmpabMat,_AbsorbNmShells[il]);
    _AbsorbShellsMaterial.push_back( tmpabMat );

    sprintf(tmpVarName,"csmtt.abs.l%d.ShellsThickness",il+1);
    const std::string AbShlThickVarName(tmpVarName);
    std::vector<double> tmpabThick;
    config.getVectorDouble(AbShlThickVarName,tmpabThick,_AbsorbNmShells[il]);
    _AbsorbShellsThick.push_back( tmpabThick );

    double tmpThick=0.0;
    for (int iShl=0; iShl<_AbsorbNmShells[il]; ++iShl) {
      tmpThick += _AbsorbShellsThick[il][iShl];
    }
    _AbsorbersThickness.push_back(tmpThick);

    if ((_AbsorbInRasius[il]+_AbsorbersThickness[il])>_distOut) { _distOut=(_AbsorbInRasius[il]+_AbsorbersThickness[il]) ; }

  }

}

void CSMTTMaker::BuildAbsorber(){

  _lpsabs = unique_ptr<CSMTTAbsorber>(new CSMTTAbsorber());

  if (_nAbsorbLayers<1) {
    exc::exceptionG4 e("GEOM","Warning in Argument",4);
    e<<"CSMTT: Number of Absorber Shell is 0\n";
    e.error();
  }

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMTT: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {
    _lpsabs->_nAbsorbLayers = _nAbsorbLayers;
    _lpsabs->_AbsorbInRasius = _AbsorbInRasius;
    _lpsabs->_AbsorbHalfLengths = _AbsorbHalfLengths;
    _lpsabs->_AbsorbNmShells = _AbsorbNmShells;
    _lpsabs->_AbsorbShellsMaterial = _AbsorbShellsMaterial;
    _lpsabs->_AbsorbShellsThick = _AbsorbShellsThick;
    _lpsabs->_AbsorbersThickness = _AbsorbersThickness;
    for (int ily=0; ily<_nAbsorbLayers; ++ily) {
      _lpsabs->_AbsorbOutRasius.push_back(0.0);
      _lpsabs->_AbsorberType.push_back(0);
    }
  }
}

} // namespace csmtt

//#endif
