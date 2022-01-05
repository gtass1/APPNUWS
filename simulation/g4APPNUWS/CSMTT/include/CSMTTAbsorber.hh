// CSMTTAbsorber manager class

#ifndef CSMTTAbsorber_hh
#define CSMTTAbsorber_hh

#include <vector>
#include "Detector.hh"

namespace csmtt {

class CSMTTAbsorber : public Detector {

  friend class CSMTTMaker;

public:
  CSMTTAbsorber();
  ~CSMTTAbsorber() {}

  std::vector<double> getAbsorbersThickness() const {
    return _AbsorbersThickness;
  }

  std::vector<int> getAbsorbType() const {
    return _AbsorberType;
  }

  std::vector<double> getAbsorbInRasius() const {
    return _AbsorbInRasius;
  }

  std::vector<double> getAbsorbOutRasius() const {
    return _AbsorbOutRasius;
  }

  std::vector<double> getAbsorbHalfLengths() const {
    return _AbsorbHalfLengths;
  }

  std::vector<int> getAbsorbNmShells() const {
    return _AbsorbNmShells;
  }

  std::vector< std::vector<std::string> > getAbsorbShellsMaterial() const {
    return _AbsorbShellsMaterial;
  }

  std::vector< std::vector<double> > getAbsorbShellsThick() const {
    return _AbsorbShellsThick;
  }

  int getAbsorbLayers() const {
    return _nAbsorbLayers;
  }

protected:
  //Absorber material description
  int _nAbsorbLayers;
  std::vector<int> _AbsorberType; //0: Barrel (Cylinder) 1: Forward (Disk)
  std::vector<double> _AbsorbInRasius;
  std::vector<double> _AbsorbOutRasius;
  std::vector<double> _AbsorbHalfLengths;
  std::vector<int>    _AbsorbNmShells;
  std::vector< std::vector<std::string> > _AbsorbShellsMaterial;
  std::vector< std::vector<double> > _AbsorbShellsThick;
  std::vector< double > _AbsorbersThickness;

};

} //namespace csmtt

#endif /* CSMTTAbsorber_hh */
