#include <cstdlib> 

#include "FairDbProcCmd.h"
#include "FairDbLogService.h"
#include "FairRegistry.h"

ostream& operator<<(ostream& os, const FairDbProcCmd& jc) 
{
  vector<string>::const_iterator itrend(jc.fCmdList.end());
  vector<string>::const_iterator itr;
  bool ifirst = true;
  for (itr = jc.fCmdList.begin(); itr!= itrend; ++itr) {
    if (ifirst) { os << (*itr); ifirst = false; }
    else os << "/" << (*itr);
  }
  itrend = jc.fOptList.end();
  for (itr = jc.fOptList.begin(); itr!= itrend; ++itr) {
    os << " " << (*itr);
  }
  return os;
}

FairDbProcCmd::FairDbProcCmd() 
{
  fItrCmdList = fCmdList.end();
  fItrOptList = fOptList.end();
}

FairDbProcCmd::FairDbProcCmd(const char *cmd) 
{
  this->Parse(cmd);
}

void FairDbProcCmd::Parse(const char *cmd) 
{
//
// Parse a command line
//
  int len;      
  bool procCmd;  
  bool procOpt;  
  bool inQuotes; 
  bool expectSpecial; 
  string word;   
   
  word          = "";
  len           = strlen(cmd);
  procCmd       = true;
  procOpt       = false;
  inQuotes      = false;
  expectSpecial = false;
 
  for (int i=0; i<len; ++i) {
    if (expectSpecial) {
      word += cmd[i];
      expectSpecial = false;
      continue; 
    }
    
    if (cmd[i] == '\\') {
      expectSpecial = true;
      continue; 
    }
    
    if (cmd[i] == '"' || cmd[i] == '\'') {
      inQuotes = !inQuotes;
      continue; 
    }
    if (inQuotes) {
      word += cmd[i];
      continue; 
    }
    
    if (i>0 && cmd[i]==' ' && cmd[i-1]==' ') {
      continue; 
    }
    
    if (procCmd && cmd[i] == '/') {
      if (word.length()>0) fCmdList.push_back(word);
      word = "";
      continue; 
    }
    
    if (cmd[i] == ' ' || cmd[i] == '=') {
      if (procCmd) {
	if (word.length()>0) fCmdList.push_back(word);
	word = "";
	procCmd = false;
	procOpt = true;
      }
      else if (procOpt) {
	if (word.length()>0) fOptList.push_back(word);
	word = "";
      }
      continue; 
    }
    word += cmd[i];
  }// ! ( loop over char ) 
  

  if (procCmd) {
     if (word.length()) fCmdList.push_back(word);
  }
  if (procOpt) {
    if (word.length()) fOptList.push_back(word);
  }

  fItrCmdList = fCmdList.begin();
  fItrOptList = fOptList.begin();
}

FairDbProcCmd::~FairDbProcCmd() {}

bool FairDbProcCmd::HaveCmd() const
{
  if (fItrCmdList == fCmdList.end()) return false;
  return true;
}

bool FairDbProcCmd::HaveOpt() const
{
  if (fItrOptList == fOptList.end()) return false;
  return true;
}

const char *FairDbProcCmd::PopCmd()
{

  if (this->HaveCmd()) {
    const char *val = 0;
    val = (*fItrCmdList).c_str();
    ++fItrCmdList;
    return val;
  }
  return 0;
}

const char *FairDbProcCmd::PushCmd()
{
  if (fItrCmdList != fCmdList.begin()) {
    const char *val = 0;
    --fItrCmdList;
    val = (*fItrCmdList).c_str();
    return val;
  }
  return 0;
}

const char *FairDbProcCmd::PushOpt()
{
  if (fItrOptList != fOptList.begin()) {
    const char *val = 0;
    --fItrOptList;
    val = (*fItrOptList).c_str();
    return val;
  }
  return 0;
}

const char *FairDbProcCmd::PopOpt()
{
  if (this->HaveOpt()) {
    const char *val = 0;
    val = (*fItrOptList).c_str();
    ++fItrOptList;
    return val;
  }
  return 0;
}
int FairDbProcCmd::PopIntOpt()
{
  if (this->HaveOpt()) {
    int val;
    val = atoi((*fItrOptList).c_str());
    ++fItrOptList;
    return val;
  }
  return 0;
}

