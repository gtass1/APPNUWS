// TRUCK geometry maker
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

#include "TRUCKMaker.hh"

#include "GENELMbsBoxkindId.hh"
#include "GENELMbsBoxkindDetail.hh"
#include "GENELMbsCylkindId.hh"
#include "GENELMbsCylkindDetail.hh"

using CLHEP::Hep3Vector;
using CLHEP::HepRotationY;
using CLHEP::HepRotationZ;

namespace trck {

TRUCKMaker::TRUCKMaker( crd::SimpleConfig const& config):
                                                    _center(){

        _isExternal     = false;
        _extFile        = config.getString("trck.extFile");
        if ( _extFile.size()>1 && ( _extFile.find_last_of(".gdml") || _extFile.find_last_of(".GDML") )!=0 ) _isExternal = true;

        _center.setZ( config.getDouble("trck.z0",0.0) );
        _center.setX( config.getDouble("trck.x0",0.0) );
        _geomType       = config.getInt("trck.geomType");

        _halfLength=0.0;
        _halfWidth=0.0;
        _halfHeight=0.0;

        loadTruck(config);

//        loadAbsorber(config);

        std::cout<<"------------------- TRUCK-------------------"<<std::endl;
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
//        BuildAbsorber();
}

TRUCKMaker::~TRUCKMaker (){}

void TRUCKMaker::loadTruck( crd::SimpleConfig const& config ){

  _TrailerPlaneLength               = config.getDouble("trck.TrailerPlaneLength",0.0);
  _TrailerPlaneWidth                = config.getDouble("trck.TrailerPlaneWidth",0.0);
  _TrailerPlaneThicknes             = config.getDouble("trck.TrailerPlaneThicknes",0.0);
  _TrailerFullPlaneEqMaterial       = config.getString("trck.TrailerFullPlaneEqMaterial","");
  _TrailerPlaneHeight               = config.getDouble("trck.TrailerPlaneHeight",0.0);
  _nTrailerRibs                     = config.getInt("trck.nTrailerRibs",0);
  config.getVectorDouble("trck.TrailerRibPos", _TrailerRibPos, _nTrailerRibs);
  if (_nTrailerRibs>0) {
    _TrailerRibsWidth                 = config.getDouble("trck.TrailerRibsWidth");
    _TrailerRibsHeight                = config.getDouble("trck.TrailerRibsHeight");
    _TrailerRibsThicknesExt           = config.getDouble("trck.TrailerRibsThicknesExt");
    _TrailerRibsThicknesCent          = config.getDouble("trck.TrailerRibsThicknesCent");
    _TrailerRibsEqMaterial            = config.getString("trck.TrailerRibsEqMaterial");
  } else {
    _TrailerRibsWidth                 = 0.0;
    _TrailerRibsHeight                = 0.0;
    _TrailerRibsThicknesExt           = 0.0;
    _TrailerRibsThicknesCent          = 0.0;
    _TrailerRibsEqMaterial            = "";
  }
  if (_TrailerFullPlaneEqMaterial.empty()) {
    _TrailerFullPlaneEqMaterial = "G4_AIR";
  }

  //Trailer Chassis description
  _TrailerShaftsInterDist           = config.getDouble("trck.TrailerShaftsInterDist",0.0);
  _TrailerShaftsFullHeightLength    = config.getDouble("trck.TrailerShaftsFullHeightLength",0.0);
  _TrailerShaftsWidth               = config.getDouble("trck.TrailerShaftsWidth",0.0);
  _TrailerShaftsHeight              = config.getDouble("trck.TrailerShaftsHeight",0.0);
  _TrailerShaftsThicknesExt         = config.getDouble("trck.TrailerShaftsThicknesExt",0.0);
  _TrailerShaftsThicknesCent        = config.getDouble("trck.TrailerShaftsThicknesCent",0.0);
  _TrailerShaftsEqMaterial          = config.getString("trck.TrailerShaftsEqMaterial");
  _TrailerReducedShaftsHeight       = config.getDouble("trck.TrailerReducedShaftsHeight",0.0);

  //Trailer wheels description
  _nTrailerWheels                   = config.getInt("trck.nTrailerWheels",0);
  _TrlWheelRimRadius                = config.getDouble("trck.TrlWheelRimRadius",0.0);
  _TrlWheelRimWidth                 = config.getDouble("trck.TrlWheelRimWidth",0.0);
  _TrlWheelRimShellThickness        = config.getDouble("trck.TrlWheelRimShellThickness",0.0);
  _TrlWheelRimEqThickness           = config.getDouble("trck.TrlWheelRimEqThickness",0.0);
  _TrlTireHeight                    = config.getDouble("trck.TrlTireHeight",0.0);
  _TrlWheelPitch                    = config.getDouble("trck.TrlWheelPitch",0.0);
  _TrlWheelDistFromBack             = config.getDouble("trck.TrlWheelDistFromBack",0.0);
  _TrlWheelAxisRadius               = config.getDouble("trck.TrlWheelAxisRadius",0.0);
  _TrlWheelRimMaterial              = config.getString("trck.TrlWheelRimMaterial");
  _TrlWheelAxisMaterial             = config.getString("trck.TrlWheelAxisMaterial");
  _TrlTireMaterial                  = config.getString("trck.TrlTireMaterial");
  _TrlTireEqThickness               = config.getDouble("trck.TrlTireEqThickness",0.0);


  //Container description
  _ContainerLength                  = config.getDouble("trck.ContainerLength",0.0);
  _ContainerWidth                   = config.getDouble("trck.ContainerWidth",0.0);
  _ContainerHeight                  = config.getDouble("trck.ContainerHeight",0.0);
  _ContainerWallThicknes            = config.getDouble("trck.ContainerWallThicknes");
  _ContainerRibsThicknes            = config.getDouble("trck.ContainerRibsThicknes",0.0);
  _ContainerBasementThicknes        = config.getDouble("trck.ContainerBasementThicknes");
  _ContainerWallEqMaterial          = config.getString("trck.ContainerWallEqMaterial");
  _ContainerRibsEqMaterial          = config.getString("trck.ContainerRibsEqMaterial");
  _ContainerBasementEqMaterial      = config.getString("trck.ContainerBasementEqMaterial");

  if (_TrailerRibsHeight>_TrailerPlaneHeight) {
    exc::exceptionG4 e("G4","Warning",4);
    e<< "TRUCK: Trailer Ribs height/thickness must be less or equal to TrailerPlaneThicknes, it has been reset to TrailerPlaneThicknes.\n";
    e.error();

    _TrailerRibsHeight=_TrailerPlaneThicknes;
  }
  if (_ContainerLength>_TrailerPlaneLength) {
    exc::exceptionG4 e("G4","Warning",4);
    e<< "TRUCK: ContainerLength must be less or equal to TrailerPlaneLength, it has been reset to TrailerPlaneLength.\n";
    e.error();

    _ContainerLength=_TrailerPlaneLength;
  }
  if (_ContainerWidth>_TrailerPlaneWidth) {
    exc::exceptionG4 e("G4","Warning",4);
    e<< "TRUCK: ContainerWidth must be less or equal to TrailerPlaneWidth, it has been reset to TrailerPlaneWidth.\n";
    e.error();

    _ContainerWidth=_TrailerPlaneWidth;
  }
  if (_ContainerRibsThicknes>0.0 && _ContainerRibsThicknes<_ContainerWallThicknes) {
    exc::exceptionG4 e("G4","Warning",4);
    e<< "TRUCK: Container ribs thickness must be >= Container wall thickness, it has been reset to ContainerWallThicknes\n";
    e.error();
    _ContainerRibsThicknes=_ContainerWallThicknes;
  }

  //Truck Chassis description
  _TruckShaftsInterDist             = config.getDouble("trck.TruckShaftsInterDist",0.0);
  _TruckShaftsLength                = config.getDouble("trck.TruckShaftsLength",0.0);
  _TruckShaftsWidth                 = config.getDouble("trck.TruckShaftsWidth",0.0);
  _TruckShaftsHeight                = config.getDouble("trck.TruckShaftsHeight",0.0);
  _TruckShaftsThicknesExt           = config.getDouble("trck.TruckShaftsThicknesExt",0.0);
  _TruckShaftsThicknesCent          = config.getDouble("trck.TruckShaftsThicknesCent",0.0);
  _TruckChassistsHeight             = config.getDouble("trck.TruckChassistsHeight",0.0);
  _TruckChassisEqMaterial           = config.getString("trck.TruckChassisEqMaterial");
  _nTruckRibs                       = config.getInt("trck.nTruckRibs",0);
  config.getVectorDouble("trck.TruckRibPos", _TruckRibPos, _nTruckRibs);
  _TruckTrailerOverlap              = config.getDouble("trck.TruckTrailerOverlap",0.0);


  //Truck wheels description
  _TruckWheelRimRadius                = config.getDouble("trck.TruckWheelRimRadius",0.0);
  _TruckWheelRimWidth                 = config.getDouble("trck.TruckWheelRimWidth",0.0);
  _TruckWheelRimShellThickness        = config.getDouble("trck.TruckWheelRimShellThickness",0.0);
  _TruckWheelRimEqThickness           = config.getDouble("trck.TruckWheelRimEqThickness",0.0);
  _TruckTireHeight                    = config.getDouble("trck.TruckTireHeight",0.0);
  _TruckWheelDistFromBack             = config.getDouble("trck.TruckWheelDistFromBack",0.0);
  _TruckWheelDistFromFront            = config.getDouble("trck.TruckWheelDistFromFront",0.0);
  _TruckWheelAxisRadius               = config.getDouble("trck.TruckWheelAxisRadius",0.0);
  _TruckWheelRimMaterial              = config.getString("trck.TruckWheelRimMaterial");
  _TruckWheelAxisMaterial             = config.getString("trck.TruckWheelAxisMaterial");
  _TruckTireMaterial                  = config.getString("trck.TruckTireMaterial");
  _TruckTireEqThickness               = config.getDouble("trck.TruckTireEqThickness",0.0);

  _TrckTrnWheelJnRadius               = config.getDouble("trck.TrckTrnWheelJnRadius");
  _TrckTrnWheelJnThickness            = config.getDouble("trck.TrckTrnWheelJnThickness");
  _TrckTrnWheelJnDistFromBack         = config.getDouble("trck.TrckTrnWheelJnDistFromBack");
  _TrckTrnWheelJnEqMaterial           = config.getString("trck.TrckTrnWheelJnEqMaterial");


//  char tmpVarName[50];
  if (_geomType==0) {

    _halfLength = (_TrailerPlaneLength + _TruckShaftsLength - _TruckTrailerOverlap)/2.0;
    _halfWidth = _TrailerPlaneWidth/2.0;
    _halfHeight = (_TrailerPlaneHeight + _ContainerHeight)/2.0;

    _center.setY(_halfHeight);

    _ContainerInLength = _ContainerLength;
    _ContainerInWidth = _ContainerWidth;
    _ContainerInHeight = _ContainerHeight;

    _nTrlBxKindEl = 2;  //two shafts is assumed
    if (_TrailerReducedShaftsHeight>0.0) {
      _nTrlBxKindEl += 2;
    }
    _nTrlBxKindEl += 1 + _nTrailerRibs; //1 is for the plane, is it is not a solid plane a volume to contain the ribs is even defined
    _nTrlCylKindEl = 5 * _nTrailerWheels; // 2 per wheel, 2 per wheel tire, 1 per axes
    ++_nTrlBxKindEl;  //container basement
    _nTrlBxKindEl += 5; //container walls, one per each face except for the basement
    _ContainerInHeight -= _ContainerBasementThicknes;
    if (_ContainerRibsThicknes>0.0) {
      _nTrlBxKindEl += 8;  //one for each box edge apart from the basement
      _ContainerInHeight -= _ContainerRibsThicknes;
      _ContainerInLength -= 2.0*_ContainerRibsThicknes;
      _ContainerInWidth -= 2.0*_ContainerRibsThicknes;
    } else {
      _ContainerInHeight -= _ContainerWallThicknes;
      _ContainerInLength -= 2.0*_ContainerWallThicknes;
      _ContainerInWidth -= 2.0*_ContainerWallThicknes;
    }

    _ContainerInRelCenter.setX(0.0);
    _ContainerInRelCenter.setY(_TrailerPlaneHeight+_ContainerBasementThicknes+_ContainerInHeight/2.0);
    _ContainerInRelCenter.setZ(-_halfLength+_ContainerLength/2.0);

    _nTrkBxKindEl = 2 + _nTruckRibs;
    _nTrkCylKindEl = 4 * 3 + 2; // 2 per wheel (3 wheel), 2 per wheel tire, 2 axes
    ++_nTrkCylKindEl; //Truck Trailer joint (fifth wheel)

  }

}

void TRUCKMaker::Build(){

  _ltrkd = std::unique_ptr<TRUCKdescription>(new TRUCKdescription());
  _ltrkd->_isExternal = _isExternal;
  _ltrkd->_geomType=_geomType;

  if (_isExternal) {
    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"TRUCK: Using GDML file option is temporarily disabled\n";
    e.error();

  } else {

    _ltrkd->_x0 = _center.x();
    _ltrkd->_y0 = _center.y();
    _ltrkd->_z0 = _center.z();
    _ltrkd->_zHalfLength = _halfLength;
    _ltrkd->_halfWidth = _halfWidth;
    _ltrkd->_halfHeight = _halfHeight;

//    _lpst->_roChhnd.reset( new TRUCKROGeometryHandle(_lpst.get()) );

    _ltrkd->_nTrkBx = _nTrlBxKindEl+_nTrkBxKindEl;
    genelmbs::Boxkind *_trkBx = new genelmbs::Boxkind[_nTrlBxKindEl+_nTrkBxKindEl];
    _ltrkd->_nTrkCyl = _nTrlCylKindEl+_nTrkCylKindEl;
    genelmbs::Cylkind *_trkCyl = new genelmbs::Cylkind[_nTrlCylKindEl+_nTrkCylKindEl];

    genelmbs::Boxkind *cntInBx = new genelmbs::Boxkind();
    cntInBx->_elmName="cntIn";
    cntInBx->_id = genelmbs::BoxkindId(0);
    cntInBx->_shapeType = genelmbs::Boxkind::full;
    cntInBx->_detail.reset( new genelmbs::BoxkindDetail(_ContainerInLength/2.0,_ContainerInWidth/2.0,_ContainerInHeight/2.0,"G4_AIR") );
    HepGeom::Translate3D cntInpos (_ContainerInRelCenter.x(),
        _ContainerInRelCenter.y()-_halfHeight,
        _ContainerInRelCenter.z() );
    cntInBx->setPosMatrix ( cntInpos );

//    _nTrailerWheels = 1;
//    boost::shared_ptr<genelmbs::Boxkind> box;
//    boost::shared_ptr<genelmbs::Boxkind> cyl;

    //         -----------------------------

    double trlTireOutRadius=_TrlWheelRimRadius+_TrlTireHeight;
    double truckTireOutRadius=_TruckWheelRimRadius+_TruckTireHeight;

    //------------------ Trailer structural elements description -----------------------------
    long lbid=0;

    genelmbs::Boxkind &trlSft = _trkBx[lbid];
    trlSft._elmName="trlrSft";
    trlSft._id = genelmbs::BoxkindId(lbid);
    trlSft._shapeType = genelmbs::Boxkind::Ishape;
    trlSft._detail.reset( new genelmbs::BoxkindDetail(_TrailerShaftsFullHeightLength/2.0, _TrailerShaftsWidth/2.0, _TrailerShaftsHeight/2.0, _TrailerShaftsEqMaterial,
                                                      _TrailerShaftsThicknesExt, _TrailerShaftsThicknesCent) );
    HepGeom::Translate3D sftposL (-_TrailerShaftsInterDist/2.0,
        _TrailerPlaneHeight-_TrailerPlaneThicknes-trlSft._detail->halfHeight()-_halfHeight,
        -_halfLength+trlSft._detail->halfLength());
    trlSft.setPosMatrix ( sftposL );
    ++lbid;

    genelmbs::Boxkind &trlSft1 = _trkBx[lbid];
    trlSft1._elmName="trlrSft";
    trlSft1._id = genelmbs::BoxkindId(lbid);
    trlSft1._shapeType = genelmbs::Boxkind::Ishape;
    trlSft1._detail.reset( new genelmbs::BoxkindDetail(_TrailerShaftsFullHeightLength/2.0, _TrailerShaftsWidth/2.0, _TrailerShaftsHeight/2.0, _TrailerShaftsEqMaterial,
                                                       _TrailerShaftsThicknesExt, _TrailerShaftsThicknesCent) );
    HepGeom::Translate3D sftposR (_TrailerShaftsInterDist/2.0,
        _TrailerPlaneHeight-_TrailerPlaneThicknes-trlSft1._detail->halfHeight()-_halfHeight,
        -_halfLength+trlSft1._detail->halfLength());
    trlSft1.setPosMatrix ( sftposR );
    ++lbid;

    if (_TrailerReducedShaftsHeight>0.0) {
      double trlrReducedShaftsLength = _TrailerPlaneLength-_TrailerShaftsFullHeightLength;
      genelmbs::Boxkind &trlSftRd = _trkBx[lbid];
      trlSftRd._elmName="trlrSft";
      trlSftRd._id = genelmbs::BoxkindId(lbid);
      trlSftRd._shapeType = genelmbs::Boxkind::Ishape;
      trlSftRd._detail.reset( new genelmbs::BoxkindDetail(trlrReducedShaftsLength/2.0, _TrailerShaftsWidth/2.0, _TrailerReducedShaftsHeight/2.0, _TrailerShaftsEqMaterial,
                                                          _TrailerShaftsThicknesExt, _TrailerShaftsThicknesCent) );
      HepGeom::Translate3D rdsftposL (-_TrailerShaftsInterDist/2.0,
          _TrailerPlaneHeight-_TrailerPlaneThicknes-trlSftRd._detail->halfHeight()-_halfHeight,
          -_halfLength+_TrailerShaftsFullHeightLength+trlSftRd._detail->halfLength());
      trlSftRd.setPosMatrix ( rdsftposL );
      ++lbid;

      genelmbs::Boxkind &trlSftRd1 = _trkBx[lbid];
      trlSftRd1._elmName="trlrSft";
      trlSftRd1._id = genelmbs::BoxkindId(lbid);
      trlSftRd1._shapeType = genelmbs::Boxkind::Ishape;
      trlSftRd1._detail.reset( new genelmbs::BoxkindDetail(trlrReducedShaftsLength/2.0, _TrailerShaftsWidth/2.0, _TrailerReducedShaftsHeight/2.0, _TrailerShaftsEqMaterial,
                                                           _TrailerShaftsThicknesExt, _TrailerShaftsThicknesCent) );
      HepGeom::Translate3D rdsftposR (_TrailerShaftsInterDist/2.0,
          _TrailerPlaneHeight-_TrailerPlaneThicknes-trlSftRd1._detail->halfHeight()-_halfHeight,
          -_halfLength+_TrailerShaftsFullHeightLength+trlSftRd1._detail->halfLength());
      trlSftRd1.setPosMatrix ( rdsftposR );
      ++lbid;
    }

    genelmbs::Boxkind &trlPln = _trkBx[lbid];
    trlPln._elmName="trlrPlane";
    trlPln._id = genelmbs::BoxkindId(lbid);
    trlPln._shapeType = genelmbs::Boxkind::full;
    trlPln._detail.reset( new genelmbs::BoxkindDetail(_TrailerPlaneLength/2.0, _TrailerPlaneWidth/2.0, _TrailerPlaneThicknes/2.0, _TrailerFullPlaneEqMaterial) );
    HepGeom::Translate3D plnpos (0.0,
        _TrailerPlaneHeight-trlPln._detail->halfHeight()-_halfHeight,
        -_halfLength+trlPln._detail->halfLength());
    trlPln.setPosMatrix ( plnpos );
    ++lbid;

    for (int itrrb=0; itrrb<_nTrailerRibs; ++itrrb) {
      genelmbs::Boxkind &trlRbs = _trkBx[lbid];
      trlRbs._elmName="trlrRbs";
      trlRbs._id = genelmbs::BoxkindId(lbid);
      trlRbs._shapeType = genelmbs::Boxkind::Ishape;
      trlRbs._detail.reset( new genelmbs::BoxkindDetail(_TrailerPlaneWidth/2.0, _TrailerRibsWidth/2.0, _TrailerRibsHeight/2.0, _TrailerRibsEqMaterial,
                                                        _TrailerRibsThicknesExt, _TrailerRibsThicknesCent) );
      HepGeom::RotateY3D ribR(CLHEP::halfpi);
      HepGeom::Translate3D ribpos (0.0,
          0.0,//_TrailerPlaneHeight+_TrailerPlaneHeight/2.0-_halfHeight,
          -_TrailerPlaneLength/2.0+trlRbs._detail->halfWidth()+_TrailerRibPos[itrrb]);
      HepGeom::Transform3D ribTransf (ribpos*ribR);
      trlRbs.setPosMatrix ( ribTransf );
      ++lbid;
    }

    //------------------ Container description -----------------------------

    genelmbs::Boxkind &cntPln = _trkBx[lbid];
    cntPln._elmName="cntBasePlane";
    cntPln._id = genelmbs::BoxkindId(lbid);
    cntPln._shapeType = genelmbs::Boxkind::full;
    cntPln._detail.reset( new genelmbs::BoxkindDetail(_ContainerLength/2.0, _ContainerWidth/2.0, _ContainerBasementThicknes/2.0, _ContainerBasementEqMaterial) );
    HepGeom::Translate3D cntbspos (_ContainerInRelCenter.x(),
        _ContainerInRelCenter.y()-_ContainerInHeight/2.0-cntPln._detail->halfHeight()-_halfHeight,
        _ContainerInRelCenter.z());
    cntPln.setPosMatrix ( cntbspos );
    ++lbid;

    double contLatWallLength=_ContainerInLength;
    double contLatWallWidth=_ContainerInHeight;
    if (_ContainerRibsThicknes<=0.0) {
      contLatWallLength+=2.0*_ContainerWallThicknes;
//      contLatWallWidth+=2.0*_ContainerWallThicknes;
    }
    for (int side=1; side>-2; side-=2) {
      genelmbs::Boxkind &cntWallLat = _trkBx[lbid];
      cntWallLat._elmName="cntrWall";//"cntrWallLat";
      cntWallLat._id = genelmbs::BoxkindId(lbid);
      cntWallLat._shapeType = genelmbs::Boxkind::full;
      cntWallLat._detail.reset( new genelmbs::BoxkindDetail(contLatWallLength/2.0, contLatWallWidth/2.0, _ContainerWallThicknes/2.0, _ContainerWallEqMaterial) );
      HepGeom::RotateZ3D cntWallLatRVert(CLHEP::halfpi);
      HepGeom::Translate3D cntWallLatpos (side*(-_ContainerInWidth-_ContainerWallThicknes)/2.0 + _ContainerInRelCenter.x(),
          _ContainerInRelCenter.y()-_halfHeight,
          _ContainerInRelCenter.z());
      HepGeom::Transform3D cntWallLatTransf (cntWallLatpos*cntWallLatRVert);
      cntWallLat.setPosMatrix ( cntWallLatTransf );
      ++lbid;
//    }
//
//    for (int side=1; side>-2; side-=2) {
      genelmbs::Boxkind &cntWallTrns = _trkBx[lbid];
      cntWallTrns._elmName="cntrWall";//"cntrWallTransv";
      cntWallTrns._id = genelmbs::BoxkindId(lbid);
      cntWallTrns._shapeType = genelmbs::Boxkind::full;
      cntWallTrns._detail.reset( new genelmbs::BoxkindDetail(_ContainerInHeight/2.0, _ContainerInWidth/2.0, _ContainerWallThicknes/2.0, _ContainerWallEqMaterial) );
      HepGeom::RotateX3D cntWallTrnsRVert(CLHEP::halfpi);
      HepGeom::Translate3D cntWallTrnspos (_ContainerInRelCenter.x(),
          _ContainerInRelCenter.y()-_halfHeight,
          _ContainerInRelCenter.z()+side*(_ContainerInLength+_ContainerWallThicknes)/2.0);
      HepGeom::Transform3D cntWallTrnsTransf (cntWallTrnspos*cntWallTrnsRVert);
      cntWallTrns.setPosMatrix ( cntWallTrnsTransf );
      ++lbid;
    }

    genelmbs::Boxkind &cntWallTop = _trkBx[lbid];
    cntWallTop._elmName="cntrWall";//"cntrWallTop";
    cntWallTop._id = genelmbs::BoxkindId(lbid);
    cntWallTop._shapeType = genelmbs::Boxkind::full;
    cntWallTop._detail.reset( new genelmbs::BoxkindDetail(contLatWallLength/2.0, _ContainerInWidth/2.0, _ContainerWallThicknes/2.0, _ContainerWallEqMaterial) );
    HepGeom::Translate3D cntWallToppos (_ContainerInRelCenter.x(),
        _ContainerInRelCenter.y()+_ContainerInHeight/2.0+cntWallTop._detail->halfHeight()-_halfHeight,
        _ContainerInRelCenter.z());
    cntWallTop.setPosMatrix ( cntWallToppos );
    ++lbid;

    if (_ContainerRibsThicknes>0.0) {

      for (int side=1; side>-2; side-=2) {
        genelmbs::Boxkind &cntRbsLat = _trkBx[lbid];
        cntRbsLat._elmName="cntrRbs";
        cntRbsLat._id = genelmbs::BoxkindId(lbid);
        cntRbsLat._shapeType = genelmbs::Boxkind::full;
        cntRbsLat._detail.reset( new genelmbs::BoxkindDetail(_ContainerLength/2.0, _ContainerRibsThicknes/2.0, _ContainerRibsThicknes/2.0, _ContainerRibsEqMaterial) );
        //        HepGeom::RotateY3D ribRVert(CLHEP::halfpi);
        HepGeom::Translate3D cntRibLatpos (side*(-_ContainerInWidth-_ContainerRibsThicknes)/2.0 + _ContainerInRelCenter.x(),
            _ContainerInRelCenter.y()+(_ContainerInHeight+_ContainerRibsThicknes)/2.0-_halfHeight,
            _ContainerInRelCenter.z());
        //        HepGeom::Transform3D ribTransf (ribpos*ribRVert);
        cntRbsLat.setPosMatrix ( cntRibLatpos );
        ++lbid;

        genelmbs::Boxkind &cntRbsTrns = _trkBx[lbid];
        cntRbsTrns._elmName="cntrRbs";
        cntRbsTrns._id = genelmbs::BoxkindId(lbid);
        cntRbsTrns._shapeType = genelmbs::Boxkind::full;
        cntRbsTrns._detail.reset( new genelmbs::BoxkindDetail(_ContainerRibsThicknes/2.0, _ContainerInWidth/2.0, _ContainerRibsThicknes/2.0, _ContainerRibsEqMaterial) );
        //        HepGeom::RotateY3D ribRVert(CLHEP::halfpi);
        HepGeom::Translate3D cntRibTrnspos (_ContainerInRelCenter.x(),
            _ContainerInRelCenter.y()+(_ContainerInHeight+_ContainerRibsThicknes)/2.0-_halfHeight,
            _ContainerInRelCenter.z() + side*(_ContainerInLength+_ContainerRibsThicknes)/2.0 );
        //        HepGeom::Transform3D ribTransf (ribpos*ribRVert);
        cntRbsTrns.setPosMatrix ( cntRibTrnspos );
        ++lbid;

        for (int side2=1; side2>-2; side2-=2) {
          genelmbs::Boxkind &cntRbsVrt = _trkBx[lbid];
          cntRbsVrt._elmName="cntrRbs";
          cntRbsVrt._id = genelmbs::BoxkindId(lbid);
          cntRbsVrt._shapeType = genelmbs::Boxkind::full;
          cntRbsVrt._detail.reset( new genelmbs::BoxkindDetail(_ContainerInHeight/2.0, _ContainerRibsThicknes/2.0, _ContainerRibsThicknes/2.0, _ContainerRibsEqMaterial) );
          HepGeom::RotateX3D cntRibVrtRVert(CLHEP::halfpi);
          HepGeom::Translate3D cntRibVrtpos (side*(-_ContainerInWidth-_ContainerRibsThicknes)/2.0 + _ContainerInRelCenter.x(),
              _ContainerInRelCenter.y()-_halfHeight,
              _ContainerInRelCenter.z() + side2*(_ContainerInLength+_ContainerRibsThicknes)/2.0 );
          HepGeom::Transform3D cntRibVrtTransf (cntRibVrtpos*cntRibVrtRVert);
          cntRbsVrt.setPosMatrix ( cntRibVrtTransf );
          ++lbid;

        }
      }

    }

    //------------------ Truck structural elements description -----------------------------

    genelmbs::Boxkind &trkSft = _trkBx[lbid];
     trkSft._elmName="trkrSft";
     trkSft._id = genelmbs::BoxkindId(lbid);
     trkSft._shapeType = genelmbs::Boxkind::Cshape;
     trkSft._detail.reset( new genelmbs::BoxkindDetail(_TruckShaftsLength/2.0, _TruckShaftsWidth/2.0, _TruckShaftsHeight/2.0, _TruckChassisEqMaterial,
                                                       _TruckShaftsThicknesExt, _TruckShaftsThicknesCent) );
     HepGeom::Translate3D trksftposL (-_TruckShaftsInterDist/2.0,
         _TruckChassistsHeight-trkSft._detail->halfHeight()-_halfHeight,
         _halfLength-trkSft._detail->halfLength());
     trkSft.setPosMatrix ( trksftposL );
     ++lbid;

     genelmbs::Boxkind &trkSft1 = _trkBx[lbid];
     trkSft1._elmName="trkrSft";
     trkSft1._id = genelmbs::BoxkindId(lbid);
     trkSft1._shapeType = genelmbs::Boxkind::Cshape;
     trkSft1._detail.reset( new genelmbs::BoxkindDetail(_TruckShaftsLength/2.0, _TruckShaftsWidth/2.0, _TruckShaftsHeight/2.0, _TruckChassisEqMaterial,
                                                        _TruckShaftsThicknesExt, _TruckShaftsThicknesCent) );
     HepGeom::RotateZ3D trksftRVertR(CLHEP::pi);
     HepGeom::Translate3D trksftposR (_TruckShaftsInterDist/2.0,
         _TruckChassistsHeight-trkSft._detail->halfHeight()-_halfHeight,
         _halfLength-trkSft._detail->halfLength());
     HepGeom::Transform3D trkSftTransfR (trksftposR*trksftRVertR);
     trkSft1.setPosMatrix ( trkSftTransfR );
     ++lbid;

     double trckRibLength=_TruckShaftsInterDist+_TruckShaftsWidth-2.0*_TruckShaftsThicknesCent;
     for (int itrkrb=0; itrkrb<_nTruckRibs; ++itrkrb) {
       genelmbs::Boxkind &trkRbs = _trkBx[lbid];
       trkRbs._elmName="trkrRbs";
       trkRbs._id = genelmbs::BoxkindId(lbid);
       trkRbs._shapeType = genelmbs::Boxkind::Cshape;
       trkRbs._detail.reset( new genelmbs::BoxkindDetail(trckRibLength/2.0, _TruckShaftsWidth/2.0, (_TruckShaftsHeight-2.0*_TruckShaftsThicknesExt)/2.0, _TruckChassisEqMaterial,
                                                          _TruckShaftsThicknesExt, _TruckShaftsThicknesCent) );
       HepGeom::RotateY3D trkribR(-CLHEP::halfpi);
       HepGeom::Translate3D trkribpos (0.0,
           _TruckChassistsHeight-trkSft._detail->halfHeight()-_halfHeight,
           _halfLength-2.0*trkSft._detail->halfLength()+trkRbs._detail->halfWidth()+_TruckRibPos[itrkrb]);
       HepGeom::Transform3D trkribTransf (trkribpos*trkribR);
       trkRbs.setPosMatrix ( trkribTransf );
       ++lbid;
     }

    //------------------ Trailer wheels description -----------------------------
    long lcyid=0;
    if (_nTrailerWheels>0) {
      for (int itrw=0; itrw<_nTrailerWheels; ++itrw) {

        for (int side=1; side>-2; side-=2) {
          genelmbs::Cylkind &tCyl = _trkCyl[lcyid];
          tCyl._elmName="trlrWheel";
          tCyl._id = genelmbs::CylkindId(lcyid);
          tCyl._shapeType = genelmbs::Cylkind::Cshape;
          tCyl._detail.reset( new genelmbs::CylkindDetail(_TrlWheelRimWidth/2.0, 0.0, _TrlWheelRimRadius, _TrlWheelRimMaterial,
                                                          _TrlWheelRimShellThickness, _TrlWheelRimEqThickness) ); // 10.0 means 1 cm of supposed rim thickness for the tire support
          HepGeom::RotateY3D cylRVertL(side*CLHEP::halfpi);
          HepGeom::Translate3D cylposL (side*(-_halfWidth+_TrlWheelRimWidth/2.0),
              trlTireOutRadius-_halfHeight,
              -_halfLength+_TrlWheelDistFromBack+_TrlWheelPitch*itrw);
          HepGeom::Transform3D cylTransfL (cylposL*cylRVertL);
          //        HepGeom::Transform3D cylTransfL (cylposL);
          tCyl.setPosMatrix ( cylTransfL );
          ++lcyid;

          genelmbs::Cylkind &tCyl1 = _trkCyl[lcyid];
          tCyl1._elmName="trlrTire";
          tCyl1._id = genelmbs::CylkindId(lcyid);
          tCyl1._shapeType = genelmbs::Cylkind::Ushape;
          tCyl1._detail.reset( new genelmbs::CylkindDetail(_TrlWheelRimWidth/2.0, _TrlWheelRimRadius, trlTireOutRadius, _TrlTireMaterial,
                                                           _TrlTireEqThickness, 5.0) ); //Tire vertical wall thickness is supposed to be equivalent 5.0 mmm
          tCyl1.setPosMatrix ( cylTransfL );
          ++lcyid;
        }

        genelmbs::Cylkind &tCyl2 = _trkCyl[lcyid];
        tCyl2._elmName="trlrAxis";
        tCyl2._id = genelmbs::CylkindId(lcyid);
        tCyl2._shapeType = genelmbs::Cylkind::full;
        tCyl2._detail.reset( new genelmbs::CylkindDetail(_halfWidth-_TrlWheelRimWidth, 0.0, _TrlWheelAxisRadius, _TrlWheelAxisMaterial) );
        HepGeom::RotateY3D cylRVert(CLHEP::halfpi);
        HepGeom::Translate3D cylpos (0.0,
            trlTireOutRadius-_halfHeight,
            -_halfLength+_TrlWheelDistFromBack+_TrlWheelPitch*itrw);
        HepGeom::Transform3D cylTransf (cylpos*cylRVert);
        //        HepGeom::Transform3D cylTransfR (cylposR);
        tCyl2.setPosMatrix ( cylTransf );
        ++lcyid;

      }
      //------------------ Truck wheels description -----------------------------

      for (int side=1; side>-2; side-=2) {
        genelmbs::Cylkind &trkWl = _trkCyl[lcyid];
        trkWl._elmName="trckWheel";
        trkWl._id = genelmbs::CylkindId(lcyid);
        trkWl._shapeType = genelmbs::Cylkind::Cshape;
        trkWl._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, 0.0, _TruckWheelRimRadius, _TruckWheelRimMaterial,
                                                        _TruckWheelRimShellThickness, _TruckWheelRimEqThickness) ); // 10.0 means 1 cm of supposed rim thickness for the tire support
        HepGeom::RotateY3D cylRVert(side*CLHEP::halfpi);
        HepGeom::Translate3D cylpos (side*(-_halfWidth+_TruckWheelRimWidth/2.0),
            truckTireOutRadius-_halfHeight,
            _halfLength-_TruckShaftsLength+_TruckWheelDistFromBack+truckTireOutRadius);
        HepGeom::Transform3D cylTransf (cylpos*cylRVert);
        //        HepGeom::Transform3D cylTransfL (cylposL);
        trkWl.setPosMatrix ( cylTransf );
        ++lcyid;

        genelmbs::Cylkind &trkTr = _trkCyl[lcyid];
        trkTr._elmName="trckTire";
        trkTr._id = genelmbs::CylkindId(lcyid);
        trkTr._shapeType = genelmbs::Cylkind::Ushape;
        trkTr._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, _TruckWheelRimRadius, truckTireOutRadius, _TruckTireMaterial,
                                                         _TruckTireEqThickness, 5.0) ); //Tire vertical wall thickness is supposed to be equivalent 5.0 mmm
        trkTr.setPosMatrix ( cylTransf );
        ++lcyid;

