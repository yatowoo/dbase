#include "FairDbProcArgs.h"

extern "C" {
#ifdef MACOSX
  #include <unistd.h>
#else
  #include <getopt.h>
#endif
#ifndef GETOPTDONE 
#define GETOPTDONE (-1)
#endif
#include <sys/types.h> // open/close dir
#include <dirent.h>    // open/close dir
}

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <csignal>
#include "TEnv.h"
#include "TROOT.h"
#include "TSystem.h"
#include "FairDbProcRootArgs.h"
//#include "FairDbCleaner.h"
#include "FairDbFloatXtrap.h"
#include "FairDbArgMacFile.h"
#include "FairDbLogService.h"


FairDbProcArgs* FairDbProcArgs::fInstance = 0;
bool     gsSIGHUP  = false; // SIGHUP signal?
bool     gsSIGTERM = false; // SIGTERM signal?
bool     gsSIGUSR1 = false; // SIGUSR1 signal?


static void gsSIGHUPhandler(int /* s */) { gsSIGHUP = true; }
static void gsSIGTERMhandler(int /* s */) { gsSIGTERM = true; }
static void gsSIGUSR1handler(int /* s */) { gsSIGUSR1 = true; }

int FairDbProcArgsSetenv(const char *name, const char *value) {
  char *s = new char [strlen(name)+strlen(value)+2];  
  sprintf(s,"%s=%s",name,value);
  int success = ::putenv(s);

  return success;
}

FairDbProcArgs::FairDbProcArgs() :
  fArgc(0),
  fArgv(0), 
  fModuleHelpList(0),
  fDfltOutFile(""),
  fTimeStart(time(0)),
  fTimeLimit(-1.0),
  fRecordLimit(-1),
  fIsBatch(false), 
  fRootEnv(0)
{

}

FairDbProcArgs::FairDbProcArgs(int argc, char** argv) :
  fArgc(argc), 
  fArgv(argv), 
  fModuleHelpList(0),
  fDfltOutFile(""),
  fTimeStart(time(0)),
  fTimeLimit(-1.0),
  fRecordLimit(-1),
  fIsBatch(false),
  fRootEnv(0)
{ 
  this->ProcessCommandLine(argc,argv);

  fRootEnv = new FairDbProcRootArgs(argc,argv);
  assert(fRootEnv);

  this->SetSignalHandlers();
}

FairDbProcArgs::~FairDbProcArgs() 
{ 
  if (fRootEnv) {
    delete fRootEnv; fRootEnv = 0;
  }
}

const char* FairDbProcArgs::GetModuleHelpList() const 
{
  return fModuleHelpList;
}

int FairDbProcArgs::GetArgc() const { return fArgc; }


char* const* FairDbProcArgs::GetArgv() const { return fArgv; }

const char* FairDbProcArgs::GetArgv(int i) const 
{ 
  assert(i>=0 && i<fArgc);
  return fArgv[i]; 
}

const char* FairDbProcArgs::GetMacroFile(unsigned int i) const
{

  if (i<this->fMacroFileList.size()) return fMacroFileList[i].c_str();
  else                               return 0;
}

const char* FairDbProcArgs::GetFileName(int i) const 
{
  assert(i>=0 && i<this->GetNfile());
  return fDataFileList[i].c_str();
}

int FairDbProcArgs::GetNfile() const 
{
  return fDataFileList.size();
}

const char* FairDbProcArgs::GetDefaultOutputFileName() const 
{
  return fDfltOutFile.c_str();
}

bool FairDbProcArgs::HaveInaccessibleFile() const
{
    if (!fRootEnv) return false;
    return fRootEnv->HaveInaccessibleFile();
}

void FairDbProcArgs::SetTimeLimit(const char* s) 
{

  int i;                  // 
  float t;                // 
  char unit[256];         // 
  double fac;             // 
  const char* second[] = {"s","second","seconds","sec", "secs", 0};
  const char* minute[] = {"m","minute","minutes","min", "mins", 0};
  const char* hour[]   = {"h","hour","hours","hr","hrs",        0};
  const char* day[]    = {"d","day","days",                     0};
  const char* week[]   = {"w","week","weeks","wk","wks",        0};
  const char* year[]   = {"y","year","years","yr","yrs",        0};

  sscanf(s,"%f %s",&t,unit);
  fac = 0.0;
  for (i=0; second[i]!=0; ++i) {
    if (strcasecmp(unit,second[i])==0) fac = 1.0;
  }
  for (i=0; minute[i]!=0; ++i) {
    if (strcasecmp(unit,minute[i])==0) fac = 60.0;
  }
  for (i=0; hour[i]!=0; ++i) {
    if (strcasecmp(unit,hour[i])==0)   fac = 60.0*60.0;
  }
  for (i=0; day[i]!=0; ++i) {
    if (strcasecmp(unit,day[i])==0)    fac = 60.0*60.0*24.0;
  }
  for (i=0; week[i]!=0; ++i) {
    if (strcasecmp(unit,week[i])==0)   fac = 60.0*60.0*24.0*7.0;
  }
  for (i=0; year[i]!=0; ++i) {
    if (strcasecmp(unit,year[i])==0)   fac=60.0*60.0*24.0*365.25;
  }
  if (fac==0.0 && strlen(unit)>0) {
    DBLOG("FairDbDS",FairDbLog::kWarning) <<
      "Time unit '"<<unit<<"' is not recognized.\n" <<
      "Try 's', 'm', 'h', 'd', 'w', or 'y'\n" <<
      "Assuming unit given is seconds.\n";
    fac = 1.0;
  }
  
  // Set the time limit
  fTimeLimit = t*fac;
}

