#include "FairDbProc.h"
#include <FairDbLogService.h>

#include "FairDbProcCmd.h"

static const char *empty_msg = "Process does not implement: ";

FairDbProc::FairDbProc() : fName(""), fConfig(false) {
}

FairDbProc::~FairDbProc() {
}

const char *FairDbProc::GetName() const {
  return fName.c_str();
}

const FairRegistry& FairDbProc::DefaultConfig() const
{
  DBLOG("FairDbDS",FairDbLog::kDebug) << 
    "FairDbProc::DefaultConfig for module " << this->GetName() << "\n";

  static const FairRegistry r; 
  return r;
}

void FairDbProc::Config(const FairRegistry& r) 
{
  DBLOG("FairDbDS",FairDbLog::kDebug) << 
    "FairDbProc::Config for module " << this->GetName() << "\n";
  
  // The default config command just does a copy...
  bool islocked = this->GetConfig().ValuesLocked();
  if (islocked) this->GetConfig().UnLockValues();
  this->GetConfig() = r;
  if (islocked) this->GetConfig().LockValues();  
}

void FairDbProc::Cmd(const char* cmd) 
{

  FairDbProcCmd c(cmd);
  this->HandleCommand(&c);
}

void FairDbProc::HandleCommand(FairDbProcCmd *command) 
{
  if (command) {
    DBLOG("FairDbDS", FairDbLog::kWarning) 
      << empty_msg << "'HandleCommand': " << command->PopCmd() << endl;
  }
  else {
    DBLOG("FairDbDS", FairDbLog::kWarning) << empty_msg << "'HandleCommand':\n";
  }
}

void FairDbProc::Reset() 
{
  this->Config(this->DefaultConfig());
}

FairRegistry& FairDbProc::GetConfig() 
{ 
  return fConfig; 
}

void FairDbProc::Help() 
{
  DBLOG("FairDbDS", FairDbLog::kWarning) << 
    "No help available for process '" << this->GetName() << "'\n";
}

void FairDbProc::Init(const char* name) 
{
  fName = name;
  FairRegistry r = this->DefaultConfig();
  this->GetConfig().UnLockValues();
  this->GetConfig() = r;
  this->GetConfig().SetName(r.GetName());
  this->Config(this->GetConfig());
  this->GetConfig().LockKeys();
}

