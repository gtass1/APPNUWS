#ifndef rAPPNUWSTRecoExtMuonTomography_H
#define rAPPNUWSTRecoExtMuonTomography_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTRecoExtMuonTomography                                             //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "generated/rAPPNUWSTRecoExtMuonTomography_Base.h"

#include "GeometrySrvHndl.h"
#include "MutoVReconstruction.h"

#include "TRotation.h"

class rAPPNUWSTRecoExtMuonTomography : public rAPPNUWSTRecoExtMuonTomography_Base
{

private:
   rAPPNUWSTRecoExtMuonTomography(const rAPPNUWSTRecoExtMuonTomography &c); // not implemented
   rAPPNUWSTRecoExtMuonTomography &operator=(const rAPPNUWSTRecoExtMuonTomography &c); // not implemented

public:
   rAPPNUWSTRecoExtMuonTomography(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTRecoExtMuonTomography_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTRecoExtMuonTomography() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();

   void DataFill();
//   void doPreTomography();
   void savePreTomography(extmutom::Image &img);

   GeometrySrvHndl   *fGeometry;
//   TClonesArray* fBrDataScatt;
   double sideWidth[3];
   double minVoxPos[3];
   double stepVox[3];
   int nVox[3];
   double maxVoxPos[3];

//   std::string method;
   extmutom::MutoVReconstruction* rec;
   extmutom::ImageHeader header;
   extmutom::MutoMuonData data;
   TRotation convRot;
   TRotation convRotInv;

   ClassDef(rAPPNUWSTRecoExtMuonTomography,0)
};

#endif   // rAPPNUWSTRecoExtMuonTomography_H
