#ifndef FAIRDBTABLESELECTOR_H
#define FAIRDBTABLESELECTOR_H

#include <map>
#include <string>

#include "Rtypes.h"

class FairDbTableSelector {

 public:
  FairDbTableSelector(const std::string& nameList = "");
  virtual ~FairDbTableSelector();
  
  // State testing member functions
  Bool_t IsEmpty() const { return fNameList.size() > 0; }
  Bool_t Test(const std::string& name) const;
  
 private:
  Int_t BestMatch(const std::string& name,
                  Bool_t type) const;
  // Map of names -> types
  // <DB> try using const string as key won't work with XCode
  // on Mavericks so i keep it non const as dirty work arround
  // for now
  
  typedef std::map<std::string, Bool_t> NameType;
  NameType fNameList;
  
  ClassDef(FairDbTableSelector,0)   // Table Name Selector
};

#endif // !(FAIRDBTABLESELECTOR)
