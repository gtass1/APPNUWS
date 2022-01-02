//
// Original author G. Tassielli
//

#include "PXSTbsLayerDetail.hh"

//using namespace std;

namespace pxstbs {

  LayerDetail::LayerDetail():
          _InnerRadius(0.0),
          _OuterRadius(0.0),
          _halfLength(0.0),
          _ypos(0.0),
          _zpos(0.0)
  {}

  LayerDetail::LayerDetail( double inRad, double outRad, double halfLength, double zpos, double ypos ):
              _InnerRadius(inRad),
              _OuterRadius(outRad),
              _halfLength(halfLength),
              _ypos(ypos),
              _zpos(zpos)
  {}

  LayerDetail::~LayerDetail (){
  }

} // namespace pxstbs
