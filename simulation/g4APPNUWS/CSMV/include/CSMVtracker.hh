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

        double r0()            const { return _r0;}
        double z0()            const { return _z0;}
        double rOut()          const { return _rOut;}
        double r0_Fw()            const { return _r0_fwd;}
        double zHalfLength_Fw()   const { return _halfLength_fwd;}
        double rOut_FW()          const { return _rOut_fwd;}
        double zPos_FW()          const { return _zPos_fwd;}

        std::string extFile()  const { return _extFile; }
        bool isExternal()      const { return _isExternal; }

        int nLayers()          const { return _nLayers; }
        int nFwdLayers()       const { return _nFwdLayers; }

        double zHalfLength()   const { return _zHalfLength;}

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
        // _r0 = Nominal radius of the center of the sector.
        // _z0 = position of the center of the tracker relative to the origin
        double _r0;
        double _z0;

        // Outer radius of a logical volume that will just contain the entire tracker.
        double _rOut;

        double _r0_fwd;
        double _halfLength_fwd;
        double _rOut_fwd;
        double _zPos_fwd;

        // Name of external gdml geometry file description.
        std::string _extFile;
        bool _isExternal;

        int _nLayers;
        int _nFwdLayers;

        double _zHalfLength;

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
