// CARGOdescription manager class

#ifndef CARGOdescription_hh
#define CARGOdescription_hh

//#include <vector>

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "Detector.hh"

#include "GENELMbsBoxkind.hh"
#include "GENELMbsCylkind.hh"

namespace crg {

class CARGOdescription : public Detector {

  friend class CARGOMaker;

public:
  CARGOdescription();
  ~CARGOdescription() {}

  std::string extFile()  const { return _extFile; }
  bool isExternal()      const { return _isExternal; }

  int geomType()         const { return _geomType; }

  int nBoxKindElem()     const { return _nCrgBx; }
  int nCylKindElem()     const { return _nCrgCyl; }

  genelmbs::Boxkind* getCrgBoxKindElem(int n) const;

  boost::shared_array<genelmbs::Boxkind> getCrgBoxKindElemArray() const {
          return _crgBx;
  }

  genelmbs::Cylkind* getCrgCylKindElem(int n) const;

  boost::shared_array<genelmbs::Cylkind> getCrgCylKindElemArray() const {
          return _crgCyl;
  }

  int nBoxKindTargetElem() const { return _nCrgBxTrgt; }
  int nCylKindTargetElem() const { return _nCrgCylTrgt; }

  genelmbs::Boxkind* getCrgBoxKindTargetElem(int n) const;
  genelmbs::Cylkind* getCrgCylKindTargetElem(int n) const;

  bool checkBoxKindElemIsTarget (int ibxel) const;
  bool checkCylKindElemIsTarget (int icylel) const;

protected:

  // Name of external gdml geometry file description.
  std::string _extFile;
  bool _isExternal;

  int _geomType;

  int _nCrgBx;
  boost::shared_array<genelmbs::Boxkind> _crgBx;
  int _nCrgBxTrgt;
  std::vector<int> _crgBxTrgtId;

  int _nCrgCyl;
  boost::shared_array<genelmbs::Cylkind> _crgCyl;
  int _nCrgCylTrgt;
  std::vector<int> _crgCylTrgtId;

};

} //namespace crg

#endif /* CARGOdescription_hh */
