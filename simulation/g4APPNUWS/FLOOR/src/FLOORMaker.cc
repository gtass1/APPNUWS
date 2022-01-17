// FLOOR geometry maker
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

#include "FLOORMaker.hh"

#include "GENELMbsBoxkindId.hh"
#include "GENELMbsBoxkindDetail.hh"
#include "GENELMbsCylkindId.hh"
#include "GENELMbsCylkindDetail.hh"

using CLHEP::Hep3Vector;
using CLHEP::HepRotationY;
using CLHEP::HepRotationZ;

namespace flr {

FLOORMaker::FLOORMaker( crd::SimpleConfig const& config):
                                                    _center(){

        _isExternal     = false;
        _extFile        = config.getString("floor.extFile");
        if ( _extFile.size()>1 && ( _extFile.find_last_of(".gdml") || _extFile.find_last_of(".GDML") )!=0 ) _isExternal = true;

        _center.setZ( config.getDouble("floor.z0",0.0) );
        _center.setX( config.getDouble("floor.x0",0.0) );
        _geomType       = config.getInt("floor.geomType");

        _halfLength=0.0;
        _halfWidth=0.0;
        _halfHeight=0.0;

        load(config);

//        loadAbsorber(config);

        std::cout<<"------------------- FLOOR-------------------"<<std::endl;
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

FLOORMaker::~FLOORMaker (){}

void FLOORMaker::load( crd::SimpleConfig const& config ){

  _Length                             = config.getDouble("floor.Length");
  _PlaneWidth                         = config.getDouble("floor.PlaneWidth",0.0);
  _PlaneThicknes                      = config.getDouble("floor.PlaneThicknes",0.0);
  _FullPlaneEqMaterial                = config.getString("floor.FullPlaneEqMaterial","");
  _nSkeletonShafts                    = config.getInt("floor.nSkeletonShafts",0);
  _SkeletonEqMaterial                 = config.getString("floor.SkeletonEqMaterial","");
  if (_nSkeletonShafts>0) {
    _SkeletonShaftShape               = static_cast<genelmbs::Boxkind::BxShpType> ( config.getInt("floor.SkeletonShaftShape",0) );
    _SkeletonShaftsInterDist          = config.getDouble("floor.SkeletonShaftsInterDist",0.0);
    if (_SkeletonShaftsInterDist<=0.0) {
      config.getVectorDouble("floor.SkeletonShaftsPos", _SkeletonShaftsPos, _nSkeletonShafts);
    }
    _SkeletonShaftWidth               = config.getDouble("floor.SkeletonShaftWidth");
    _SkeletonShaftHeight              = config.getDouble("floor.SkeletonShaftHeight");
    _SkeletonShaftThicknesExt         = config.getDouble("floor.SkeletonShaftThicknesExt");
    _SkeletonShaftThicknesCent        = config.getDouble("floor.SkeletonShaftThicknesCent");
  } else {
    _SkeletonShaftShape               = genelmbs::Boxkind::full;
    _SkeletonShaftsInterDist          = 0.0;
    _SkeletonShaftWidth               = 0.0;
    _SkeletonShaftHeight              = 0.0;
    _SkeletonShaftThicknesExt         = 0.0;
    _SkeletonShaftThicknesCent        = 0.0;
  }
  _nSkeletonRibs                      = config.getInt("floor.nSkeletonRibs",0);
  if (_nSkeletonRibs>0) {
    _SkeletonRibShape                 = static_cast<genelmbs::Boxkind::BxShpType> ( config.getInt("floor.SkeletonRibShape",0) );
    _SkeletonRibsInterDist            = config.getDouble("floor.SkeletonRibsInterDist",0.0);
    if (_SkeletonRibsInterDist<=0.0) {
      config.getVectorDouble("floor.SkeletonRibsPos", _SkeletonRibsPos, _nSkeletonRibs);
    }
    _SkeletonRibWidth                 = config.getDouble("floor.SkeletonRibWidth");
    _SkeletonRibHeight                = config.getDouble("floor.SkeletonRibHeight");
    _SkeletonRibThicknesExt           = config.getDouble("floor.SkeletonRibThicknesExt");
    _SkeletonRibThicknesCent          = config.getDouble("floor.SkeletonRibThicknesCent");
    _SkeletonEqMaterial               = config.getString("floor.SkeletonEqMaterial");
  } else {
    _SkeletonRibShape                 = genelmbs::Boxkind::full;
    _SkeletonRibsInterDist            = 0.0;
    _SkeletonRibWidth                 = 0.0;
    _SkeletonRibHeight                = 0.0;
    _SkeletonRibThicknesExt           = 0.0;
    _SkeletonRibThicknesCent          = 0.0;
  }
  if (_FullPlaneEqMaterial.empty()) {
    _FullPlaneEqMaterial = "G4_AIR";
  }

//  char tmpVarName[50];
  if (_geomType==0) {

    if (_nSkeletonShafts>0) {
      double skeletonWitdh = 0.0;
      if (_SkeletonShaftsInterDist>0.0) {
        skeletonWitdh = _nSkeletonShafts*_SkeletonShaftsInterDist;
        _SkeletonShaftsPos.clear();
        for (int isft=0; isft<_nSkeletonShafts; ++isft) {
          _SkeletonShaftsPos.push_back((isft+0.5)*_SkeletonShaftsInterDist);
        }
      } else {
        skeletonWitdh = _SkeletonShaftsPos.back()-_SkeletonShaftsPos[0]+_SkeletonShaftWidth;
      }
      if (_PlaneWidth<skeletonWitdh) {
        if (_PlaneWidth>0.0) {
          exc::exceptionG4 e("G4","Warning",4);
          e<< "FLOOR: plane width must be greater or equal to the skeleton width, it has been reset to it.\n";
          e.error();
        }
        _PlaneWidth=skeletonWitdh;
      }
    }
    if (_nSkeletonRibs>0) {
      double skeletonLength = 0.0;
      if (_SkeletonRibsInterDist>0.0) {
        skeletonLength = _nSkeletonRibs*_SkeletonRibsInterDist;
        _SkeletonRibsPos.clear();
        for (int irb=0; irb<_nSkeletonRibs; ++irb) {
          _SkeletonRibsPos.push_back((irb+0.5)*_SkeletonRibsInterDist);
        }
      } else {
        skeletonLength = _SkeletonRibsPos.back()-_SkeletonRibsPos[0]+_SkeletonRibWidth;
      }
      if (_Length<skeletonLength) {
        exc::exceptionG4 e("G4","Warning",4);
        e<< "FLOOR: plane length must be greater or equal to the skeleton length (= (nSkeletonRibs+1)*SkeletonRibsInterDist), it has been reset to it.\n";
        e.error();
        _Length=skeletonLength;
      }
    }

    _halfLength = _Length/2.0;
    _halfWidth = _PlaneWidth/2.0;
    _halfHeight = (_PlaneThicknes+_SkeletonShaftHeight+_SkeletonRibHeight)/2.0;

    _center.setY(-_halfHeight);

    _nBxKindEl = 1;  //Floor Plane
    if (_nSkeletonShafts>0) {
      _nBxKindEl+=_nSkeletonShafts;
    }
    if (_nSkeletonRibs>0) {
      _nBxKindEl+=_nSkeletonRibs;
    }
  }

}

void FLOORMaker::Build(){

  _lflrd = std::unique_ptr<FLOORdescription>(new FLOORdescription());
  _lflrd->_isExternal = _isExternal;
  _lflrd->_geomType=_geomType;

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"FLOOR: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {

    _lflrd->_x0 = _center.x();
    _lflrd->_y0 = _center.y();
    _lflrd->_z0 = _center.z();
    _lflrd->_zHalfLength = _halfLength;
    _lflrd->_halfWidth = _halfWidth;
    _lflrd->_halfHeight = _halfHeight;

//    _lpst->_roChhnd.reset( new FLOORROGeometryHandle(_lpst.get()) );

    _lflrd->_nFlrBx = _nBxKindEl;
    genelmbs::Boxkind *_flrBx = new genelmbs::Boxkind[_nBxKindEl];

    //------------------ structural elements description -----------------------------
    long lbid=0;


    genelmbs::Boxkind &flrPln = _flrBx[lbid];
    flrPln._elmName="floorPlane";
    flrPln._id = genelmbs::BoxkindId(lbid);
    flrPln._shapeType = genelmbs::Boxkind::full;
    flrPln._detail.reset( new genelmbs::BoxkindDetail(_Length/2.0, _PlaneWidth/2.0, _PlaneThicknes/2.0, _FullPlaneEqMaterial) );
    HepGeom::Translate3D plnpos (0.0,
        _halfHeight-flrPln._detail->halfHeight(),
        0.0);
    flrPln.setPosMatrix ( plnpos );
    ++lbid;

    for (int isft=0; isft<_nSkeletonShafts; ++isft) {

      genelmbs::Boxkind &flrSft = _flrBx[lbid];
      flrSft._elmName="floorSft";
      flrSft._id = genelmbs::BoxkindId(isft);
      flrSft._shapeType = _SkeletonShaftShape;
      flrSft._detail.reset( new genelmbs::BoxkindDetail(_Length/2.0, _SkeletonShaftWidth/2.0, _SkeletonShaftHeight/2.0, _SkeletonEqMaterial,
                                                          _SkeletonShaftThicknesExt, _SkeletonShaftThicknesCent) );
      HepGeom::Translate3D sftpos (-_halfWidth+_SkeletonShaftsPos[isft],
          _halfHeight-_PlaneThicknes-flrSft._detail->halfHeight(),
          0.0);
      flrSft.setPosMatrix ( sftpos );
      ++lbid;
    }

    for (int irb=0; irb<_nSkeletonRibs; ++irb) {
      genelmbs::Boxkind &flrRbs = _flrBx[lbid];
      flrRbs._elmName="floorRbs";
      flrRbs._id = genelmbs::BoxkindId(irb);
      flrRbs._shapeType = _SkeletonRibShape;
      flrRbs._detail.reset( new genelmbs::BoxkindDetail(_PlaneWidth/2.0, _SkeletonRibWidth/2.0, _SkeletonRibHeight/2.0, _SkeletonEqMaterial,
                                                        _SkeletonRibThicknesExt, _SkeletonRibThicknesCent) );
      HepGeom::RotateY3D ribR(CLHEP::halfpi);
      HepGeom::Translate3D ribpos (0.0,
          _halfHeight-_PlaneThicknes-_SkeletonShaftHeight-flrRbs._detail->halfHeight(),
          -_halfLength+_SkeletonRibsPos[irb]);
      HepGeom::Transform3D ribTransf (ribpos*ribR);
      flrRbs.setPosMatrix ( ribTransf );
      ++lbid;
    }

    _lflrd->_flrBx.reset(_flrBx);

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

} // namespace flr

//#endif
