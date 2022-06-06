#ifndef rAPPNUWSTReadMCDataCSMBT_H
#define rAPPNUWSTReadMCDataCSMBT_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTReadMCDataCSMBT                                                          //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <util/GeometrySrvHndl.h>

#include "generated/rAPPNUWSTReadMCDataCSMBT_Base.h"

#include "TClonesArray.h"
#include <map>

class rAPPNUWSCSMBTGeantStep;

class rAPPNUWSTReadMCDataCSMBT : public rAPPNUWSTReadMCDataCSMBT_Base
{

private:
   rAPPNUWSTReadMCDataCSMBT(const rAPPNUWSTReadMCDataCSMBT &c); // not implemented
   rAPPNUWSTReadMCDataCSMBT &operator=(const rAPPNUWSTReadMCDataCSMBT &c); // not implemented

public:
   rAPPNUWSTReadMCDataCSMBT(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTReadMCDataCSMBT_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTReadMCDataCSMBT() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();
   void LoadEvent(Int_t nev);
   void DefineHitsInCSMBT();

   TClonesArray* fBrDataTrk;
   TClonesArray* fBrDataCSMBT;
///   TClonesArray* fCSMBTHitsMap;
   GeometrySrvHndl* fGeometry;

   typedef std::map<unsigned long,std::vector<rAPPNUWSCSMBTGeantStep *> > CSMBTChHitMap;

   void fillHitMap ( CSMBTChHitMap& hitmap );

   double _minimumEnergy;  // minimum energy deposition of G4 step

   ClassDef(rAPPNUWSTReadMCDataCSMBT,0)
};

#endif   // rAPPNUWSTReadMCDataCSMBT_H
