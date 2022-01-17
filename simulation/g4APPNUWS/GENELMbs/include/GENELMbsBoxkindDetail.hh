//
// Original author G. Tassielli
//

#ifndef BoxkindDetail_hh
#define BoxkindDetail_hh

#include <string>
#include <vector>

#include "manageException.hh"

namespace genelmbs {

class BoxkindDetail{

public:
  BoxkindDetail();

  BoxkindDetail( double halfLength, double halfWidth, double halfHeight, std::string materialName, double wallThicknesses=0.0, double centWallThickness=0.0 );

  ~BoxkindDetail ();

  double      halfLength()         const { return _halfLength;}
  double      halfWidth()          const { return _halfWidth;}
  double      halfHeight()         const { return _halfHeight;}
  double      wallThickness()      const { return _wallThickness;}
  double      centWallThickness()  const { return _centWallThickness;}

  std::string const materialName() const { return _materialName;  }

private:

  double _halfLength;
  double _halfWidth;
  double _halfHeight;
  double _wallThickness;
  double _centWallThickness;

  std::string _materialName;

};

}  //namespace genelmbs

#endif /* BoxkindDetail_hh */
