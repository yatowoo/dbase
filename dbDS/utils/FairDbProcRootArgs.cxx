#include "FairDbProcRootArgs.h"

#include <TSystem.h>

#include <iostream>
#include <cassert>     // assert
#include <stdlib.h>    // abort
#include <cstring>     // strlen, strncmp
#include <cstdio>      // sprintf
using namespace std;
extern "C" {
#include <sys/types.h> // opendir, closedir
#include <dirent.h>    // opendir, closedir
#ifdef MACOSX
  #include <unistd.h>
#else
  #include <getopt.h>
#endif
#ifndef GETOPTDONE     
#define GETOPTDONE (-1)
#endif
}
#include "FairDbLogService.h"
// ROOT part
#include "TROOT.h"
#include "TRint.h"
#include "TSystem.h"
#include "TString.h"
#include "TSysEvtHandler.h"
#include "TInterpreter.h"

#include "FairDbArgMacFile.h"

FairDbProcRootArgs::FairDbProcRootArgs(int argc, char** argv) : 
  fHaveBatchOpt(false),
  fHaveQuitOpt(false),
  fHaveMacroFiles(false),
  fHaveInaccessibleFile(false)
{
  static int   localArgc = 0;   // 
  static char* localArgv[1024]; // 
  if (argc>0) {
    localArgv[localArgc++] = argv[0];
  }
  else {
    localArgv[localArgc++] = const_cast<char*>("grillo"); //
  }
  
  // Set Root Globals 
  if (gROOT==0) {
    static TROOT root("TROOT",localArgv[0]);
  }
  assert(gROOT);

  // Create the application
  TApplication* app = gROOT->GetApplication();
  if (app==0) {
    int c;

#ifndef MACOSX
  optind = 0; // 
#else
  optind = 1; // 
#endif

    const char* opts = "bs:nqlt:r:x:hH:d:u:p:o:v:";
    while ((c=getopt(argc, argv, opts)) != GETOPTDONE) {
      DBLOG("FairDbDS",FairDbLog::kDebug)
        << "Processing  command-line ROOT option argument: "
        << c << " '" << (char)c << "'" << endl;
      switch (c) {
      case 'l': localArgv[localArgc++] = const_cast<char*>("-l"); break;
      case 'b': localArgv[localArgc++] = const_cast<char*>("-b"); fHaveBatchOpt = true; break;
      case 'n': localArgv[localArgc++] = const_cast<char*>("-n"); break;
      case 'q': localArgv[localArgc++] = const_cast<char*>("-q"); fHaveQuitOpt  = true; break;
      case 'h': localArgv[localArgc++] = const_cast<char*>("-h"); break;
      default: break;
      }
    }
    int ndir = 0; // 
    for (int i=optind; i<argc; ++i) {
      DIR* d = opendir(argv[i]);
      if (d) {
	localArgv[localArgc++] = argv[i];
	closedir(d);
	++ndir;
      }
      TString fname = FairDbArgMacFile(argv[i]);
      if (fname != "") {
        if (gSystem->AccessPathName(fname.Data())) {
          cerr << "Skipping inaccessible file: " << argv[i] << endl;
	  fHaveInaccessibleFile = true;
          continue;
        }
        fHaveMacroFiles = true;
	localArgv[localArgc++] = argv[i];
      }
    }
    if (ndir>1) {
      DBLOG("FairDbDS",FairDbLog::kError) << 
	"Ambiguous: More than one directory specified on command line.\n";
      exit(1);
    }

    TRint* rint = new TRint("TAPP",&localArgc,localArgv,0,0,kTRUE);
    assert(rint == gROOT->GetApplication());
    std::string p = gSystem->BaseName(localArgv[0]); p += " [%d] ";
    rint->SetPrompt(p.c_str());    

    this->SignalConfig();
    this->InterpreterConfig();
    this->LoadIncludes();
    this->LoadClasses();
  }
}


void FairDbProcRootArgs::InterpreterConfig()
{

  if (gInterpreter) {
    gInterpreter->SaveContext();
    gInterpreter->SaveGlobalsContext();
  }
}

void FairDbProcRootArgs::SignalConfig() 
{

  if (gSystem) { 
    gSystem->ResetSignal(kSigBus,                  kTRUE);
    gSystem->ResetSignal(kSigSegmentationViolation,kTRUE);
    gSystem->ResetSignal(kSigSystem,               kTRUE);
    gSystem->ResetSignal(kSigPipe,                 kTRUE);
    gSystem->ResetSignal(kSigIllegalInstruction,   kTRUE);
    gSystem->ResetSignal(kSigQuit,                 kTRUE);
    if (fHaveBatchOpt)
      gSystem->ResetSignal(kSigInterrupt,          kTRUE);
    gSystem->ResetSignal(kSigWindowChanged,        kTRUE);
    gSystem->ResetSignal(kSigChild,                kTRUE);
    gSystem->ResetSignal(kSigUrgent,               kTRUE);
    gSystem->ResetSignal(kSigFloatingException,    kTRUE);
    gSystem->ResetSignal(kSigTermination,          kTRUE);
    gSystem->ResetSignal(kSigUser1,                kTRUE);
    gSystem->ResetSignal(kSigUser2,                kTRUE);
  }
}

void FairDbProcRootArgs::LoadIncludes()
{

  TApplication* app = gROOT->GetApplication();
  if (app) {
    app->ProcessLine("#include <iomanip>");
    app->ProcessLine("#include <string>");

    // Load Fairroot include files
    TString mp = gROOT->GetMacroPath();
    TString ip;
    const char* p;
    p = gSystem->Getenv("FAIRROOT_INSTALL");
    if (p) {
      mp += ":";
      mp += p;
      mp += ":";
      mp += p;
      mp += "/macros";
      ip += " -I";
      ip += p;
    }
    p = gSystem->Getenv("FAIRROOT_INSTALL");
    if (p) {
      mp += ":";
      mp += p;
      mp += "/macros";
      ip += " -I";
      ip += p;
    }
    
    gROOT->SetMacroPath(mp.Data());
    gSystem->SetIncludePath(ip);
    
    TString dip = ".include ";
    dip += gSystem->Getenv("FAIRROOT_INSTALL");
    gROOT->ProcessLine(dip.Data());

    dip = ".include ";
    dip += gSystem->Getenv("FAIRROOT_INSTALL");
    gROOT->ProcessLine(dip.Data());
    dip += "/include";
    gROOT->ProcessLine(dip.Data());
  }
}

void FairDbProcRootArgs::LoadClasses()
{

  if (gROOT) {
    gROOT->LoadClass("TGeometry",   "Graf3d");
    gROOT->LoadClass("TTree",       "Tree");
    gROOT->LoadClass("TMatrix",     "Matrix");
    gROOT->LoadClass("TMinuit",     "Minuit");
    gROOT->LoadClass("TPostScript", "Postscript");
    gROOT->LoadClass("TCanvas",     "Gpad");
    gROOT->LoadClass("THtml",       "Html");
  }
}

FairDbProcRootArgs::~FairDbProcRootArgs() 
{
}

int FairDbProcRootArgs::RunTheApp()
{
  TApplication* app = gROOT->GetApplication();
  if (app) {
    app->Run(kTRUE); // 
    return 1;
  }
  return 0;
}
