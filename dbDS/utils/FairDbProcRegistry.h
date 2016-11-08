#ifndef FAIRDBPROCREGISTRY_H
#define FAIRDBPROCREGISTRY_H

#include <cassert>
#include <stdlib.h>
#include <list>
#include <map>
#include <string>

#ifndef FAIRDBPROCPROXY_H
#include "FairDbProcProxy.h"
#endif

#ifndef __CINT__ // CINT do not understand this. Max be because it is C++!
# ifdef SITE_HAS_SIGC //
#  ifndef SIGNAL_SYSTEM_H
#   include <sigc++/signal.h>
#   define SIGNAL_SYSTEM_H
#  endif
# endif
#endif

class FairDbProc;

class FairDbProcRegistry
{
  friend std::ostream& operator<<(std::ostream& os, 
				  const FairDbProcRegistry& jcr);
public:
  ~FairDbProcRegistry();

  static FairDbProcRegistry& Instance();

  void             Register(FairDbProcProxy *proxy);
  FairDbProcProxy* LookUp(const char *name, bool loadlibs=true);

private:
  FairDbProcRegistry();
  FairDbProcRegistry(FairDbProcRegistry& /* pcmr */) { abort(); }
  FairDbProcRegistry& operator=(const FairDbProcRegistry& /* pcmr */ ) { 
    abort(); return *this;
  }
  int BuildModToLibMap();
  int CheckLibMap(const std::string& mod);
  struct Cleaner {
    void ClassIsUsed() { } //
    ~Cleaner() {
      if (FairDbProcRegistry::fInstance!=0) {
	delete FairDbProcRegistry::fInstance;
	FairDbProcRegistry::fInstance = 0;
      }
    }
  };
  friend struct Cleaner;

#ifdef SITE_HAS_SIGC //
# ifndef __CINT__
public:
  SigC::Signal1<void,const char*> SigRegister;
# endif
#endif

private:
  static FairDbProcRegistry*        fInstance;   // 
  std::list<FairDbProcProxy*>       fModuleTable;// 
  std::map<std::string,std::string> fModLibMap;  //
};


#define FAIRDBPROCREG(CLASS, NAME, FUNC)                          \
static class gs__Register__##CLASS##__ : public FairDbProcProxy { \
public:                                                           \
  gs__Register__##CLASS##__() {                                   \
    fName = NAME;                                                 \
    fFunc = FUNC;                                                 \
    FairDbProcRegistry::Instance().Register(this);                \
  }                                                               \
  FairDbProc *CreateModule()       {                              \
    FairDbProc* m = new CLASS();                                  \
    m->Init(NAME);                                                \
    return m;                                                     \
  }                                                               \
} gs__module__##CLASS##__proxy__

#endif // FAIRDBPROCREGISTRY_H
