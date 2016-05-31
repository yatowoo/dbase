#ifndef FAIRDBLOGFILE_H
#define FAIRDBLOGFILE_H


#include <memory>
#include <string>
#include <string.h>

#include <TObject.h>

#include <map>
#include <vector>

//namespace db_admin {
  
class FairDbLogFile : public TObject
{
 public:
  FairDbLogFile();
  virtual ~FairDbLogFile();
  
  void dump();
  
 protected:
   std::map<int,double> amap;
  
   ClassDef(FairDbLogFile, 1) 
};//! FairDbLogFile 

  
//}// ! namespace db_admin

#endif // FAIRDBLOGFILE
