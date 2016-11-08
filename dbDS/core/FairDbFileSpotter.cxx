#include "FairDbFileSpotter.h"
#include "FairDbLogService.h"

#include <fstream>
#include "TSystem.h"

using namespace std;

ClassImp(FairDbFileSpotter)

FairDbFileSpotter::FairDbFileSpotter()
{

  DBLOG("FairDbDS", FairDbLog::kVerbose) << "FairDbFileSpotter ctor called" << endl;
  
// Put current directory first in list.
  fDirs.push_back(".");

// Look up SRT private and public names and add them if defined.
  const char* str = gSystem->Getenv("SRT_PRIVATE_CONTEXT");
  if ( str ) fDirs.push_back(str);
  str = gSystem->Getenv("SRT_PUBLIC_CONTEXT");
  if ( str ) fDirs.push_back(str);

}


FairDbFileSpotter::~FairDbFileSpotter() {

  DBLOG("FairDbDS", FairDbLog::kVerbose) << "FairDbFileSpotter dtor called" << endl;
}

string FairDbFileSpotter::Find(const string& fileName) const {

  for (list<string>::const_iterator itr = fDirs.begin();
       itr != fDirs.end();
       ++itr) {
    string fName =  *itr + "/" + fileName;
    ifstream test(fName.c_str());
    if ( test.is_open() ) return fName;
  }

  DBLOG("FairDbDS",FairDbLog::kInfo) << "file " << fileName << " not found !..." <<  endl;
  
  return "";
}

