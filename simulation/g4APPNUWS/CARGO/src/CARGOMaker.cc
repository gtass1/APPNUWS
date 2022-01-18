// CARGO geometry maker
//
// Original author G. Tassielli
//

#include "SimpleConfig.hh"
#include "manageException.hh"
//#include "messagefacility/MessageLogger/MessageLogger.h"
#include "GeomHandle.hh"

#include "CLHEP/Vector/RotationY.h"
#include "CLHEP/Vector/RotationZ.h"

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

#include "CARGOMaker.hh"

#include "GENELMbsBoxkindId.hh"
#include "GENELMbsBoxkindDetail.hh"
#include "GENELMbsCylkindId.hh"
#include "GENELMbsCylkindDetail.hh"

#include "TRUCKdescription.hh"

//using CLHEP::Hep3Vector;
//using CLHEP::HepRotationY;
//using CLHEP::HepRotationZ;

namespace crg {

CARGOMaker::CARGOMaker( crd::SimpleConfig const& config) {

        _isExternal     = false;
        _extFile        = config.getString("floor.extFile");
        if ( _extFile.size()>1 && ( _extFile.find_last_of(".gdml") || _extFile.find_last_of(".GDML") )!=0 ) _isExternal = true;

        _geomType       = config.getInt("floor.geomType");

        load(config);

        std::cout<<"------------------- CARGO-------------------"<<std::endl;
//        cout<<"distIn "<<_distIn<<" distOut "<<_distOut<<" leng "<<2.0*_halfLength<<endl;
//        cout<<"nLayers "<<_nLayers<<endl;
//        unsigned long totNumRO = 0;
//        for (int il=0; il<_nLayers; ++il) {
//          cout<<"Ly: "<<il<<endl;
//          cout<<"InR "<<_LayersVertPos[il]<<" Thick "<<_LaddersThickness[il]<<endl;
//          cout<<"nRoX "<<_nROsFstSdPerLadder[il]<<" nRoY "<<_nROsSndSdPerLadder[il]<<" nRo "<<_nROsPerLadder[il]<<endl;
//          totNumRO +=  _nROsPerLadder[il]*_nLaddersPerFstSide[il]*_nLaddersPerSndSide[il];
//        }
//
//        cout<<"Tot Numb of Channels "<<totNumRO<<endl;
//        cout<<"nAbsorber Layers "<<_nAbsorbLayers<<endl;

        // Do the real work.
        Build( );
}

CARGOMaker::~CARGOMaker (){}

void CARGOMaker::load( crd::SimpleConfig const& config ){

  _nBxKindEl                          = config.getInt("cargo.nBxKindEl");
  config.getVectorDouble("cargo.BxKindLength", _BxKindLength, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindWidth", _BxKindWidth, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindHeight", _BxKindHeight, _nBxKindEl);
  config.getVectorInt("cargo.BxKindShape", _BxKindShape, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindThickness", _BxKindThickness, _nBxKindEl);
  config.getVectorString("cargo.BxKindEqMaterial", _BxKindEqMaterial, _nBxKindEl);
  config.getVectorInt("cargo.BxKindIsTarget", _BxKindIsTarget, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindPosX", _BxKindPosX, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindPosY", _BxKindPosY, _nBxKindEl);
  config.getVectorDouble("cargo.BxKindPosZ", _BxKindPosZ, _nBxKindEl);


  _nCylKindEl                          = config.getInt("cargo.nCylKindEl");
  config.getVectorDouble("cargo.CylKindLength", _CylKindLength, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindInnerRadius", _CylKindInnerRadius, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindOuterRadius", _CylKindOuterRadius, _nCylKindEl);
  config.getVectorInt("cargo.CylKindShape", _CylKindShape, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindThickness", _CylKindThickness, _nCylKindEl);
  config.getVectorInt("cargo.CylKindOrientation",_CylKindOrientation, _nCylKindEl);
  config.getVectorString("cargo.CylKindEqMaterial", _CylKindEqMaterial, _nCylKindEl);
  config.getVectorInt("cargo.CylKindIsTarget", _CylKindIsTarget, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindPosX", _CylKindPosX, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindPosY", _CylKindPosY, _nCylKindEl);
  config.getVectorDouble("cargo.CylKindPosZ", _CylKindPosZ, _nCylKindEl);

//  char tmpVarName[50];
  if (_geomType==0) {

    GeomHandle<trck::TRUCKdescription> trckdesc;

    double cntnrHLength = trckdesc->getContainerElem()->getDetail()->halfLength();
    double cntnrHWidth = trckdesc->getContainerElem()->getDetail()->halfWidth();
    double cntnrHHeight = trckdesc->getContainerElem()->getDetail()->halfHeight();

    if (_nBxKindEl>0) {

      for (int ibx=0; ibx<_nBxKindEl; ++ibx) {
        _BxKindPosX[ibx]-=cntnrHWidth;
        _BxKindPosY[ibx]-=cntnrHHeight;
        _BxKindPosZ[ibx]-=cntnrHLength;

        if ( (_BxKindPosX[ibx]-_BxKindWidth[ibx]/2.0)<-cntnrHWidth || (_BxKindPosX[ibx]+_BxKindWidth[ibx]/2.0)>cntnrHWidth ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Box element: "<<ibx<<" extrudes out of the container space on X direction\n";
          e.error();
        }

        if ( (_BxKindPosY[ibx]-_BxKindHeight[ibx]/2.0)<-cntnrHHeight || (_BxKindPosY[ibx]+_BxKindHeight[ibx]/2.0)>cntnrHHeight ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Box element: "<<ibx<<" extrudes out of the container space on Y direction\n";
          e.error();
        }

        if ( (_BxKindPosZ[ibx]-_BxKindLength[ibx]/2.0)<-cntnrHLength || (_BxKindPosZ[ibx]+_BxKindLength[ibx]/2.0)>cntnrHLength ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Box element: "<<ibx<<" extrudes out of the container space on Z direction\n";
          e.error();
        }

      }

    }

    if (_nCylKindEl>0) {

      for (int icyl=0; icyl<_nCylKindEl; ++icyl) {
        _CylKindPosX[icyl]-=cntnrHWidth;
        _CylKindPosY[icyl]-=cntnrHHeight;
        _CylKindPosZ[icyl]-=cntnrHLength;

        double cyldimX=0.0, cyldimY=0.0, cyldimZ=0.0;
        if (_CylKindOrientation[icyl]==0) {
          cyldimX=_CylKindOuterRadius[icyl];
          cyldimY=_CylKindOuterRadius[icyl];
          cyldimZ=_CylKindLength[icyl]/2.0;
        } else if (_CylKindOrientation[icyl]==1) {
          cyldimX=_CylKindOuterRadius[icyl];
          cyldimY=_CylKindLength[icyl]/2.0;
          cyldimZ=_CylKindOuterRadius[icyl];
        } else if (_CylKindOrientation[icyl]==2) {
          cyldimX=_CylKindLength[icyl]/2.0;
          cyldimY=_CylKindOuterRadius[icyl];
          cyldimZ=_CylKindOuterRadius[icyl];
        }

        if ( (_CylKindPosX[icyl]-cyldimX)<-cntnrHWidth || (_CylKindPosX[icyl]+cyldimX)>cntnrHWidth ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Cylinder element: "<<icyl<<" extrudes out of the container space on X direction\n";
          e.error();
        }

        if ( (_CylKindPosY[icyl]-cyldimY)<-cntnrHHeight || (_CylKindPosY[icyl]+cyldimY)>cntnrHHeight ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Cylinder element: "<<icyl<<" extrudes out of the container space on Y direction\n";
          e.error();
        }

        if ( (_CylKindPosZ[icyl]-cyldimZ)<-cntnrHLength || (_CylKindPosZ[icyl]+cyldimZ)>cntnrHLength ) {
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"CARGO: Cylinder element: "<<icyl<<" extrudes out of the container space on Z direction\n";
          e.error();
        }

      }

    }


  } //geom 00

}

void CARGOMaker::Build(){

  _lcrgd = std::unique_ptr<CARGOdescription>(new CARGOdescription());
  _lcrgd->_isExternal = _isExternal;
  _lcrgd->_geomType=_geomType;

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CARGO: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {

    //------------------ Box elements description -----------------------------
    _lcrgd->_nCrgBx = _nBxKindEl;
    genelmbs::Boxkind *_crgBx = new genelmbs::Boxkind[_nBxKindEl];

    for (int ibx=0; ibx<_nBxKindEl; ++ibx) {

      genelmbs::Boxkind &crgBx = _crgBx[ibx];
      crgBx._elmName="cargoBox";
      crgBx._id = genelmbs::BoxkindId(ibx);
      crgBx._shapeType = static_cast<genelmbs::Boxkind::BxShpType>(_BxKindShape[ibx]);
      crgBx._detail.reset( new genelmbs::BoxkindDetail(_BxKindLength[ibx]/2.0, _BxKindWidth[ibx]/2.0, _BxKindHeight[ibx]/2.0, _BxKindEqMaterial[ibx],
                                                       _BxKindThickness[ibx] ) );
      HepGeom::Translate3D bxpos (_BxKindPosX[ibx],
          _BxKindPosY[ibx],
          _BxKindPosZ[ibx]);
      crgBx.setPosMatrix ( bxpos );

      if (_BxKindIsTarget[ibx]) {
        ++(_lcrgd->_nCrgBxTrgt);
        _lcrgd->_crgBxTrgtId.push_back(ibx);
      }

    }

    //------------------ Cylinder elements description -----------------------------
    _lcrgd->_nCrgCyl = _nCylKindEl;
    genelmbs::Cylkind *_crgCyl = new genelmbs::Cylkind[_nCylKindEl];

    for (int icyl=0; icyl<_nCylKindEl; ++icyl) {

      genelmbs::Cylkind &crgCyl = _crgCyl[icyl];
      crgCyl._elmName="cargoCyl";
      crgCyl._id = genelmbs::CylkindId(icyl);
      crgCyl._shapeType = static_cast<genelmbs::Cylkind::CylShpType>(_CylKindShape[icyl]);
      crgCyl._detail.reset( new genelmbs::CylkindDetail(_CylKindLength[icyl]/2.0, _CylKindInnerRadius[icyl], _CylKindOuterRadius[icyl], _CylKindEqMaterial[icyl],
                                                       _CylKindThickness[icyl] ) );

      HepGeom::Translate3D cylpos (_CylKindPosX[icyl],
          _CylKindPosY[icyl],
          _CylKindPosZ[icyl]);
      if (_CylKindOrientation[icyl]==0) {
        crgCyl.setPosMatrix ( cylpos );
      } else if (_CylKindOrientation[icyl]==1) {
        HepGeom::RotateX3D cylR(CLHEP::halfpi);
        HepGeom::Transform3D cylTransf (cylpos*cylR);
        crgCyl.setPosMatrix ( cylTransf );
      } else if (_CylKindOrientation[icyl]==2) {
        HepGeom::RotateY3D cylR(CLHEP::halfpi);
        HepGeom::Transform3D cylTransf (cylpos*cylR);
        crgCyl.setPosMatrix ( cylTransf );
      }

      if (_CylKindIsTarget[icyl]) {
        ++(_lcrgd->_nCrgCylTrgt);
        _lcrgd->_crgCylTrgtId.push_back(icyl);
      }

    }

    _lcrgd->_crgBx.reset(_crgBx);
    _lcrgd->_crgCyl.reset(_crgCyl);

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

} // namespace crg

//#endif
