#include "FairDbCmdOpts.h"
#include <cstring>
#include <cstdlib>
#include <cstring>

#include "FairDbProcCmd.h" 
#include "FairDbString.h"
#include "FairDbLogService.h"

ClassImp(FairDbCmdOpts)

FairDbCmdOpts::FairDbCmdOpts(const string& validOpts,
                             FairDbProcCmd* pcmd,
                             const list<string>* defaultOpts) :
fIsValid(kTRUE),
fValidOpts(":")
{
  //   ValidOpts:     Format Opts list of colon separated valid options 
  //   *pcmd:         FairDbProcCmd to process. Optional, default = nullptr.
  //   *defaultOpts:  Ordered list of default options.
  
  DBLOG("FairDbDS", FairDbLog::kVerbose) << "FairDbCmdOpts ctor" << endl;
  
  // (+) leading and trailing colons for optimzing lookup.
  fValidOpts += validOpts;
  fValidOpts += ':';
  // Call then the ProcessCmd
  if ( pcmd ) ProcessCmd( *pcmd, defaultOpts );
}

FairDbCmdOpts::~FairDbCmdOpts() {
  DBLOG("FairDbDS", FairDbLog::kVerbose) << "FairDbCmdOpts dtor" << endl;
}

Bool_t FairDbCmdOpts::AddOpt(const string& opt, FairDbProcCmd& pcmd) {
  //
  //  Add  a (valid) option + corresponding value
  //  (option has to be validated)
  //
 
  string val("yes");
  Bool_t isOpt = kFALSE;

  if ( opt == "--Date"           ) isOpt = kTRUE;
  if ( opt == "--DatabaseNumber" ) isOpt = kTRUE;
  if ( opt == "--Detector"       ) isOpt = kTRUE;
  if ( opt == "--File"           ) isOpt = kTRUE;
  if ( opt == "--LogFile"        ) isOpt = kTRUE;
  if ( opt == "--NumSeqNo"       ) isOpt = kTRUE;
  if ( opt == "--Reason"         ) isOpt = kTRUE;
  if ( opt == "--SimFlag"        ) isOpt = kTRUE;
  if ( opt == "--SeqNo"          ) isOpt = kTRUE;
  if ( opt == "--SeqNoMin"       ) isOpt = kTRUE;
  if ( opt == "--SeqNoMax"       ) isOpt = kTRUE;
  if ( opt == "--Since"          ) isOpt = kTRUE;
  if ( opt == "--Table"          ) isOpt = kTRUE;
  if ( opt == "--Task"           ) isOpt = kTRUE;
  if ( opt == "--UpdateTime"     ) isOpt = kTRUE;

  if ( isOpt ) {
    const char* valPtr = pcmd.PopOpt();
    if ( ! valPtr) {
      DBLOG("FairDbDS",FairDbLog::kError) << "Option " << opt 
			     << " should have a corresponding value" << endl;
      fIsValid = kFALSE;
      return kFALSE;
    }
  else val = valPtr;
  }
  return AddOpt(opt,val);
}

Bool_t FairDbCmdOpts::AddOpt(const string& opt, const string& val) {
  //
  // If validated store the options
  //
  
  if ( ! IsValid( opt ) ) {
    DBLOG("FairDbDS",FairDbLog::kError) << "Option " << opt 
 		           << " is not valid for this command" << endl;
    fIsValid = kFALSE;
    return kFALSE;
  }
  if ( TestOpt(opt) )
    {
      DBLOG("FairDbDS",FairDbLog::kWarning) << "Option " << opt 
 		                << " has been duplicated" << endl;
    }
  fOptsMap[opt] = val;
  return fIsValid;
}

void FairDbCmdOpts::GetOptIntList(const string& opt, list<int>& nos) const {
  //
  //  GetOptIntList()  Parse and gives back a comma separated list of valid options
  //
  
  string optValue = this->GetOptString(opt);
  bool fail = (opt == "no");
  
  if ( ! fail ) {
    vector<string> ls;
    FairUtilString::StringTok(ls,optValue,",");
    if ( ls.size() == 0 ) fail = true;
    else {
      vector<string>::iterator    itr = ls.begin();
      vector<string>::iterator itrEnd = ls.end();
      while ( itr != itrEnd ) {
        if ( FairUtilString::IsInt(itr->c_str()) ) nos.push_back(atoi(itr->c_str()));
        else fail = true;
        ++itr;
      }
    }
  } // ! (fail)
  if ( fail ) nos.clear();
}

Int_t FairDbCmdOpts::GetOptInt(const string& opt) const {
  //
  //  GetOptInt() gives back corresponding option value as an integer
  //
  
  string value = this->GetOptString(opt);
  if ( opt == "no"  ) return 0;
  if ( opt == "yes" ) return 1;
  return atoi(value.c_str());
}

string FairDbCmdOpts::GetOptString(const string& opt) const {
  //
  //  GetOptString()  Gives back corresponding option value as a string.
  //
  
  FairDbCmdOpts* me = const_cast<FairDbCmdOpts*>(this);
  return TestOpt(opt) ? me->fOptsMap[opt] : "no";
}

string FairDbCmdOpts::GetOpts() const {
  //
  //  GetOpts():  Return all options as one string
  //
  
  string opts;
  for ( map<string,string>::const_iterator itr = fOptsMap.begin();
        itr != fOptsMap.end();
        ++itr ) {
    opts += (*itr).first + " " + (*itr).second + " ";
  }
  return opts;
}

Bool_t FairDbCmdOpts::IsValid(const string& opt) const {
  //
  //  IsValid():  Test option for validity
  //
  
  string optStr(":");
  optStr += opt;
  optStr += ':';
  return  fValidOpts.find(optStr) != string::npos; 
}

Bool_t FairDbCmdOpts::ProcessCmd(FairDbProcCmd& jcmd,
                                 const list<string>* defaultOpts) {
  //
  //  FairDbProcCmd is processed  and  stores all options.
  //
  
  list<string> defDefOPts;
  defDefOPts.push_back("--Table");
  defDefOPts.push_back("--File");

  if ( defaultOpts == 0 ) defaultOpts = &defDefOPts;
  
  list<string>::const_iterator 
          defOptsItr    = defaultOpts->begin();
  list<string>::const_iterator 
          defOptsItrEnd = defaultOpts->end();

  const char* opt = jcmd.PopOpt();

  // Loop over options
  while (opt) {
    // Options.  part
    if ( opt[0] == '-' ) {
      string option = opt;
      AddOpt(opt, jcmd);
    }   
    // Param. part
    else {
      string defaultOpt = "";
     if ( defOptsItr != defOptsItrEnd ) {
       defaultOpt = *defOptsItr;
       ++defOptsItr;
     }
     if (  defaultOpt != "" && ! TestOpt(defaultOpt) && IsValid(defaultOpt) 
           ) AddOpt(defaultOpt, opt);
     
     else {
       DBLOG("FairDbDS",FairDbLog::kWarning) << "Unknown parameter " << opt << endl;
       fIsValid = kFALSE;
     }
    }
    opt = jcmd.PopOpt(); 
  }   // while(options)
  
  return fIsValid;
}

Bool_t FairDbCmdOpts::TestOpt(const string& opt) const {
  //
  //  TestOpt():  Test an existing option.
  map<string,string>::const_iterator loc = fOptsMap.find(opt);
  return loc != fOptsMap.end();
}