void FairDbProcArgs::SetSignalHandlers()
{
  signal(SIGHUP,gsSIGHUPhandler);    
  signal(SIGTERM,gsSIGTERMhandler);  
  signal(SIGUSR1,gsSIGUSR1handler);  // signal emitted 

  bool activateFPE = true;

  if(gEnv) {
    if(strcasecmp(gEnv->GetValue("fairdb.fpe","on"),"off")==0) activateFPE=false;
  } 
  static FairDbFloatXtrap floatXtrap(activateFPE); // 

}

bool FairDbProcArgs::CheckRecordLimit(int n) const
{
  if (fRecordLimit<0)  return false; 
  if (n>=fRecordLimit) return true;
  return false;
}

bool FairDbProcArgs::CheckTimeLimit(int* tsec) const
{
  if (fTimeLimit<0.0) return false; //

  time_t t = time(0);
  *tsec = t; 
  if (difftime(t,fTimeStart) >= fTimeLimit) return true;
  
  return false;
}

bool FairDbProcArgs::ContinueRun(int n) const
{

  int t;
  if (this->CheckRecordLimit(n) == true) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << 
      "Record limit exceeded at "<<n<<" records." << endl;
    return false;
  }
  if (this->CheckTimeLimit(&t) == true) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << 
      "Time limit exceeded at "<<t<<" seconds." << endl;
    return false;
  }
  if (gsSIGHUP) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << 
      "Process received SIGHUP.\n" << endl;
    return false;
  }
  if (gsSIGTERM) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << 
      "Process received SIGTERM.\n" << endl;
    return false;
  }
  if (gsSIGUSR1) {
    int run;
    FairDbLogService::Instance()->GetCurrentRun(run);
    cout << flush;  
    cerr << "FairDbProcArgs: Process " << gSystem->GetPid() 
         << " received SIGUSR1 after "
         << n << " record sets," 
         << endl
         <<"  last processed run " << run  
         << flush << endl;
    ProcInfo_t pi;
    gSystem->GetProcInfo(&pi);
    cerr << "  CPU: user " << pi.fCpuUser << "s, sys " 
         << pi.fCpuSys << "s, "
         << " Mem: res " << pi.fMemResident 
         << "k, virtual " << pi.fMemVirtual << "k "
         << flush << endl;

    gsSIGUSR1 = false;  // message is emitted 
  }
  return true; // 
}

bool FairDbProcArgs::IsBatch() const { return fIsBatch; }



FairDbProcArgs& FairDbProcArgs::Instance() 
{
  if (fInstance) return *fInstance;
  return FairDbProcArgs::Instance(0,0);
}


FairDbProcArgs& FairDbProcArgs::Instance(int argc, char** argv)
{
  if (fInstance) return *fInstance;


  static FairDbProcArgs::Cleaner c;
  c.ClassIsUsed();

  fInstance = new FairDbProcArgs(argc, argv);
  return *fInstance;
}

int FairDbProcArgs::RunRootApp() 
{

  if (fRootEnv) {
    int r = fRootEnv->RunTheApp();
    //FairDbDSleaner::Instance().Reap();
    return r;
  }
  return 0;
}

