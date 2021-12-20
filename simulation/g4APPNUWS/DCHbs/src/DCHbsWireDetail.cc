//
// Original author G. Tassielli
//

#include "DCHbsWireDetail.hh"

namespace dchbs {

  WireDetail::WireDetail( std::vector<double> & thicknesses,
                   std::vector<std::string> & materialNames,
               double halfLength
                           ):
    _materialNames(materialNames),
    _shellsThicknesses(thicknesses),
    _halfLength(halfLength)
  {
          _radius=0.;
          std::vector<double>::iterator ithick = thicknesses.begin();
          while(ithick!= thicknesses.end()){
                  _radius += *ithick;
                  ++ithick;
          }
  }

  WireDetail::~WireDetail (){
  }

} // namespace dchbs
