//
// Original author G. Tassielli
//

#include "DCHbsCellWallDetail.hh"

namespace dchbs {

  CellWallDetail::CellWallDetail( std::vector<double> & thicknesses,
                   std::vector<std::string> & materialNames
				   ):
    _materialNames(materialNames),
    _shellsThicknesses(thicknesses)
  {
          _totThick=0.;
          std::vector<double>::iterator ithick = thicknesses.begin();
          while(ithick!= thicknesses.end()){
                  _totThick += *ithick;
                  ++ithick;
          }
  }

  CellWallDetail::~CellWallDetail (){
  }

} // namespace dchbs
