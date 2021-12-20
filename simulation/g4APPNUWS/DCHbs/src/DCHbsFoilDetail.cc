//
// Original author G. Tassielli
//

#include "DCHbsFoilDetail.hh"

namespace dchbs {

  FoilDetail::FoilDetail( std::vector<double> & thicknesses,
                   std::vector<std::string> & materialNames,
               double halfLength
                           ):
    _materialNames(materialNames),
    _shellsThicknesses(thicknesses),
    _halfLength(halfLength)
  {
          _totThick=0.;
          std::vector<double>::iterator ithick = thicknesses.begin();
          while(ithick!= thicknesses.end()){
                  _totThick += *ithick;
                  ++ithick;
          }
  }

  FoilDetail::~FoilDetail (){
  }

} // namespace dchbs
