// interface to manage the geometries of the Pixel/Strip detector readout
//
// Original author G. Tassielli
//

#ifndef ROGeometryHandle_hh
#define ROGeometryHandle_hh

#include "manageException.hh"
#include "GeomHandle.hh"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Transform3D.h"

//#include "PXSTdetector.hh"
//#include "PXSTbsLadder.hh"
//#include "PXSTbsLayer.hh"

namespace pxstbs {

class Ladder;
class Layer;
class ReadOut;

class ROGeometryHandle {

protected:
        ROGeometryHandle( /*Detector *slt*/);// :
//           _fLayer(-1)
//          ,_fPhiSec(-1)
//          ,_fLadder(-1)
//          ,_fLadderID(-1)
//          ,_fSubShell(-1)
//          ,_fChFstSd(-1)
//          ,_fChSndSd(-1)
//          ,_fROChanID(0)
////          ,_chambID(-1)
//          ,_lay(nullptr)
//          ,pxstdetector(slt)
//{}

public:

        virtual ~ROGeometryHandle();

        virtual void  SelectLadder(int Layer, int PhiSec, int Ladder) {
        	exc::exceptionG4 e("GEOM","Fatal Error",0);
        	e<<"SelectLadder Method is not implemented for the interface class ROGeometryHandle, please use one of the real implementation \n";
        	e<<"Ignored arguments "<<Layer<<" "<<PhiSec<<" "<<Ladder<<"\n";
        	e.error();
        }
        virtual void  SelectRO(int Layer, int PhiSec, int Ladder, int fChFstSd, int fChSndSd, int fSubShell=0) {
        	exc::exceptionG4 e("GEOM","Fatal Error",0);
        	e<<"SelectRO Method is not implemented for the interface class ROGeometryHandle, please use one of the real implementation \n";
        	e<<"Ignored arguments "<<Layer<<" "<<PhiSec<<" "<<Ladder<<" "<<fChFstSd<<" "<<fChSndSd<<" "<<fSubShell<<"\n";
        	e.error();
        }
        virtual void  SelectRODet(unsigned long det) {
        	exc::exceptionG4 e("GEOM","Fatal Error",0);
        	e<<"SelectRODet Method is not implemented for the interface class ROGeometryHandle, please use one of the real implementation \n";
        	e<<"Ignored arguments "<<det<<"\n";
        	e.error();
        }
        virtual void  SelectRO(int chambID, int channelID) {
        	exc::exceptionG4 e("GEOM","Fatal Error",0);
        	e<<"SelectRO Method is not implemented for the interface class ROGeometryHandle, please use one of the real implementation \n";
        	e<<"Ignored arguments "<<chambID<<" "<<channelID<<"\n";
        	e.error();
        }
        virtual unsigned long computeDet(int Layer, int PhiSec, int Ladder, int ChFstSd,int ChSndSd, int subShell=0);
        virtual unsigned long computeDet(int chambID, int channelID);
        virtual int computeChambID(int Layer, int PhiSec, int Ladder);
        virtual int computeChannelID(int ChFstSd,int ChSndSd, int subShell=0);
        virtual void  Global2Local(double *global, double *local) ;
        virtual void  Local2Global(double *local, double *global) ;
        virtual void  Global2Local(HepGeom::Point3D<double> &global, HepGeom::Point3D<double> &local) ;
        virtual void  Local2Global(HepGeom::Point3D<double> &local, HepGeom::Point3D<double> &global) ;
        virtual int   GetLayerId()                                { return _fLayer;   }
        virtual int   GetPhiSecId()                               { return _fPhiSec;  }
        virtual int   GetLadderId()                               { return _fLadder;  }
        virtual int   GetChFstSdId()                              { return _fChFstSd; }
        virtual int   GetChSndSdId()                              { return _fChSndSd; }
        virtual float GetLayerOutsideRad()                        ;
        virtual float GetLayerInsideRad()                         ;
        virtual const CLHEP::Hep3Vector GetROChanCenterLcl () const ;
        virtual const CLHEP::Hep3Vector GetROChanCenterGbl() const  ;
        virtual const CLHEP::Hep3Vector GetROLayerMidPoint()      ;
        virtual double GetROChanFstSdDim() const                  ;
        virtual double GetROChanSndSdDim() const                  ;
        virtual const CLHEP::Hep3Vector GetLadderMidPoint() const                  ;
        virtual const CLHEP::Hep3Vector GetLadderFstSdDir() const                  ;
        virtual const CLHEP::Hep3Vector GetLadderSndSdDir() const                  ;
        virtual bool isFwLayer() const ;
        virtual bool isVertLayer() const ;

        virtual Layer* GetLayer()                              { return _lay; }
        virtual boost::shared_ptr<Ladder> GetLadder()          { return _lad; }
        virtual boost::shared_ptr<ReadOut> GetROChannel();

protected:
        int       _fLayer;
        int       _fPhiSec;
        int       _fLadder;
        int       _fLadderID;
        int       _fSubShell;
        int       _fChFstSd;
        int       _fChSndSd;
        unsigned long       _fROChanID;
        Layer    *_lay;
        boost::shared_ptr<Ladder> _lad;
        HepGeom::Transform3D _matrx;
        HepGeom::Transform3D _invmatrx;
        HepGeom::Point3D<double> tmpGlobal;
        HepGeom::Point3D<double> tmpLocal;
        HepGeom::Point3D<double> _fROChMidLcl;
        HepGeom::Point3D<double> _fROChMidGbl;

//        GeomHandle<PXSTdetector> pxstdetector;
//        Detector *pxstdetector;

};

} //namespace pxstbs

#endif /* ROGeometryHandle_hh */
