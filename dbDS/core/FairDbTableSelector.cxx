#include "FairDbTableSelector.h"
#include "FairDbLogService.h"
#include "FairDbString.h"

#include <ctype.h>
#include <fstream>

ClassImp(FairDbTableSelector)

FairDbTableSelector::FairDbTableSelector(const std::string& nameList)
{

  DBLOG("FairDbDS", FairDbLog::kVerbose) << "Creating FairDbTableSelector" << endl;

  bool ok = true;

  // Split list of names 
  std::vector<std::string> names;
  FairUtilString::StringTok(names, nameList,",");
  
  std::vector<std::string>::iterator itr    = names.begin();
  std::vector<std::string>::iterator itrEnd = names.end();
  for (; itr != itrEnd; ++itr) {
    std::string& name = *itr;
    if ( name.size() == 0 ) {
      ok = false;
       DBLOG("FairDbDS", FairDbLog::kError) << "List contains an empty element" 
                                            << endl;
       continue;
    }
    // ---
    bool type = true;
    if ( name[0] == '!' ) {
       type = false;
       name.erase(0,1);
    }
    // ---
    int loc    = 0;
    int locMax =  name.size() -1;
    while (    loc <= locMax 
               && ( isalnum(name[loc]) ||  name[loc] == '_' )
               ) ++loc;
    if ( loc < locMax || ( loc == locMax & name[loc] != '*' ) ) {
      ok = false;
      DBLOG("FairDbDS", FairDbLog::kError) << "List contains an illegal element: " 
                                           << name << endl;
      continue;
    }
    
    // Recording 
    fNameList[name] = type;
  }
  
  // Clearing 
   if (! ok ) fNameList.clear();
}

FairDbTableSelector::~FairDbTableSelector() {
  DBLOG("FairDbDS", FairDbLog::kVerbose) << "~FairDbTableSelector()" << endl;
}

Int_t FairDbTableSelector::BestMatch(const std::string& name,
                                Bool_t type) const {

  typedef std::map<std::string,
                   Bool_t>::const_reverse_iterator itr_t;
  itr_t itr    = fNameList.rbegin(); 
  itr_t itrEnd = fNameList.rend();

  for (; itr != itrEnd; ++itr)
    if (     itr->second == type
         && FairUtilString::cmp_wildcard(name,itr->first) == 0
       ) return itr->first.size();
  return 0;

}

Bool_t FairDbTableSelector::Test(const std::string& name) const {
  return this->BestMatch(name,true) > this->BestMatch(name,false);
}
