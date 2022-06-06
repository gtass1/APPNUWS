#ifndef rAPPNUWSTrAPPNUWSMain_H
#define rAPPNUWSTrAPPNUWSMain_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTrAPPNUWSMain                                                      //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "generated/rAPPNUWSTrAPPNUWSMain_Base.h"

class rAPPNUWSTrAPPNUWSMain : public rAPPNUWSTrAPPNUWSMain_Base
{

private:
   rAPPNUWSTrAPPNUWSMain(const rAPPNUWSTrAPPNUWSMain &c); // not implemented
   rAPPNUWSTrAPPNUWSMain &operator=(const rAPPNUWSTrAPPNUWSMain &c); // not implemented

public:
   rAPPNUWSTrAPPNUWSMain(const char *name = 0, const char *title = 0, int level = 0, const char *taskSuffix = 0, TFolder *histoFolder = 0)
   :rAPPNUWSTrAPPNUWSMain_Base(name,title,level,taskSuffix,histoFolder) {}
   virtual ~rAPPNUWSTrAPPNUWSMain() {}

protected:
   // Event Methods
   void Init();
   void BeginOfRun();
   void Event();
   void EndOfRun();
   void Terminate();


   ClassDef(rAPPNUWSTrAPPNUWSMain,0) // Main task. This task does common initialization for all the tasks.
};

#endif   // rAPPNUWSTrAPPNUWSMain_H
