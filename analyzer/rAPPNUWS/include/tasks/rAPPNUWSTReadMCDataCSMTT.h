#ifndef rAPPNUWSTReadMCDataCSMTT_H
#define rAPPNUWSTReadMCDataCSMTT_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTReadMCDataCSMTT                                                         //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <util/GeometrySrvHndl.h>


#include "generated/rAPPNUWSTReadMCDataCSMTT_Base.h"

#include "TClonesArray.h"
#include <map>

class rAPPNUWSCSMTTGeantStep;

class rAPPNUWSTReadMCDataCSMTT : public rAPPNUWSTReadMCDataCSMTT_Base
{

private:
   rAPPNUWSTReadMCDataCSMTT(const rAPPNUWSTReadMCDataCSMTT &c); // not implemented
   rAPPNUWSTReadMCDataCSMTT &operator=(const rAPPNUWSTReadMCDataCSMTT &c); // not implemented

public:
   rAPPNUWSTReadMCDataCSMTT(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTReadMCDataCSMTT_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTReadMCDataCSMTT() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();
   void LoadEvent(Int_t nev);
   void DefineHitsInCSMTT();

   TClonesArray* fBrDataTrk;
   TClonesArray* fBrDataCSMTT;
   GeometrySrvHndl* fGeometry;

   typedef std::map<unsigned long,std::vector<rAPPNUWSCSMTTGeantStep *> > CSMTTChHitMap;

   void fillHitMap ( CSMTTChHitMap& hitmap );

   double _minimumEnergy;  // minimum energy deposition of G4 step

   ClassDef(rAPPNUWSTReadMCDataCSMTT,0)
};

#endif   // rAPPNUWSTReadMCDataCSMTT_H