        genelmbs::Cylkind &trkWl1 = _trkCyl[lcyid];
        trkWl1._elmName="trckWheel";
        trkWl1._id = genelmbs::CylkindId(lcyid);
        trkWl1._shapeType = genelmbs::Cylkind::Cshape;
        trkWl1._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, _TruckWheelAxisRadius, _TruckWheelRimRadius, _TruckWheelRimMaterial,
                                                        _TruckWheelRimShellThickness, _TruckWheelRimEqThickness) ); // 10.0 means 1 cm of supposed rim thickness for the tire support
        HepGeom::Translate3D cylpos1 (side*(-_halfWidth+_TruckWheelRimWidth+50.0+_TruckWheelRimWidth/2.0),
            truckTireOutRadius-_halfHeight,
            _halfLength-_TruckShaftsLength+_TruckWheelDistFromBack+truckTireOutRadius);
        HepGeom::Transform3D cylTransf1 (cylpos1*cylRVert);
        //        HepGeom::Transform3D cylTransfL (cylposL);
        trkWl1.setPosMatrix ( cylTransf1 );
        ++lcyid;

        genelmbs::Cylkind &trkTr1 = _trkCyl[lcyid];
        trkTr1._elmName="trckTire";
        trkTr1._id = genelmbs::CylkindId(lcyid);
        trkTr1._shapeType = genelmbs::Cylkind::Ushape;
        trkTr1._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, _TruckWheelRimRadius, truckTireOutRadius, _TruckTireMaterial,
                                                         _TruckTireEqThickness, 5.0) ); //Tire vertical wall thickness is supposed to be equivalent 5.0 mmm
        trkTr1.setPosMatrix ( cylTransf1 );
        ++lcyid;

        genelmbs::Cylkind &trkWlF = _trkCyl[lcyid];
        trkWlF._elmName="trckWheel";
        trkWlF._id = genelmbs::CylkindId(lcyid);
        trkWlF._shapeType = genelmbs::Cylkind::Cshape;
        trkWlF._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, 0.0, _TruckWheelRimRadius, _TruckWheelRimMaterial,
                                                        _TruckWheelRimShellThickness, _TruckWheelRimEqThickness) ); // 10.0 means 1 cm of supposed rim thickness for the tire support
        HepGeom::Translate3D cylposF (side*(-_halfWidth+_TruckWheelRimWidth/2.0),
            truckTireOutRadius-_halfHeight,
            _halfLength-_TruckWheelDistFromFront-truckTireOutRadius);
        HepGeom::Transform3D cylTransfF (cylposF*cylRVert);
        //        HepGeom::Transform3D cylTransfL (cylposL);
        trkWlF.setPosMatrix ( cylTransfF );
        ++lcyid;

        genelmbs::Cylkind &trkTrF = _trkCyl[lcyid];
        trkTrF._elmName="trckTire";
        trkTrF._id = genelmbs::CylkindId(lcyid);
        trkTrF._shapeType = genelmbs::Cylkind::Ushape;
        trkTrF._detail.reset( new genelmbs::CylkindDetail(_TruckWheelRimWidth/2.0, _TruckWheelRimRadius, truckTireOutRadius, _TruckTireMaterial,
                                                         _TruckTireEqThickness, 5.0) ); //Tire vertical wall thickness is supposed to be equivalent 5.0 mmm
        trkTrF.setPosMatrix ( cylTransfF );
        ++lcyid;

      }

      genelmbs::Cylkind &trkAxB = _trkCyl[lcyid];
      trkAxB._elmName="trckAxis";
      trkAxB._id = genelmbs::CylkindId(lcyid);
      trkAxB._shapeType = genelmbs::Cylkind::full;
      trkAxB._detail.reset( new genelmbs::CylkindDetail(_halfWidth-_TruckWheelRimWidth, 0.0, _TruckWheelAxisRadius, _TruckWheelAxisMaterial) );
      HepGeom::RotateY3D trkAxBRVert(CLHEP::halfpi);
      HepGeom::Translate3D trkAxBpos (0.0,
          truckTireOutRadius-_halfHeight,
          _halfLength-_TruckShaftsLength+_TruckWheelDistFromBack+truckTireOutRadius);
      HepGeom::Transform3D trkAxBTransf (trkAxBpos*trkAxBRVert);
      //        HepGeom::Transform3D trkAxBTransfR (trkAxBposR);
      trkAxB.setPosMatrix ( trkAxBTransf );
      ++lcyid;

      genelmbs::Cylkind &trkAxF = _trkCyl[lcyid];
      trkAxF._elmName="trckAxis";
      trkAxF._id = genelmbs::CylkindId(lcyid);
      trkAxF._shapeType = genelmbs::Cylkind::full;
      trkAxF._detail.reset( new genelmbs::CylkindDetail(_halfWidth-_TruckWheelRimWidth, 0.0, _TruckWheelAxisRadius, _TruckWheelAxisMaterial) );
      HepGeom::RotateY3D trkAxFRVert(CLHEP::halfpi);
      HepGeom::Translate3D trkAxFpos (0.0,
          truckTireOutRadius-_halfHeight,
          _halfLength-_TruckWheelDistFromFront-truckTireOutRadius);
      HepGeom::Transform3D trkAxFTransf (trkAxFpos*trkAxFRVert);
      //        HepGeom::Transform3D trkAxFTransfR (trkAxFposR);
      trkAxF.setPosMatrix ( trkAxFTransf );
      ++lcyid;

      genelmbs::Cylkind &trkTrnJn = _trkCyl[lcyid];
      trkTrnJn._elmName="trckTrnJoin";
      trkTrnJn._id = genelmbs::CylkindId(lcyid);
      trkTrnJn._shapeType = genelmbs::Cylkind::full;
      trkTrnJn._detail.reset( new genelmbs::CylkindDetail(_TrckTrnWheelJnThickness/2.0, 0.0, _TrckTrnWheelJnRadius, _TrckTrnWheelJnEqMaterial) );
      HepGeom::RotateX3D trkTrnJnR(CLHEP::halfpi);
      HepGeom::Translate3D trkTrnJnpos (0.0,
          _TruckChassistsHeight+trkTrnJn._detail->halfLength()-_halfHeight,
          _halfLength-_TruckShaftsLength+_TrckTrnWheelJnDistFromBack);
      HepGeom::Transform3D trkTrnJnTransf (trkTrnJnpos*trkTrnJnR);
      //        HepGeom::Transform3D trkTrnJnTransfR (trkTrnJnposR);
      trkTrnJn.setPosMatrix ( trkTrnJnTransf );
      ++lcyid;

    }

    _ltrkd->_trkBx.reset(_trkBx);
    _ltrkd->_trkCyl.reset(_trkCyl);
    _ltrkd->_containerBx.reset(cntInBx);

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

