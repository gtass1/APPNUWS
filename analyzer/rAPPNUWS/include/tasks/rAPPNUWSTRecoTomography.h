#ifndef rAPPNUWSTRecoTomography_H
#define rAPPNUWSTRecoTomography_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTRecoTomography                                                    //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "generated/rAPPNUWSTRecoTomography_Base.h"

#include "GeometrySrvHndl.h"

class rAPPNUWSTRecoTomography : public rAPPNUWSTRecoTomography_Base
{

private:
   rAPPNUWSTRecoTomography(const rAPPNUWSTRecoTomography &c); // not implemented
   rAPPNUWSTRecoTomography &operator=(const rAPPNUWSTRecoTomography &c); // not implemented

public:
   rAPPNUWSTRecoTomography(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTRecoTomography_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTRecoTomography() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();

   void doPreTomography();

   GeometrySrvHndl   *fGeometry;
//   TClonesArray* fBrDataScatt;
   double sideWidth[3];
   double minVoxPos[3];
   double stepVox[3];
   int nVox[3];
   double maxVoxPos[3];

   ClassDef(rAPPNUWSTRecoTomography,0)
};

#endif   // rAPPNUWSTRecoTomography_H
