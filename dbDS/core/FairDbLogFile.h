#ifndef FAIRDBLOGFILE_H
#define FAIRDBLOGFILE_H

#include <fstream>
#include <list>
#include <map>
#include <string>
#include "Rtypes.h"

using namespace std;

class FairDbValRecord;
class FairDbCmdOpts;

//
// DB Archive Log File 
//

class FairDbLogFile 
{
 public:
  
  FairDbLogFile(const string& fileName = "",UInt_t dbNo = 0);
  virtual ~FairDbLogFile();
  Bool_t IsValid() const { return fLogFile && fLogFile->is_open(); }
  ofstream* GetStream() { return IsValid() ? fLogFile : 0; }
  void LogCmd(const string& cmd,const FairDbCmdOpts& opts); 
  void LogMsg(const string& msg,Bool_t addDateStamp=kTRUE); 
  void LogRec(const FairDbValRecord* vrec =0); 
  void LogRec(const string tableName, UInt_t seqNo); 
  
 private:
  UInt_t fDbNo;
  std::list<UInt_t> fLogDBSrcNos; 
  std::map<UInt_t,UInt_t>  fLogNumSeqNo;    
  std::map<UInt_t,UInt_t>  fLogSeqNoMax;    
  std::map<UInt_t,UInt_t>  fLogSeqNoMin;   
  string fLogTableName;  
  // Corresponding Log File
  ofstream* fLogFile;
  
  ClassDef(FairDbLogFile, 0) 
};//! FairDbLogFile 

#endif // FAIRDBLOGFILE
