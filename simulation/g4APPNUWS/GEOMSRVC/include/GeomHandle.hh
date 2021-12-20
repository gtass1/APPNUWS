#ifndef GeomHandle_hh
#define GeomHandle_hh

//
// A safe pointer to the geometry information for a
// detector component.
//
// Author G. Tassielli
//

#include "GeomService.hh"

//namespace mu2e {
  template <typename DET>
  class GeomHandle
  {
  public:
    GeomHandle()
    {
      _detector = GeomService::Instance()->getElement<DET>();
    }
    ~GeomHandle() { }

    DET const * operator->() const { return _detector;}
    DET const * get()        const { return _detector;}
    DET const & operator*()  const { return *_detector;}
    DET const * operator->() { return _detector;}
    DET const & operator*()  { return *_detector;}

  private:
    GeomHandle(const GeomHandle&);
    GeomHandle& operator=(const GeomHandle&);

    // unnecessary
    DET* operator&();

    DET* _detector;
  };
//}

#endif /* GeomHandle_hh */
