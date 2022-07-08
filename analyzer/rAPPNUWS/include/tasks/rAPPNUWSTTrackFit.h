#ifndef rAPPNUWSTTrackFit_H
#define rAPPNUWSTTrackFit_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTTrackFit                                                      //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <util/GeometrySrvHndl.h>
#include "generated/rAPPNUWSTTrackFit_Base.h"

#include "generated/rAPPNUWSCSMTTHit.h"
#include "generated/rAPPNUWSCSMBTHit.h"

#include "TVector3.h"
#include "TObject.h"
#include "Track.h"

#include <map>
#include <vector>
#include <utility>

//class rAPPNUWSRecoTracks;

class rAPPNUWSTTrackFit : public rAPPNUWSTTrackFit_Base
{

private:
   rAPPNUWSTTrackFit(const rAPPNUWSTTrackFit &c); // not implemented
   rAPPNUWSTTrackFit &operator=(const rAPPNUWSTTrackFit &c); // not implemented

public:
   rAPPNUWSTTrackFit(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTTrackFit_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTTrackFit() {}

protected:

   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();
//   void StoreReconstructedTrack();
//   Bool_t Fit(rAPPNUWSRecoTracks *aTrack);
   Bool_t Fit2(TObject *aTrack);
   Bool_t Fit2TT(TObject *aTrack);
   Bool_t Fit2BT(TObject *aTrack);
   void MCFinder();
   void addTThit (rAPPNUWSCSMTTHit *ahit, int &ihit, int &ihit0, int &nhits, genfit::Track &fitTrack/*, int &nid*/);
   void addBThit (rAPPNUWSCSMBTHit *ahit, int &ihit, int &ihit0, int &nhits, genfit::Track &fitTrack/*, int &nid*/);
   void FindScatter();
   void matchTracks( std::map<int,int> &trckmatch, std::vector< std::pair<double,double> > &trckmatchVals );


   double distCut;
   double angleCut;

  GeometrySrvHndl   *fGeometry;
//   TClonesArray   *fBrHitsDC;
   TClonesArray   *fBrHitsCSMBT;
   TClonesArray   *fBrHitsCSMTT;
   TClonesArray   *fBrDataTrk;

   Double_t       fTheta;
   Double_t       fPhi;
   TVector3       fVert;
//   Double_t       fChiSquare;
//   Double_t       fStatus;  

   std::vector<genfit::Track*> TTftracks;
   std::vector<genfit::Track*> BTftracks;
   std::vector<genfit::Track*> COMBBTftracks;
   std::vector<genfit::Track*> DISPftracks;

   ClassDef(rAPPNUWSTTrackFit,0)
};

#endif   // rAPPNUWSTTrackFit_H
