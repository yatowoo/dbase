#include "FairDbProcRegistry.h"
#include <cstdio>
#include <cstring>
#include "TSystem.h" 
#include "FairDbLogService.h"

FairDbProcRegistry* FairDbProcRegistry::fInstance = 0;

ostream& operator<<(ostream& os, const FairDbProcRegistry& jcr) 
{
  list<FairDbProcProxy*>::const_iterator itrModuleTable;
  os << "Registered  precesses:\n";
  for (itrModuleTable  = jcr.fModuleTable.begin();
       itrModuleTable != jcr.fModuleTable.end();
       ++itrModuleTable) {
    os << " " << 
      (*itrModuleTable)->GetName() << " - " <<
      (*itrModuleTable)->GetFunc() << "\n";
  }
  return os;
}


FairDbProcRegistry::~FairDbProcRegistry() {}


void FairDbProcRegistry::Register(FairDbProcProxy* proxy)
{
  const char* modName = proxy->GetName();
  if (this->LookUp(modName,false)) {
    return;
  }
  else {
    list<FairDbProcProxy*>::iterator itrModuleTable;
    for (itrModuleTable  = fModuleTable.begin();
	 itrModuleTable != fModuleTable.end();
	 ++itrModuleTable) {
      if (strcmp(modName,(*itrModuleTable)->GetName())<0) {
	break;
      }
    }
    fModuleTable.insert(itrModuleTable,proxy);
#ifdef SITE_HAS_SIGC 
    this->SigRegister(modName);
#endif
    DBLOG("FairDbDS", FairDbLog::kDebug) 
      << "Registered process " << modName << " " 
      << proxy << ".\n";
  }
}

FairDbProcProxy* FairDbProcRegistry::LookUp(const char *name, bool loadlib)
{
  list<FairDbProcProxy*>::iterator itrModuleTable;
  for (itrModuleTable  = fModuleTable.begin();
       itrModuleTable != fModuleTable.end();
       ++itrModuleTable) {
    if (strcmp(name,(*itrModuleTable)->GetName())==0) {
      return (*itrModuleTable);
    }
  }
  if (loadlib && this->CheckLibMap(name)) return this->LookUp(name);
  return 0;
}

FairDbProcRegistry::FairDbProcRegistry()
{
#ifdef DEBUG_PROC
#include <FairDbLogStream.h>
  MsgService::Instance()->GetStream("FairDbDS")->SetLogLevel(FairDbLog::kVerbose);
#endif
  this->BuildModToLibMap();
}

FairDbProcRegistry& FairDbProcRegistry::Instance() 
{
  static FairDbProcRegistry::Cleaner c; // 
  if (fInstance==0) {
    c.ClassIsUsed(); // 
    fInstance = new FairDbProcRegistry;
  }
  return *fInstance;
}


int FairDbProcRegistry::BuildModToLibMap()
{
  FILE* fp = 0;
  const char* tmp = getenv("FAIRROOT_INSTALL");
  if (!tmp) return 0;
  std::string modfile = tmp;
  modfile += "/tmp/fairdbdslibmap.txt";
  if ( (fp=fopen(modfile.c_str(),"r"))==0 ) {
    tmp = getenv("FAIRROOT_INSTALL");
    if (!tmp) return 0;
    std::string modfile = tmp;
    modfile += "/tmp/fairdbdslibmaps.txt";
    if ( (fp=fopen(modfile.c_str(),"r"))==0 ) {
      modfile = "/tmp/fairdbdslibmaps.txt";
      if ( (fp=fopen(modfile.c_str(),"r"))==0 ) {
        // nothing found
	return 0;
      }
    }
  }
  std::string lib = "";
  char buff[256];
  while (fgets(buff,255,fp)) {
    if (buff[0]=='#') continue;
    const char* b = buff;
    for (; *b==' ' && *b!='\0'; ++b);
    int len = strlen(buff)-1;
    for (; len>0 && (buff[len]==' '||buff[len]=='\n'); --len);
    buff[len+1] = '\0';

    len = strlen(b);
    if (len<2) continue;
    const char* tag = b + len - 3;
    if (strncmp(b,"lib",3)==0 && strncmp(tag, ".so",3)==0) {
      lib = b;
    }
    else {
      std::string module = b;
      if (fModLibMap[module]=="") {
	fModLibMap[module] = lib;
      }
      else {
	if (!(fModLibMap[module] == lib)) {
	  DBLOG("FairDbDS",FairDbLog::kWarning) << 
	    "Duplicate entry: " << 
	    module << "->" << fModLibMap[module] << "," <<
	    module << "->" << lib                << "." << 
	    std::endl;
	}
      }
    }
  }
  fclose(fp);
  return 1;
}

int FairDbProcRegistry::CheckLibMap(const std::string& module) 
{
  std::string lib = fModLibMap[module];
  if (lib=="") {
    DBLOG("FairDbDS",FairDbLog::kWarning) << 
      "\n" <<
      "Unable to find process '"<<module<<"' in process list.\n" <<
      "Consider regenerating list with 'makemodulemap' command or\n" <<
      "edit the $FAIRROOT_INSTALL/tmp/fairdblibmap.txt file by hand.\n";
    return 0;
  }
  if (gSystem) {
    if (gSystem->Load(lib.c_str())==0) {
      std::map<std::string,std::string>::iterator itr(fModLibMap.begin());
      std::map<std::string,std::string>::iterator itrEnd(fModLibMap.end());
      for (;itr!=itrEnd;++itr) {
	if (itr->first == lib) fModLibMap.erase(itr);
      }
      return 1;
    }
  }
  return 0;
}