double FairDbProcCmd::PopFloatOpt()
{
  if (this->HaveOpt()) {
    double val = 0.0;
    val = atof((*fItrOptList).c_str());
    ++fItrOptList;
    return val;
  }
  return 0.0;
}

void FairDbProcCmd::SplitLine(const char *line, char sep, string& a, string& b)
{
  a = "";
  b = "";
  bool doFront = true;
  for (const char* c = line; *c != '\0'; ++c) {
    if (*c == ' ') continue;
    if (*c == sep) {
      doFront = false;
      continue;
    }
    if (doFront) a += (*c);
    else         b += (*c);
  }

  DBLOG("FairDbDS",FairDbLog::kVerbose) << 
    "Splitting line: " << line << " into words: " << a << " :  " << b << ".\n";
}


void FairDbProcCmd::StringTok(std::vector<std::string>& ls,
			   const std::string& str,
			   const std::string& tok)
{
  const string::size_type S     = str.size();
  const string::size_type toksz = tok.size();
  string::size_type  i = 0;
  
  while (i < S) {
    while ( (i<S) && (tok.find(str[i])<=toksz) ) {
      ++i;
    }
    if (i == S)  return; 
    string::size_type  j = i+1;
    while ( (j<S) && !(tok.find(str[j])<=toksz) ) {
      ++j;
    }

    ls.push_back(str.substr(i,j-i));
    
    i = j+1;
  }  
}


bool FairDbProcCmd::IsInt(const char* s) 
{
  char* endptr;
  strtol(s, &endptr,0);
  if (endptr != (s + strlen(s))) return false;
  return true;
}


bool FairDbProcCmd::IsFloat(const char* s)
{
  char* endptr;
  strtod(s, &endptr);
  if (endptr != (s + strlen(s))) return false;
  return true;
}


void FairDbProcCmd::StringToRegistry(FairRegistry& r, const char* s) 
{
  // Registrx storage
  string cmd("Set "); 
  cmd += s;           
  FairDbProcCmd c(cmd.c_str());

  r.UnLockValues();
  while (c.HaveOpt()) {
    const char* key;
    const char* value;

    key = c.PopOpt();
    if (c.HaveOpt() == false) {
      DBLOG("FairDbDS",FairDbLog::kWarning) << "Value is missing for key \"" 
                                << key << "\"!" << endl;
      return;
    }
    value = c.PopOpt();
    
     if (FairDbProcCmd::IsInt(value)) {
       int i = atoi(value);
      r.Set(key, i);
    }
    else if (FairDbProcCmd::IsFloat(value)) {
       double d = atof(value);
      r.Set(key, d);
    }
    else if (FairDbProcCmd::IsBool(value)) {
      bool b = FairDbProcCmd::atob(value);
      r.Set(key, b); 
    }
    else {
      r.Set(key, value);
    }
  }

  r.LockValues();
}

bool FairDbProcCmd::IsBool(const char* value)
{
  std::string v(value);
  if (v == "true")   return true; 
  if (v == "false")  return true;
  if (v == "kTRUE")  return true; 
  if (v == "kFALSE") return true;
  if (v == "TRUE")   return true; 
  if (v == "FALSE")  return true;
  if (v == "True")   return true;
  if (v == "False")  return true;
  if (v == "on")     return true;
  if (v == "off")    return true;
  if (v == "On")     return true;
  if (v == "Off")    return true;
  if (v == "ON")     return true;
  if (v == "OFF")    return true;
  return false;
}

bool FairDbProcCmd::atob(const char* value)
{
  std::string v(value);
  if (v == "true")   return true;  
  if (v == "false")  return false;
  if (v == "kTRUE")  return true;  
  if (v == "kFALSE") return false;
  if (v == "TRUE")   return true;  
  if (v == "FALSE")  return false;
  if (v == "True")   return true;
  if (v == "False")  return false;
  if (v == "on")     return true;
  if (v == "off")    return false;
  if (v == "On")     return true;
  if (v == "Off")    return false;
  if (v == "ON")     return true;
  if (v == "OFF")    return false;

  DBLOG("FairDbDs",FairDbLog::kWarning) << 
    " Trying to convert a string '" << value << "' to a boolean. Result will be taken as 'false'\n";
  return false;
}