//void TRUCKMaker::loadAbsorber( crd::SimpleConfig const& config ){
//
//  char tmpVarName[50];
//
//  _nAbsorbLayers   = config.getInt("trck.nAbsLayers",0);
//  for (int il=0; il<_nAbsorbLayers; ++il) {
//    //Absorber Layers Parameters
//    sprintf(tmpVarName,"trck.abs.l%d.InRad",il+1);
//    const std::string inRadVarName(tmpVarName);
//    _AbsorbInRasius.push_back(config.getDouble(inRadVarName));
//
//    if (_AbsorbInRasius[il]<_distIn) { _distIn=_AbsorbInRasius[il]; }
//
//    sprintf(tmpVarName,"trck.abs.l%d.halfLength",il+1);
//    const std::string halflVarName(tmpVarName);
//    _AbsorbHalfLengths.push_back(config.getDouble(halflVarName));
//
//    if (_AbsorbHalfLengths[il]>_halfLength) { _halfLength=_AbsorbHalfLengths[il]; }
//
//    sprintf(tmpVarName,"trck.abs.l%d.nShells",il+1);
//    const std::string nShlVarName(tmpVarName);
//    _AbsorbNmShells.push_back(config.getInt(nShlVarName));
//
//    sprintf(tmpVarName,"trck.abs.l%d.ShellsMaterial",il+1);
//    const std::string AbShlMatVarName(tmpVarName);
//    std::vector< std::string > tmpabMat;
//    config.getVectorString(AbShlMatVarName,tmpabMat,_AbsorbNmShells[il]);
//    _AbsorbShellsMaterial.push_back( tmpabMat );
//
//    sprintf(tmpVarName,"trck.abs.l%d.ShellsThickness",il+1);
//    const std::string AbShlThickVarName(tmpVarName);
//    std::vector<double> tmpabThick;
//    config.getVectorDouble(AbShlThickVarName,tmpabThick,_AbsorbNmShells[il]);
//    _AbsorbShellsThick.push_back( tmpabThick );
//
//    double tmpThick=0.0;
//    for (int iShl=0; iShl<_AbsorbNmShells[il]; ++iShl) {
//      tmpThick += _AbsorbShellsThick[il][iShl];
//    }
//    _AbsorbersThickness.push_back(tmpThick);
//
//    if ((_AbsorbInRasius[il]+_AbsorbersThickness[il])>_distOut) { _distOut=(_AbsorbInRasius[il]+_AbsorbersThickness[il]) ; }
//
//  }
//
//}

