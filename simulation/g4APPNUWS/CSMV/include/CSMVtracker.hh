// CSMVtracker manager class
//
// Original author G. Tassielli
//

#ifndef CSMVtracker_hh
#define CSMVtracker_hh

#include <deque>
#include <vector>
#include <map>
#include <iterator>
#include <memory>
#include <string>

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "PXSTbsLayer.hh"

#include "CSMVROGeometryHandle.hh"

#include "Detector.hh"

namespace csmv {

class CSMVtracker: public Detector {

        friend class CSMVMaker;

public:
        CSMVtracker(std::string name="CSMVtracker");
        ~CSMVtracker() {}

        double x0()            const { return _x0;}
        double y0()            const { return _y0;}
        double z0()            const { return _z0;}
        double distIn()          const { return _distIn;}
        double distOut()          const { return _distOut;}

        std::string extFile()  const { return _extFile; }
        bool isExternal()      const { return _isExternal; }

        int nLayers()          const { return _nLayers; }

        double zHalfLength()   const { return _zHalfLength;}
        double halfWidth()   const { return _halfWidth;}
        double halfThickness()   const { return _halfThickness;}

        int geomType()         const { return _geomType; }

//        SensorType sensType()  const { return _sensType; }

//        pxstbs::ROGeometryHandle* getROGeometryHandle() const;
        CSMVROGeometryHandle* getROGeometryHandle() const;

        pxstbs::Layer* getLayer(int n) const /*throw(cet::exception)*/;

        boost::shared_array<pxstbs::Layer> getLayersArray() const {
                return _lr;
        }

protected:
        // Nominal values.
        // _x0 = position of the center of the tracker relative to the origin
        // _y0 = position of the center of the tracker relative to the origin
        // _z0 = position of the center of the tracker relative to the origin
        double _x0;
        double _y0;
        double _z0;

        // distance from the center of the closer face of the logical volume that will just contain the entire tracker.
        double _distIn;

        // distance from the center of the farther face of the logical volume that will just contain the entire tracker.
        double _distOut;

        // Name of external gdml geometry file description.
        std::string _extFile;
        bool _isExternal;

        int _nLayers;

        double _zHalfLength;           //Nominal Half-Length of the tracker along the z axis
        double _halfWidth;             //Nominal Half-Width of the tracker along the second axis
        double _halfThickness;         //Nominal Half-Thickness of the tracker along the third axis

        //decimal digit:magior version, unit digit:minor version  00: Dummy (Simplified Si tracker, just cylindrical shell for layer)

        int _geomType;

        //Sensor type: 0:Pixel, 1:Strip
//        SensorType _sensType;

        boost::shared_array<pxstbs::Layer> _lr;

//        std::unique_ptr<pxstbs::ROGeometryHandle> _roChhnd;
        boost::shared_ptr<CSMVROGeometryHandle> _roChhnd;

};

} //namespace csmv

#endif /* CSMVtracker_hh */
