#include "FairDbProcProxy.h"
#include "FairDbProc.h"
#include "FairDbLogService.h"


FairDbProcProxy::FairDbProcProxy()
{
  fName = "<null>";
}


FairDbProcProxy::~FairDbProcProxy(){}

const char *FairDbProcProxy::GetName() const
{ 
  return fName;
}

const char *FairDbProcProxy::GetFunc() const
{ 
  return fFunc;
}

void FairDbProcProxy::Help()
{
  FairDbProc* m = this->CreateModule();
  if (m) {
    m->Help();
    delete m;
  }
}

FairDbProc *FairDbProcProxy::CreateModule()
{
  DBLOG("FairDbDS", FairDbLog::kError) 
    << "Attempt to create null process '" << fName << "'\n";
  return 0;
}
