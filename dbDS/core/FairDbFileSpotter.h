#ifndef FAIRDBFILESPOTTER_H
#define FAIRDBFILESPOTTER_H


#include <list>
#include <string>

#include "Rtypes.h"

 

class FairDbFileSpotter {
 public:
  FairDbFileSpotter();
  virtual ~FairDbFileSpotter();
  
  std::string Find( const std::string& fname) const;
  std::list<std::string> fDirs;
  
  ClassDef(FairDbFileSpotter,0) 
};

#endif // !(FAIRDBFILESPOTTER)
