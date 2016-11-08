#ifndef FAIRDBADS_H
#define FAIRDBADS_H

#include <iostream>
#include <fstream>
#include <list>
#include <string>

using namespace std;

#include "FairDbFileSpotter.h"
#include "FairDbProc.h"

class FairDbConnectionPool;
class FairDbProxy;
class FairDbValRecordMap;
class FairDbLogFile;
class FairDbTableSelector;

class FairDbADS : public FairDbProc 
{
 public:
  
  friend class FairDbQAssessor;  
  
  FairDbADS();
  virtual ~FairDbADS();
  
  // Interactive mode methods
  virtual void HandleCommand(FairDbProcCmd *command);
  virtual void Help();
  
 protected:
  Bool_t ApplySQLFixup(UInt_t dbNo,
                       std::ifstream& in, 
                       FairDbLogFile& logFile,
                       Bool_t enabled);
  Bool_t BuildValRecSet(const std::string& tableName,UInt_t dbNo);
  Bool_t CheckMemory(FairDbProcCmd *cmd=0);
  void   ClearCache();
  Bool_t ExportTables(FairDbProcCmd *cmd);
  Bool_t ExportTable(const std::string& tableName,
                     std::ofstream* out,
                     FairDbLogFile& logFile,
                     std::list<int>seqnos);
  const FairDbProxy& GetDBProxy(const std::string& tableName) const;
  Bool_t GlobaliseSeqNo(FairDbProcCmd *cmd);
  Bool_t GlobaliseSeqNo(const std::string& tableName);
  Bool_t Import(FairDbProcCmd *cmd);
  Bool_t LogEntry(FairDbProcCmd *cmd);
  void   ListTables(const FairDbTableSelector& nameFilter,
                    UInt_t dbNo,
                    std::list<string>& tableNames) const;
  Bool_t Query(FairDbProcCmd *cmd);
  void   Quit() const;
  void   SetLogLevel(FairDbProcCmd *cmd) const;
  void   SetTimer(FairDbProcCmd *cmd) const;
  
  FairDbADS& operator=(const FairDbADS&);
  
  FairDbConnectionPool& fConnections;    // only one connection pool
  FairDbValRecordMap*      fValRecSet;   // Cached copy of FairDb Record Map
  FairDbFileSpotter     fFileLocater; //File spotter utility.
  
  ClassDef(FairDbADS,0) // ADS Process
};

#endif // FAIRDBADS
