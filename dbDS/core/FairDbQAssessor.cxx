#include "FairDbQAssessor.h"
#include <fstream>
#include <memory>

#include "TSQLStatement.h"

#include "FairDb.h"
#include "FairDbStatement.h"
#include "FairDbConnectionPool.h"
#include "FairDbTableInterfaceStore.h"
#include "FairDbValRecordMap.h"
#include "FairDbLogService.h"

#include "FairDbADS.h"
#include "FairDbProcCmd.h"

ClassImp(FairDbQAssessor)


FairDbQAssessor::FairDbQAssessor(FairDbADS* ads) :
fDbTask(ads),
fStatement0(0),
fStatement1(0)
{
  
  DBLOG("FairDb", FairDbLog::kVerbose) << " FairDbQAssessor() ctor" << endl;
  
  fStatement0  =   FairDbTableInterfaceStore::Instance()  
    .GetConnectionPool().CreateStatement(0);
  
  fStatement1  =   FairDbTableInterfaceStore::Instance()  
    .GetConnectionPool().CreateStatement(1);
  
  if ( ! fStatement0 || ! fStatement1 ) {
    DBLOG("FairDb", FairDbLog::kInfo) 
      << "Cannot get FairDbStatement for cascade entries 0,1 "
      << "unable to validate FairDbDS." << endl;
  }
  else {
    if ( ! PrepareDb() ) {
      DBLOG("FairDb", FairDbLog::kInfo) << "Cannot run QA test: DB not prepared !" << endl;
    }
    else {
      if ( RunAllTests() ) {
        DBLOG("FairDb", FairDbLog::kInfo) << "All tests passed" << endl;
      }
      else {
        DBLOG("FairDb", FairDbLog::kInfo) << "Validation failed!" << endl;
      }
    }
  }   
}  

FairDbQAssessor::~FairDbQAssessor() {
  
  DBLOG("FairDb", FairDbLog::kVerbose) << " FairDbQAssessor() dtor " << endl;
  
  delete fStatement0;
  fStatement0 = 0;
  delete fStatement1;
  fStatement0 = 0;  
}

Bool_t FairDbQAssessor::PrepareDb() {
  
  Bool_t ok = kTRUE;
  
  if ( ! fStatement0 || ! fStatement1 ) {
    DBLOG("FairDbDS",FairDbLog::kInfo) 
      << "Cannot prepare  for tests (no database access)" << endl;
    return kFALSE;
  }
  
  string prepareDBFileName 
    = fFileLocater.Find("PrepareDB.mysql");
  ifstream prepareDBFile(prepareDBFileName.c_str());
  
  if ( ! prepareDBFile.is_open() ) {
    DBLOG("FairDbDS",FairDbLog::kInfo)
      << "Cannot prepare  for tests (no validation data file)" << endl;
    return kFALSE;
  }
  
  // Loop processing SQL commands.
  
  while ( ! prepareDBFile.eof() ) {
    string sql;
    string line;
    while ( ! prepareDBFile.eof() ) {
      getline(prepareDBFile,line);
      // Remove trailing spaces.
      int last = line.size()-1;
      while ( last >= 0 && line[last] == ' ') line.erase(last--);
      // Ignore blank and comment lines
      if ( last < 0 || line[0] == '#' ) continue;
      // Append and look for terminating ";".
      sql += line;
      if ( line[last] == ';' ) {
	DBLOG("FairDbDS",FairDbLog::kDebug) << "SQL " << sql << endl;
        fStatement0->ExecuteUpdate(sql.c_str());
        if ( fStatement0->PrintExceptions() ) ok = kFALSE;
        fStatement1->ExecuteUpdate(sql.c_str());
        if ( fStatement1->PrintExceptions() ) ok = kFALSE;
        sql = "";
        break;
      }
    }
    if ( sql != "" ) {
      DBLOG("FairDbDS",FairDbLog::kInfo)
        << "File ends with incomplete SQL "<< sql << endl;
    }
  }
  
  return ok;  
}

Bool_t FairDbQAssessor::RunAllTests() {
  
  Bool_t ok = kTRUE;
  
  if ( ! TestGlobaliseSeqNo() ) ok = kFALSE;
  if ( ! TestExportImport()   ) ok = kFALSE;
  
  return ok;
  
}

Bool_t FairDbQAssessor::TestExportImport() {
  //  DB: extract data from database nb# 0  and import into database nb# 1.
  
  DBLOG("FairDbDS",FairDbLog::kInfo) << "Testing Export/Import       ..." << endl;
  
  Bool_t ok = kTRUE;
  
  if ( ! ok ) {
    DBLOG("FairDbDS",FairDbLog::kError) << "  Cannot prepare test" << endl;
    return kFALSE;
  }
  
  //  Extract data
  FairDbProcCmd pExport("/Export --Table DBMVALIDATEDATA1 --File FairDbQAssessorData1.dat");
  fDbTask->HandleCommand(&pExport);
  //  Force to load ValidityRecs from DB entry  nb# 1.
  fDbTask->ClearCache();
  fDbTask->fValRecSet = new FairDbValRecordMap("DBMVALIDATEDATA1",1);
  
  //  Import data
  FairDbProcCmd pImport("/Import --DatabaseNumber 1 --File FairDbQAssessorData1.dat");
  fDbTask->HandleCommand(&pImport);
  
  return ok;  
}

Bool_t FairDbQAssessor::TestGlobaliseSeqNo() {
  
  DBLOG("FairDbDS",FairDbLog::kInfo) << "Testing SeqNo globalisation ..." << endl;
  
  string sqlLocalMain =
    "select SeqNo from DBMVALIDATEDATA1 where SeqNo < 100000000;";
  string sqlLocalVal =
    "select SeqNo from DBMVALIDATEDATA1VLD where SeqNo < 100000000;";
  
  TSQLStatement* stmt = 0;
  
  //  Count number before
  Int_t numBefore = 0;
  stmt = fStatement0->ExecuteQuery(sqlLocalMain.c_str());
  fStatement0->PrintExceptions();
  if ( stmt ) while ( stmt->NextResultRow() ) ++numBefore;
  delete stmt;
  stmt = fStatement0->ExecuteQuery(sqlLocalVal.c_str());
  fStatement0->PrintExceptions();
  if ( stmt ) while ( stmt->NextResultRow() ) ++numBefore;
  delete stmt;
  
  //  Globalise
  FairDbProcCmd pc("/GlobaliseSeqNo DBMVALIDATEDATA1");
  fDbTask->HandleCommand(&pc);
  
  //  Count number after
  Int_t numAfter = 0;
  stmt = fStatement0->ExecuteQuery(sqlLocalMain.c_str());
  fStatement0->PrintExceptions();
  if ( stmt ) while ( stmt->NextResultRow() ) ++numAfter;
  delete stmt;
  stmt = fStatement0->ExecuteQuery(sqlLocalVal.c_str());
  fStatement0->PrintExceptions();
  if ( stmt ) while ( stmt->NextResultRow() ) ++numAfter;
  delete stmt;
  
  Bool_t ok = numBefore == 9 &&  numAfter == 0;
  DBLOG("FairDbDS",FairDbLog::kInfo) 
    << "    Before there should be 9 local Seqno, found " 
    << numBefore << endl  
    << "    After there should be 0 local Seqno, found " 
    << numAfter << endl
    << "Test has " << (ok ? "passed" : "failed!!") << endl;
  
  return ok;

}