//void TRUCKMaker::BuildAbsorber(){
//
//  _lpsabs = unique_ptr<TRUCKdescription>(new TRUCKdescription());
//
//  if (_nAbsorbLayers<1) {
//    exc::exceptionG4 e("GEOM","Warning in Argument",4);
//    e<<"TRUCK: Number of Absorber Shell is 0\n";
//    e.error();
//  }
//
//  if (_isExternal) {
//    //                throw cet::exception("GEOM") <<"Using GDML file option is temporarily disabled\n";
//    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
//    e<<"TRUCK: Using GDML file option is temporarily disabled\n";
//    e.error();
//
//  } else {
//    _lpsabs->_nAbsorbLayers = _nAbsorbLayers;
//    _lpsabs->_AbsorbInRasius = _AbsorbInRasius;
//    _lpsabs->_AbsorbHalfLengths = _AbsorbHalfLengths;
//    _lpsabs->_AbsorbNmShells = _AbsorbNmShells;
//    _lpsabs->_AbsorbShellsMaterial = _AbsorbShellsMaterial;
//    _lpsabs->_AbsorbShellsThick = _AbsorbShellsThick;
//    _lpsabs->_AbsorbersThickness = _AbsorbersThickness;
//    for (int ily=0; ily<_nAbsorbLayers; ++ily) {
//      _lpsabs->_AbsorbOutRasius.push_back(0.0);
//      _lpsabs->_AbsorberType.push_back(0);
//    }
//  }
//}

} // namespace trck

//#endif
