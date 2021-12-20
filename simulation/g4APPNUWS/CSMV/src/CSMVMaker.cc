// CSMV geometry maker
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


#include "PXSTbsLayer.hh"
#include "PXSTbsLadderId.hh"

#include "CSMVROGeometryHandle.hh"
#include "CSMVMaker.hh"
//#include "CSMVAbsorber.hh"
//#include "CSMVtracker.hh"

//#ifndef __CINT__

using CLHEP::Hep3Vector;
using CLHEP::HepRotationY;
using CLHEP::HepRotationZ;

using namespace std;
//using namespace svx;

namespace csmv {

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
CSMVMaker::CSMVMaker( crd::SimpleConfig const& config):
                                                    _center(){

        _isExternal     = false;
        _extFile        = config.getString("csmv.extFile");
        if ( _extFile.size()>1 && ( _extFile.find_last_of(".gdml") || _extFile.find_last_of(".GDML") )!=0 ) _isExternal = true;

        _z0             = config.getDouble("csmv.z0");
        _geomType       = config.getInt("csmv.geomType");

        _r0=1.0e+6;
        _halfLength=0.0;
        _rOut=0.0;

        loadBarrelTracker(config);

        _r0_fwd=1.0e+6;
        _halfLength_fwd=1.0e+6;
        _rOut_fwd=0.0;
        _zPos_fwd=0.0;

        loadForwardTracker(config);

        loadBarrelAbsorber(config);

        loadForwardAbsorber(config);

        if (_nFwdLayers>0) {
          _halfLength_fwd=(_zPos_fwd-_halfLength_fwd)*0.5;
          _zPos_fwd-=_halfLength_fwd;
        }

        cout<<"------------------- CSMV -------------------"<<endl;
        cout<<"r0 "<<_r0<<" rOut "<<_rOut<<" leng "<<2.0*_halfLength<<endl;
        cout<<"nLayers "<<_nLayers<<endl;
        unsigned long totNumRO = 0;
        for (int il=0; il<_nLayers; ++il) {
          cout<<"Ly: "<<il<<endl;
          cout<<"InR "<<_LayersInnerRad[il]<<" Thick "<<_LaddersThickness[il]<<endl;
          cout<<"nRoX "<<_nROsFstSdPerLadder[il]<<" nRoY "<<_nROsSndSdPerLadder[il]<<" nRo "<<_nROsPerLadder[il]<<endl;
          totNumRO +=  _nROsPerLadder[il]*_nLaddersPerSector[il]*_nPhiSectors[il];
        }
        cout<<"nFwdLayers "<<_nFwdLayers<<endl;
        for (int il=0; il<_nFwdLayers; ++il) {
          cout<<"Ly: "<<_nLayers+il<<endl;
          cout<<"InR "<<_LayersInnerRad_fwd[il]<<" Thick "<<_LaddersThickness_fwd[il]<<endl;
          for( int ilad=0; ilad<_nLaddersPerSector_fwd[il]; ++ilad ){
            cout<<"Lad: "<<ilad<<endl;
            cout<<"nRoX "<<_nROsFstSdShrtPerLadder_fwd[il][ilad]<<" nRoX2 "<<_nROsFstSdLngPerLadder_fwd[il][ilad]<<" nRoY "<<_nROsSndSdPerLadder_fwd[il][ilad]<<" nRo "<<_nROsPerLadder_fwd[il][ilad]<<endl;
            totNumRO +=  _nROsPerLadder_fwd[il][ilad]*_nPhiSectors_fwd[il];
          }
        }

        cout<<"Tot Numb of Channels "<<totNumRO<<endl;
        cout<<"nAbsorber Layers "<<_nAbsorbLayers<<endl;

//        _r0-=1*CLHEP::mm;
//        _rOut+=1*CLHEP::mm;
//        _halfLength+=1*CLHEP::mm;


        // Do the real work.
        Build( );
        BuildAbsorber();
}

CSMVMaker::~CSMVMaker (){}

void CSMVMaker::loadBarrelTracker( crd::SimpleConfig const& config ){

  _nLayers        = config.getInt("csmv.Brl.nLayers");

  config.getVectorDouble("csmv.Brl.LayersInRad", _LayersInnerRad, _nLayers);
  config.getVectorInt("csmv.Brl.nPhiSectors", _nPhiSectors, _nLayers);
  config.getVectorInt("csmv.Brl.nLaddersPerSector",_nLaddersPerSector,_nLayers);
  config.getVectorDouble("csmv.Brl.LaddersHalfLength",_LaddersHalfLength,_nLayers);
  config.getVectorDouble("csmv.Brl.LaddersThickness",_LaddersThickness,_nLayers);
  config.getVectorInt("csmv.Brl.ROTypes",_ROTypes,_nLayers);
  config.getVectorDouble("csmv.Brl.ROfirstSideDim",_ROfirstSideDim,_nLayers);
  config.getVectorDouble("csmv.Brl.ROfirstSideInsul",_ROfirstSideInsul,_nLayers);
  config.getVectorDouble("csmv.Brl.ROSecondSideDim",_ROSecondSideDim,_nLayers);
  config.getVectorDouble("csmv.Brl.ROSecondSideInsul",_ROSecondSideInsul,_nLayers);
  config.getVectorDouble("csmv.Brl.ROangle",_ROangle,_nLayers);

 char tmpVarName[50];

  for (int il=0; il<_nLayers; ++il) {

    _phiAngles.push_back( CLHEP::twopi/((double)_nPhiSectors[il]) );
    if (_LayersInnerRad[il]<_r0) { _r0=_LayersInnerRad[il]; }
    double il_rOut=(_LayersInnerRad[il]+_LaddersThickness[il])/cos(0.5*_phiAngles[il]);
    if (il_rOut>_rOut) { _rOut=il_rOut; }
    if (_nLaddersPerSector[il]*_LaddersHalfLength[il]>_halfLength) { _halfLength=_nLaddersPerSector[il]*_LaddersHalfLength[il]; }

    //Ladders Parameters
    sprintf(tmpVarName,"csmv.Brl.l%d.ld.nShells",il+1);
    const std::string nShlVarName(tmpVarName);
    _LaddersNmShells.push_back( config.getInt(nShlVarName) );

    sprintf(tmpVarName,"csmv.Brl.l%d.ld.ShellsMaterial",il+1);
    const std::string ShlMatVarName(tmpVarName);
    std::vector< std::string > tmpldMat;
    config.getVectorString(ShlMatVarName,tmpldMat,_LaddersNmShells[il]);
    _LaddersShellsMaterial.push_back( tmpldMat );

    sprintf(tmpVarName,"csmv.Brl.l%d.ld.ShellsThickness",il+1);
    const std::string ShlThickVarName(tmpVarName);
    std::vector<double> tmpldThick;
    config.getVectorDouble(ShlThickVarName,tmpldThick,_LaddersNmShells[il]);
    _LaddersShellsThick.push_back( tmpldThick );

    //readOuts Parameters
    sprintf(tmpVarName,"csmv.Brl.l%d.ro.nShells",il+1);
    const std::string nROShlVarName(tmpVarName);
    _ReadoutsNmShells.push_back( config.getInt(nROShlVarName) );

    sprintf(tmpVarName,"csmv.Brl.l%d.ro.ShellIds",il+1);
    const std::string ROShlIdsVarName(tmpVarName);
    std::vector<int> tmproIds;
    config.getVectorInt(ROShlIdsVarName,tmproIds,_ReadoutsNmShells[il]);
    _ReadOutsShellIds.push_back( tmproIds );

  }

  if (_geomType==0) {
    for (int il=0; il<_nLayers; ++il) {
//            double FsrSdLadderDim = CLHEP::twopi*(_LayersInnerRad[il]+_LaddersThickness[il])/((double)_nPhiSectors[il]);
      double FsrSdLadderDim = 2.0*_LayersInnerRad[il]*tan( 0.5*_phiAngles[il] );
      _LaddersWidth.push_back(FsrSdLadderDim);
      unsigned int nROFstSdperLad = ((FsrSdLadderDim-_ROfirstSideInsul[il])/(_ROfirstSideDim[il]+_ROfirstSideInsul[il]));
      _nROsFstSdPerLadder.push_back( nROFstSdperLad );
      unsigned int nROSndSdperLad = ((2.0*_LaddersHalfLength[il]-_ROSecondSideInsul[il])/(_ROSecondSideDim[il]+_ROSecondSideInsul[il]));
      _nROsSndSdPerLadder.push_back( nROSndSdperLad );
      unsigned long nROperLad = nROFstSdperLad;
      nROperLad *= nROSndSdperLad;
      _nROsPerLadder.push_back( nROperLad );
      if (nROFstSdperLad>10000 || nROSndSdperLad>10000) {
        //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
        exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
        e<<"CSMV: Maximum number of Channels allowed per X or Y per Ladder is 10000!\n";
        e.error();

      }
    }
  }

}

void CSMVMaker::loadForwardTracker( crd::SimpleConfig const& config ){

  _nFwdLayers        = config.getInt("csmv.Fwd.nLayers",0);

  if (_nFwdLayers==0) {
    std::cout<<"CSMV: no Forward layers present!"<<std::endl;
    return;
  }
  config.getVectorDouble("csmv.Fwd.LayersInRad", _LayersInnerRad_fwd, _nFwdLayers);
  config.getVectorDouble("csmv.Fwd.LayersZPos", _LayersZPos_fwd, _nFwdLayers);
//  config.getVectorDouble("csmv.Fwd.LayersHeight",_LayersHeight_fwd,_nFwdLayers);
  config.getVectorInt("csmv.Fwd.nPhiSectors", _nPhiSectors_fwd, _nFwdLayers);
  config.getVectorInt("csmv.Fwd.nLaddersPerSector",_nLaddersPerSector_fwd,_nFwdLayers);
//  config.getVectorDouble("csmv.Fwd.LaddersHeight",_LaddersHeight_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.LaddersThickness",_LaddersThickness_fwd,_nFwdLayers);
  config.getVectorInt("csmv.Fwd.ROTypes",_ROTypes_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.ROfirstSideDim",_ROfirstSideDim_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.ROfirstSideInsul",_ROfirstSideInsul_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.ROSecondSideDim",_ROSecondSideDim_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.ROSecondSideInsul",_ROSecondSideInsul_fwd,_nFwdLayers);
  config.getVectorDouble("csmv.Fwd.ROangle",_ROangle_fwd,_nFwdLayers);

  char tmpVarName[50];

  for (int il=0; il<_nFwdLayers; ++il) {

    _phiAngles_fwd.push_back( CLHEP::twopi/((double)_nPhiSectors_fwd[il]) );
    if (_LayersInnerRad_fwd[il]<_r0_fwd) { _r0_fwd=_LayersInnerRad_fwd[il]; }
    double tmpZmin = _LayersZPos_fwd[il]-0.5*_LaddersThickness_fwd[il];
    double tmpZmax = _LayersZPos_fwd[il]+0.5*_LaddersThickness_fwd[il];
    if (tmpZmin<_halfLength_fwd) { _halfLength_fwd=tmpZmin; }
    if (tmpZmax>_zPos_fwd) { _zPos_fwd=tmpZmax; }

    //Ladders Parameters

    sprintf(tmpVarName,"csmv.Fwd.l%d.ld.LaddersHeight",il+1);
    const std::string LdHeightVarName(tmpVarName);
    std::vector<double> tmpldHeight;
    config.getVectorDouble(LdHeightVarName,tmpldHeight,_nLaddersPerSector_fwd[il]);
    _LaddersHeight_fwd.push_back( tmpldHeight );

    double tmpLyHeight=0.0;
    for (int iLd=0; iLd<_nLaddersPerSector_fwd[il]; ++iLd) { tmpLyHeight+=_LaddersHeight_fwd[il][iLd]; }
    _LayersHeight_fwd.push_back(tmpLyHeight);

    sprintf(tmpVarName,"csmv.Fwd.l%d.ld.nShells",il+1);
    const std::string nShlVarName(tmpVarName);
    _LaddersNmShells_fwd.push_back( config.getInt(nShlVarName) );

    sprintf(tmpVarName,"csmv.Fwd.l%d.ld.ShellsMaterial",il+1);
    const std::string ShlMatVarName(tmpVarName);
    std::vector< std::string > tmpldMat;
    config.getVectorString(ShlMatVarName,tmpldMat,_LaddersNmShells_fwd[il]);
    _LaddersShellsMaterial_fwd.push_back( tmpldMat );

    sprintf(tmpVarName,"csmv.Fwd.l%d.ld.ShellsThickness",il+1);
    const std::string ShlThickVarName(tmpVarName);
    std::vector<double> tmpldThick;
    config.getVectorDouble(ShlThickVarName,tmpldThick,_LaddersNmShells_fwd[il]);
    _LaddersShellsThick_fwd.push_back( tmpldThick );

    //readOuts Parameters
    sprintf(tmpVarName,"csmv.Fwd.l%d.ro.nShells",il+1);
    const std::string nROShlVarName(tmpVarName);
    _ReadoutsNmShells_fwd.push_back( config.getInt(nROShlVarName) );

    sprintf(tmpVarName,"csmv.Fwd.l%d.ro.ShellIds",il+1);
    const std::string ROShlIdsVarName(tmpVarName);
    std::vector<int> tmproIds;
    config.getVectorInt(ROShlIdsVarName,tmproIds,_ReadoutsNmShells_fwd[il]);
    _ReadOutsShellIds_fwd.push_back( tmproIds );

    //
    double tmpROut = (_LayersInnerRad_fwd[il]+_LayersHeight_fwd[il])/cos(0.5*_phiAngles_fwd[il]);
    if (tmpROut>_rOut_fwd) { _rOut_fwd=tmpROut; }
  }

  if (_geomType==0) {
    for (int il=0; il<_nFwdLayers; ++il) {
      std::vector<double> LdsShrtSdDim;
      std::vector<double> LdsLngSdDim;
      std::vector<unsigned int> nROsFstSdShrtPerLd;
      std::vector<unsigned int> nROsFstSdLngPerLd;
      std::vector<unsigned int> nROsSndSdPerLd;
      std::vector<int> LaddersNRwDltRO;
      std::vector<int> LaddersNRwAddDltRO;
      std::vector<unsigned long> nROsPerLd;

//      cout<<"il "<<il<<endl;
//      cout<<_nLaddersPerSector_fwd[il]<<endl;
//      cout<<_LayersInnerRad_fwd[il]<<endl;
//      cout<<_phiAngles_fwd[il]<<endl;
//      cout<<_ROfirstSideDim_fwd[il]<<" "<<_ROfirstSideInsul_fwd[il]<<endl;
//      cout<<_ROSecondSideDim_fwd[il]<<" "<<_ROSecondSideInsul_fwd[il]<<endl;

      double tmpLdInnerRad=_LayersInnerRad_fwd[il];
      double FsrSdLadderLngDim = 2.0*tmpLdInnerRad*tan( 0.5*_phiAngles_fwd[il] );
      for (int iLd=0; iLd<_nLaddersPerSector_fwd[il]; ++iLd) {

//        cout<<" iLd "<<iLd<<endl;
//        cout<<_LaddersHeight_fwd[il][iLd]<<endl;

        double FsrSdLadderShrtDim = FsrSdLadderLngDim;
        tmpLdInnerRad += _LaddersHeight_fwd[il][iLd];
        FsrSdLadderLngDim = 2.0*tmpLdInnerRad*tan( 0.5*_phiAngles_fwd[il] );
        LdsShrtSdDim.push_back(FsrSdLadderShrtDim);
        LdsLngSdDim.push_back(FsrSdLadderLngDim);
        unsigned int nROFstSdShrtperLad = ((FsrSdLadderShrtDim-_ROfirstSideInsul_fwd[il])/(_ROfirstSideDim_fwd[il]+_ROfirstSideInsul_fwd[il]));
        nROsFstSdShrtPerLd.push_back( nROFstSdShrtperLad );
        unsigned int nROFstSdLngperLad = ((FsrSdLadderLngDim-_ROfirstSideInsul_fwd[il])/(_ROfirstSideDim_fwd[il]+_ROfirstSideInsul_fwd[il]));
        nROsFstSdLngPerLd.push_back( nROFstSdLngperLad );
        unsigned int nROSndSdperLad = ((_LaddersHeight_fwd[il][iLd]-_ROSecondSideInsul_fwd[il])/(_ROSecondSideDim_fwd[il]+_ROSecondSideInsul_fwd[il]));
        nROsSndSdPerLd.push_back( nROSndSdperLad );
//        cout<<_ROfirstSideDim_fwd[il]<<" "<<_ROfirstSideInsul_fwd[il]<<" "<<2.0*tan( 0.5*_phiAngles_fwd[il] )<<endl;
//        cout<< _ROSecondSideDim_fwd[il]<<" "<<_ROSecondSideInsul_fwd[il]<<endl;
        float lNRwPerOneDltRO = 1.0/( ( ( (_ROfirstSideDim_fwd[il]+_ROfirstSideInsul_fwd[il])/(2.0*tan( 0.5*_phiAngles_fwd[il]) ) ) / ( _ROSecondSideDim_fwd[il]+_ROSecondSideInsul_fwd[il] ) ) );
        int tmpNRwPerOneDltRO = floor(lNRwPerOneDltRO);
        int tmpNRwPerAddOneDltRO = 0;
        float tmpLeft = fabs(lNRwPerOneDltRO-tmpNRwPerOneDltRO);
//        cout<<"tmpLeft "<<tmpLeft<<endl;
        if (tmpLeft>0.0) {
          tmpLeft=1.0/tmpLeft;
          tmpNRwPerAddOneDltRO = floor( tmpLeft );
          if ( tmpNRwPerAddOneDltRO>0.0 ) { ++tmpNRwPerAddOneDltRO; }
          //if ( (tmpLeft-tmpNRwPerAddOneDltRO)>0.0 ) { ++tmpNRwPerAddOneDltRO; }
        }
//        if (tmpLeft>0.0) { tmpNRwPerAddOneDltRO = rint( 10.0*tmpLeft ); }
//        cout<<"tmpNRwPerOneDltRO "<<tmpNRwPerOneDltRO<<" tmpNRwPerAddOneDltRO "<<tmpNRwPerAddOneDltRO<<endl;
        LaddersNRwDltRO.push_back( tmpNRwPerOneDltRO );
        LaddersNRwAddDltRO.push_back( tmpNRwPerAddOneDltRO );
        unsigned long nROperLad = nROFstSdShrtperLad;
        unsigned long nROperRow = nROFstSdShrtperLad;
//        cout<<nROSndSdperLad<<" "<<tmpNRwPerOneDltRO<<endl;
//        int nDlROAdd=0;
        for (unsigned int iSdRo=1; iSdRo<nROSndSdperLad; ++iSdRo) {
          nROperRow+=tmpNRwPerOneDltRO;
//            if ( iSdRo%tmpNRwPerOneDltRO==0 ) { ++nROperRow; ++nDlROAdd; }
//            if ( tmpNRwPerAddOneDltRO!=0 && nDlROAdd>0 && nDlROAdd%tmpNRwPerAddOneDltRO==0 ) { ++nROperRow; }
          if ( tmpNRwPerAddOneDltRO!=0 && iSdRo%tmpNRwPerAddOneDltRO==0 ) { ++nROperRow; }
          nROperLad += nROperRow;
        }
        nROsPerLd.push_back( nROperLad );

        if (nROFstSdLngperLad>10000 || nROSndSdperLad>10000) {
          //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CSMV: Maximum number of Channels allowed per X or Y per Fwd Ladder is 10000!\n";
          e.error();

        }
      }
      _LaddersShrtSdDim_fwd.push_back( LdsShrtSdDim );
      _LaddersLngSdDim_fwd.push_back( LdsLngSdDim );
      _nROsFstSdShrtPerLadder_fwd.push_back( nROsFstSdShrtPerLd );
      _nROsFstSdLngPerLadder_fwd.push_back( nROsFstSdLngPerLd );
      _nROsSndSdPerLadder_fwd.push_back( nROsSndSdPerLd );
      _LaddersNRwDltRO_fwd.push_back( LaddersNRwDltRO );
      _LaddersNRwAddDltRO_fwd.push_back( LaddersNRwAddDltRO );
      _nROsPerLadder_fwd.push_back( nROsPerLd );
   }
  }
}

void CSMVMaker::Build(){

  _lpst = unique_ptr<CSMVtracker>(new CSMVtracker());
  _lpst->_isExternal = _isExternal;
  _lpst->_geomType=_geomType;

  if (_nLayers<1) {
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMV: Number of Layer is not acceptable\n";
    e.error();
  }

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMV: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {

    _lpst->_nLayers = _nLayers + 2*_nFwdLayers;
    _lpst->_nFwdLayers = _nFwdLayers;
    _lpst->_r0 = _r0;
    _lpst->_rOut = _rOut;
    _lpst->_z0 = _z0;
    _lpst->_zHalfLength = _halfLength;

    _lpst->_r0_fwd = _r0_fwd;
    _lpst->_halfLength_fwd = _halfLength_fwd;
    _lpst->_rOut_fwd = _rOut_fwd;
    _lpst->_zPos_fwd = _zPos_fwd;

    _lpst->_roChhnd.reset( new CSMVROGeometryHandle(_lpst.get()) );

    pxstbs::Layer *_lr     = new pxstbs::Layer[_nLayers + 2*_nFwdLayers];

    boost::shared_ptr<pxstbs::Ladder> lad;

    //         -----------------------------

    for ( int ily=0; ily<_nLayers; ++ily ) {
      std::cout <<"Building CSMV layer: "<<ily+1<<std::endl;

      _lr[ily]._id = pxstbs::LayerId(ily);
      double layHalfLength = _LaddersHalfLength[ily]*_nLaddersPerSector[ily];
      _lr[ily]._detail.reset( new pxstbs::LayerDetail( _LayersInnerRad[ily], (_LayersInnerRad[ily]+_LaddersThickness[ily])/cos(0.5*_phiAngles[ily]), layHalfLength, 0/*_z0*/ ) );
      _lr[ily]._nPhiSectors=_nPhiSectors[ily];
      _lr[ily]._nLaddersPerSector=_nLaddersPerSector[ily];

      long ldid=0;
      for (int isec=0; isec<_nPhiSectors[ily]; ++isec ) {

        ldid=(isec+1)*100;

        for( int ilad=0; ilad<_nLaddersPerSector[ily]; ++ilad ){
          //std::cout <<"Building ladder: "<<ilad+1<<" in Sector "<<isec+1<<std::endl;

          _lr[ily].addLadder(new pxstbs::Ladder());
          lad = _lr[ily]._ladders.back();
          lad->_detail.reset( new pxstbs::LadderDetail( _LaddersWidth[ily], _LaddersHalfLength[ily], _LaddersShellsThick[ily], _LaddersShellsMaterial[ily] ) );
          lad->_id = pxstbs::LadderId(&_lr[ily]._id, ldid);
          if (_ROTypes[ily]==1) { lad->_ladderType=pxstbs::Ladder::pixel; }
          else if (_ROTypes[ily]==2) { lad->_ladderType=pxstbs::Ladder::strip; }
          //lad->_ladderGeomType=pxstbs::Ladder::spherical;
          lad->_ladderGeomType=pxstbs::Ladder::plane;

          double ladZpos = layHalfLength -(2*ilad+1)*_LaddersHalfLength[ily];
//          double ladZpos = 0.0;

          HepGeom::Translate3D ladRZpos (_lr[ily].getDetail()->InnerRadius()+0.5*lad->_detail->thickness(),
                                         0.0,
                                         ladZpos); //in case of cylindrical ladder remove the X translation
          //                cout<<"iSec "<<isec<<" Angle "<<isec*_phiAngles[ily]<<endl;
          HepGeom::RotateZ3D ladPutVert(-CLHEP::halfpi);
          HepGeom::RotateZ3D ladPhipos (isec*_phiAngles[ily]);
          HepGeom::Transform3D ladTransf ( ladPhipos * ladRZpos * ladPutVert );
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

    for (int jly=0; jly<2*_nFwdLayers; ++jly) {
      int ily = jly%_nFwdLayers;
      bool isNegative=(jly>=_nFwdLayers);
      int ilyId=_nLayers+jly;
      std::cout <<"Building CSMV FW layer: "<<jly+1<<std::endl;

      _lr[ilyId]._id = pxstbs::LayerId(ilyId);
      double layHalfLength = 0.5*_LaddersThickness_fwd[ily];
      if (isNegative) {
        _lr[ilyId]._detail.reset( new pxstbs::LayerDetail( _LayersInnerRad_fwd[ily], (_LayersInnerRad_fwd[ily]+_LayersHeight_fwd[ily])/cos(0.5*_phiAngles_fwd[ily]), layHalfLength, /*_z0*/-_LayersZPos_fwd[ily] ) );
      } else {
        _lr[ilyId]._detail.reset( new pxstbs::LayerDetail( _LayersInnerRad_fwd[ily], (_LayersInnerRad_fwd[ily]+_LayersHeight_fwd[ily])/cos(0.5*_phiAngles_fwd[ily]), layHalfLength, /*_z0+*/_LayersZPos_fwd[ily] ) );
      }
      _lr[ilyId]._layerOrnt = pxstbs::Layer::vertical;
      _lr[ilyId]._layerZone = pxstbs::Layer::forward;
      _lr[ilyId]._nPhiSectors=_nPhiSectors_fwd[ily];
      _lr[ilyId]._nLaddersPerSector=_nLaddersPerSector_fwd[ily];


      long ldid=0;
//      for (int isec=1; isec<2/*_nPhiSectors_fwd[ily]*/; ++isec ) {
//
//        ldid=(isec+1)*100;

      for( int ilad=0; ilad<_nLaddersPerSector_fwd[ily]; ++ilad ){
        //std::cout <<"Building FW ladder: "<<ilad+1<<" in Sector "<<isec+1<<std::endl;

        for (int isec=0; isec<_nPhiSectors_fwd[ily]; ++isec ) {

          ldid=(isec+1)*100+ilad;

          _lr[ilyId].addLadder(new pxstbs::Ladder());
          lad = _lr[ilyId]._ladders.back();
          lad->_detail.reset( new pxstbs::LadderDetail( _LaddersShrtSdDim_fwd[ily][ilad], 0.5*_LaddersHeight_fwd[ily][ilad], _LaddersShellsThick_fwd[ily], _LaddersShellsMaterial_fwd[ily], _LaddersLngSdDim_fwd[ily][ilad] ) );
          lad->_id = pxstbs::LadderId(&_lr[ilyId]._id, ldid);
          if (_ROTypes_fwd[ily]==1) { lad->_ladderType=pxstbs::Ladder::pixel; }
          else if (_ROTypes_fwd[ily]==2) { lad->_ladderType=pxstbs::Ladder::strip; }
          //lad->_ladderGeomType=pxstbs::Ladder::spherical;
          lad->_ladderGeomType=pxstbs::Ladder::plane;

          //          double ladZpos = layHalfLength -(2*ilad+1)*_LaddersHalfLength[ily];
          double ladZpos = _LayersInnerRad_fwd[ily];
          for (int jlad=0; jlad<ilad; ++jlad) {
            ladZpos+=_LaddersHeight_fwd[ily][jlad];
          }
          ladZpos+=0.5*_LaddersHeight_fwd[ily][ilad];

          HepGeom::Translate3D ladRZpos (0.0,
              0.0,
              ladZpos); //in case of cylindrical ladder remove the X translation
          //                cout<<"iSec "<<isec<<" Angle "<<isec*_phiAngles[ily]<<endl;
          HepGeom::RotateZ3D ladPutVert1(CLHEP::halfpi);
          HepGeom::RotateX3D ladPutVert2(CLHEP::halfpi);
          HepGeom::RotateZ3D *putBackWd = new HepGeom::RotateZ3D(0.0);
          if (isNegative) { delete putBackWd; putBackWd=new HepGeom::RotateZ3D(CLHEP::pi); }
          HepGeom::RotateZ3D ladPhipos (isec*_phiAngles_fwd[ily]);
          HepGeom::TranslateZ3D layZpos(_lr[ilyId].getDetail()->zPosition());
          HepGeom::Transform3D ladTransf ( layZpos*ladPhipos * ladPutVert1* ladPutVert2 * ladRZpos * (*putBackWd) );
          lad->setPosMatrix ( ladTransf );

          lad->_ros.reset( new pxstbs::ReadOut() );
          lad->_ros->_detail.reset( new pxstbs::ReadOutDetail(
              _ROfirstSideDim_fwd[ily],_ROfirstSideInsul_fwd[ily],
              _ROSecondSideDim_fwd[ily],_ROSecondSideInsul_fwd[ily],
              _ROangle_fwd[ily],_ReadOutsShellIds_fwd[ily]
          )
          );
          lad->_nReadOuts=_nROsPerLadder_fwd[ily][ilad];
          lad->_nReadOutsFstSd=_nROsFstSdShrtPerLadder_fwd[ily][ilad];
          lad->_nReadOutsSndSd=_nROsSndSdPerLadder_fwd[ily][ilad];
          lad->_nReadOutsFstSd2=_nROsFstSdLngPerLadder_fwd[ily][ilad];
          lad->_nSndSdDltRO=_LaddersNRwDltRO_fwd[ily][ilad];
          lad->_nSndSdAddDltRO=_LaddersNRwAddDltRO_fwd[ily][ilad];

          //++ldid;
          delete putBackWd;
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

void CSMVMaker::loadBarrelAbsorber( crd::SimpleConfig const& config ){

  char tmpVarName[50];

  _nAbsorbLayers   = config.getInt("csmv.Brl.nAbsLayers",0);
  for (int il=0; il<_nAbsorbLayers; ++il) {
    //Absorber Layers Parameters
    sprintf(tmpVarName,"csmv.Brl.abs.l%d.InRad",il+1);
    const std::string inRadVarName(tmpVarName);
    _AbsorbInRasius.push_back(config.getDouble(inRadVarName));

    if (_AbsorbInRasius[il]<_r0) { _r0=_AbsorbInRasius[il]; }

    sprintf(tmpVarName,"csmv.Brl.abs.l%d.halfLength",il+1);
    const std::string halflVarName(tmpVarName);
    _AbsorbHalfLengths.push_back(config.getDouble(halflVarName));

    if (_AbsorbHalfLengths[il]>_halfLength) { _halfLength=_AbsorbHalfLengths[il]; }

    sprintf(tmpVarName,"csmv.Brl.abs.l%d.nShells",il+1);
    const std::string nShlVarName(tmpVarName);
    _AbsorbNmShells.push_back(config.getInt(nShlVarName));

    sprintf(tmpVarName,"csmv.Brl.abs.l%d.ShellsMaterial",il+1);
    const std::string AbShlMatVarName(tmpVarName);
    std::vector< std::string > tmpabMat;
    config.getVectorString(AbShlMatVarName,tmpabMat,_AbsorbNmShells[il]);
    _AbsorbShellsMaterial.push_back( tmpabMat );

    sprintf(tmpVarName,"csmv.Brl.abs.l%d.ShellsThickness",il+1);
    const std::string AbShlThickVarName(tmpVarName);
    std::vector<double> tmpabThick;
    config.getVectorDouble(AbShlThickVarName,tmpabThick,_AbsorbNmShells[il]);
    _AbsorbShellsThick.push_back( tmpabThick );

    double tmpThick=0.0;
    for (int iShl=0; iShl<_AbsorbNmShells[il]; ++iShl) {
      tmpThick += _AbsorbShellsThick[il][iShl];
    }
    _AbsorbersThickness.push_back(tmpThick);

    if ((_AbsorbInRasius[il]+_AbsorbersThickness[il])>_rOut) { _rOut=(_AbsorbInRasius[il]+_AbsorbersThickness[il]) ; }

  }

}

void CSMVMaker::loadForwardAbsorber( crd::SimpleConfig const& config ) {

  char tmpVarName[50];

  _nAbsorbLayers_fwd = config.getInt("csmv.Fwd.nAbsLayers",0);
  for (int il=0; il<_nAbsorbLayers_fwd; ++il) {
    //Absorber Layers Parameters
    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.InRad",il+1);
    const std::string inRadVarName(tmpVarName);
    _AbsorbInRasius_fwd.push_back(config.getDouble(inRadVarName));

    if (_AbsorbInRasius_fwd[il]<_r0_fwd) { _r0_fwd=_AbsorbInRasius_fwd[il]; }

    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.OutRad",il+1);
    const std::string outRadVarName(tmpVarName);
    _AbsorbOutRasius_fwd.push_back(config.getDouble(outRadVarName));

    if (_AbsorbOutRasius_fwd[il]>_rOut_fwd) { _rOut_fwd=_AbsorbOutRasius_fwd[il]; }

    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.zPos",il+1);
    const std::string zPosVarName(tmpVarName);
    _AbsorbZpos_fwd.push_back(config.getDouble(zPosVarName));

    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.nShells",il+1);
    const std::string nShlVarName(tmpVarName);
    _AbsorbNmShells_fwd.push_back(config.getInt(nShlVarName));

    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.ShellsMaterial",il+1);
    const std::string AbShlMatVarName(tmpVarName);
    std::vector< std::string > tmpabMat;
    config.getVectorString(AbShlMatVarName,tmpabMat,_AbsorbNmShells_fwd[il]);
    _AbsorbShellsMaterial_fwd.push_back( tmpabMat );

    sprintf(tmpVarName,"csmv.Fwd.abs.l%d.ShellsThickness",il+1);
    const std::string AbShlThickVarName(tmpVarName);
    std::vector<double> tmpabThick;
    config.getVectorDouble(AbShlThickVarName,tmpabThick,_AbsorbNmShells_fwd[il]);
    _AbsorbShellsThick_fwd.push_back( tmpabThick );

    double tmpThick=0.0;
    for (int iShl=0; iShl<_AbsorbNmShells_fwd[il]; ++iShl) {
      tmpThick += _AbsorbShellsThick_fwd[il][iShl];
    }
    _AbsorbersThickness_fwd.push_back(tmpThick);

    //_zPos_fwd-_halfLength_fwd
    double tmpMinZ = _AbsorbZpos_fwd[il]-0.5*_AbsorbersThickness_fwd[il];
    if (tmpMinZ<_halfLength_fwd) { _halfLength_fwd=tmpMinZ; }

    double tmpMaxZ = _AbsorbZpos_fwd[il]+0.5*_AbsorbersThickness_fwd[il];
    if (tmpMaxZ>_zPos_fwd) { _zPos_fwd=tmpMaxZ; }

  }

}

void CSMVMaker::BuildAbsorber(){

  _lpsabs = unique_ptr<CSMVAbsorber>(new CSMVAbsorber());

  if (_nAbsorbLayers<1) {
    exc::exceptionG4 e("GEOM","Warning in Argument",4);
    e<<"CSMV: Number of Absorber Shell is 0\n";
    e.error();
  }
  if (_nAbsorbLayers_fwd<0) {
    exc::exceptionG4 e("GEOM","Warning in Argument",4);
    e<<"CSMV: Number of Forwad Absorber Shell is negative\n";
    e.error();
  }

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMV: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {
    _lpsabs->_nAbsorbLayers = _nAbsorbLayers+_nAbsorbLayers_fwd;
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
    for (int ily=0; ily<_nAbsorbLayers_fwd; ++ily) {
      _lpsabs->_AbsorberType.push_back(1);
      _lpsabs->_AbsorbInRasius.push_back(_AbsorbInRasius_fwd[ily]);
      _lpsabs->_AbsorbOutRasius.push_back(_AbsorbOutRasius_fwd[ily]);
      _lpsabs->_AbsorbHalfLengths.push_back(_AbsorbZpos_fwd[ily]);
      _lpsabs->_AbsorbNmShells.push_back(_AbsorbNmShells_fwd[ily]);
      _lpsabs->_AbsorbShellsMaterial.push_back(_AbsorbShellsMaterial_fwd[ily]);
      _lpsabs->_AbsorbShellsThick.push_back(_AbsorbShellsThick_fwd[ily]);
      _lpsabs->_AbsorbersThickness.push_back(_AbsorbersThickness_fwd[ily]);
    }

  }
}

} // namespace csmv

//#endif
