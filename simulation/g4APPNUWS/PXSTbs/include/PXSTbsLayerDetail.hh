//
// Original author G. Tassielli
//

#ifndef PXSTbs_LayerDetail_hh
#define PXSTbs_LayerDetail_hh

#include <string>

namespace pxstbs {

class LayerDetail{

public:
  LayerDetail();

  LayerDetail( double inRad, double outRad, double halfLength, double zpos=0.0, double ypos=0.0  );

  ~LayerDetail ();

  double      InnerRadius()   const { return _InnerRadius;}
  double      OuterRadius()   const { return _OuterRadius;}
  double      halfLength()    const { return _halfLength;}
  double      yPosition()     const { return _ypos;}
  double      zPosition()     const { return _zpos;}

private:

  double _InnerRadius;
  double _OuterRadius;
  double _halfLength;
  double _ypos;
  double _zpos;

};

}  //namespace pxstbs

#endif /* PXSTbs_LayerDetail_hh */