void FairDbProcArgs::ProcessCommandLine(int argc, char *const *argv)
{
  int c;
  
#ifndef MACOSX
  optind = 0; //
#else
  optind = 1; //
#endif

  while ((c = getopt(argc, argv, "bs:nqlt:r:x:hH:d:u:p:o:v:")) != GETOPTDONE) {
    DBLOG("FairDbDS",FairDbLog::kDebug) << "Processing  command-line option argument: " << c << endl;
    switch (c) {
    case 'b': break; // 
    case 's': break; // 
    case 'q': break; // 
    case 'n': break; // 
    case 'l': break; // 
    case 't': this->SetTimeLimit(optarg);  break; // 
    case 'r': fRecordLimit = atoi(optarg); break; // 
    case 'x': //
      this->AddMacroFile(optarg); // 
      break;
    case 'h':
      DBLOG("FairDbDS",FairDbLog::kInfo) 
        << " usage: " << argv[0] 
	<< " -bnq -H<modules> -x<file>"
	<< " -d<url> -u<user> -p<passwd> -t'<time>' -r[n]"
	<< " -o<filename> <filenames...>\n"
	<< "  -b: Run in batch mode without graphics.\n"
	<< "  -n: Do not execute logon and logoff macros.\n"
	<< "  -q: Exit after processing command line macro files.\n"
	<< "  -h: Print this friendly help message.\n"
	<< "  -H: Print help for modules in list.\n"
	<< "  -x: Specifies a Job Control Macro to load.\n"
	<< "      Multiple instances of '-x' are allowed.\n"
	<< "  -d: Supply database URL (replaces FAIRDB_TSQL_URL)\n"
	<< "  -u: Supply database user (replaces FAIRDB_TSQL_USER)\n"
	<< "  -p: Supply database password (replaces FAIRDB_TSQL_PSWD)\n" 
	<< "  -t: Specify job time limit. Eg. -t'10 minutes'\n"
	<< "  -r: Specify limit on number of records to process.\n"
	<< "  -o: Set the name of the default output file.\n"
        << "  -v: Set FairDbDS message verbosity [V,D,I,W,E,F].\n";
      exit(1);
    case 'H': fModuleHelpList = optarg; break;
    case 'd': if (optarg) FairDbProcArgsSetenv("FAIRDB_TSQL_URL", optarg); break;
    case 'u': if (optarg) FairDbProcArgsSetenv("FAIRDB_TSQL_USER",optarg); break;
    case 'p': if (optarg) FairDbProcArgsSetenv("FAIRDB_TSQL_PSWD",optarg); break;
    case 'o': fDfltOutFile = optarg; break;
    case 'v': // set the FairDbLogService level
      {
        FairDbLog::LogLevel_t lvl = FairDbLog::kInfo;
        switch (optarg[0]) {
        case 'V': case 'v': lvl = FairDbLog::kVerbose; break;
        case 'D': case 'd': lvl = FairDbLog::kDebug;   break;
        case 'I': case 'i': lvl = FairDbLog::kInfo;    break;
        case 'W': case 'w': lvl = FairDbLog::kWarning; break;
        case 'E': case 'e': lvl = FairDbLog::kError;   break;
        case 'F': case 'f': lvl = FairDbLog::kFatal;   break;
        default:
          DBLOG("FairDbDS",FairDbLog::kWarning)
            << "Can not interpret -v level '" << optarg << "'." << endl;
        }
        FairDbLogStream *s = FairDbLogService::Instance()->GetStream("FairDbDS");
        if (s) s->SetLogLevel(lvl);
        else
          DBLOG("FairDbDS",FairDbLog::kWarning)
            << "Can not find FairDbDS message stream." << endl;
        break;
      }
    default: // Do nothing just pass them along...
      DBLOG("FairDbDS",FairDbLog::kInfo)
        << " getopt returned unexpected character code " << c << "." << endl;
      break;
    }
  }
  for (int i=optind; i<argc; ++i) {
    int len = strlen(argv[i]);
    if (strcmp(argv[i]+len-5,".root")==0) {
      this->AddDataFile(argv[i]);
      DBLOG("FairDbDS",FairDbLog::kDebug)
        << "FairDbProcArgs::ProcessCommandLine() added data file: "
        << argv[i] << endl;
    }
    else if ( FairDbArgMacFile(argv[i]) != "" ) {
      DBLOG("FairDbDS",FairDbLog::kDebug)
        << "FairDbProcArgs::ProcessCommandLine() found macro script: "
        << argv[i] << endl;
    }
    else {
      DIR* d = opendir(argv[i]);
      if (d) {
         closedir(d);
        DBLOG("FairDbDS",FairDbLog::kDebug)
          << "FairDbProcArgs::ProcessCommandLine() found directory: "
          << argv[i] << endl;
      }
      else {
        DBLOG("FairDbDS",FairDbLog::kInfo)
          << "FairDbProcArgs::ProcessCommandLine() unrecognizable arg: "
          << argv[i] << endl;
      }
    }
  } // 

}

void FairDbProcArgs::AddDataFile(const char *filename)
{
  assert(filename);
  string s(filename);
  fDataFileList.push_back(s);
}

void FairDbProcArgs::AddMacroFile(const char *filelist) 
{
  const char *p;
  for (p=filelist; *p!='\0';) {
    for (; *p==' ' && *p!='\0'; ++p);
    if (*p!='\0') {
      string s;
      s = "";
      for (; *p!=' ' && *p!='\0'; ++p) s += *p;
      if (s.length()>0) fMacroFileList.push_back(s);
    }
  }
}
