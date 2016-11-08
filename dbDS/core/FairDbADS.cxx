#include "FairDbADS.h"

#include <algorithm>
#include <fstream>
#include <memory>
using std::auto_ptr;
#include <sstream>

#include "db_detector_def.h"
#include "DataType.h"

#include "FairDb.h"
#include "FairDbExceptionLog.h"
#include "FairDbProxy.h"
#include "FairDbConnectionPool.h"
#include "FairDbConfigData.h"
#include "FairDbResult.h"
#include "FairDbReader.h"
#include "FairDbSqlValidityData.h"
#include "FairDbStatement.h"
#include "FairDbStopWatchManager.h"
#include "FairDbValRecord.h"
#include "FairDbValRecordMap.h"
#include "FairDbTableInterface.h"
#include "FairDbTableInterfaceStore.h"
#include "FairDbWriter.h"

#include "FairDbCmdOpts.h"
#include "FairDbLogFile.h"
#include "FairDbTableSelector.h"

#include "FairDbProc.h"
#include "FairDbProcRegistry.h" 
#include "FairDbProcCmd.h" 

#include "FairDbLogService.h"     

#include "TSQLStatement.h"

#include "FairDbLogUtil.h"
#include "ValCondition.h"
#include "ValTimeStamp.h"

using namespace std;

ClassImp(FairDbADS)
                                                                          

FAIRDBPROCREG(FairDbADS,"FairDbADS","FairDb Admin");

FairDbADS::FairDbADS() :
fConnections(FairDbTableInterfaceStore::Instance().GetConnectionPool()),
fValRecSet(0)
{
    //  FairDbLogService::Instance()->GetStream("FairDb")->SetLogLevel(FairDbLog::kVerbose);
    //  FairDbLogService::Instance()->GetStream("FairDbDS")->SetLogLevel(FairDbLog::kDebug);
  DBLOG("FairDbDS", FairDbLog::kVerbose) << "Creating FairDbADS" << endl;
  
}


FairDbADS::~FairDbADS() {

  DBLOG("FairDbDS", FairDbLog::kVerbose) << "Destroying FairDbADS" << endl;

  ClearCache();

}

Bool_t FairDbADS::ApplySQLFixup(UInt_t dbNo,
                                ifstream& in, 
                                FairDbLogFile& logFile,
                                Bool_t enabled) {

  auto_ptr<FairDbStatement> stmtDb(FairDbTableInterfaceStore::Instance()
                              .GetConnectionPool()
                              .CreateStatement(dbNo) );
  if ( ! stmtDb.get() ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) 
      << "Attempting to write to non-existant cascade entry " << dbNo
      << endl;
   return kFALSE;
  }


  while ( in.peek() == 'S' ) {
    string line;
    getline(in,line);
    if ( line.substr(0,4) != "SQL:" ) {
      DBLOG("FairDbDS",FairDbLog::kError) 
        << "SQL Fixup line does not start: SQL: " << line
        << endl;
     return kFALSE;
    }

    //Remove anything after last ';'
    string::size_type locSemiColon = line.rfind(';');
    if ( locSemiColon == string::npos )  {
      DBLOG("FairDbDS",FairDbLog::kError) 
        << "SQL Fixup line does not end with semicolon: " << line
        << endl;
     return kFALSE;
    }
    line.erase(locSemiColon+1);

    if ( enabled ) {
      //some hackery to make sure data gets deleted before VAL entry
      std::string::size_type p;
      if((line.find("delete from ")!=std::string::npos) &&
	 ((p=line.find("VAL"))!=std::string::npos)){
	std::string noVAL=line.substr(0,p);
	noVAL+=line.substr(p+3);
	stmtDb->ExecuteUpdate(noVAL.substr(4).c_str());
	logFile.LogMsg(line,kFALSE);
	if ( stmtDb->PrintExceptions() ) logFile.LogMsg("   - above command failed.",kFALSE);
      }
      stmtDb->ExecuteUpdate(line.substr(4).c_str());
      logFile.LogMsg(line,kFALSE);
      if ( stmtDb->PrintExceptions() ) logFile.LogMsg("   - above command failed.",kFALSE);
    }
    else {
      string msg("SQL fixup disabled. Ignoring :");
      msg += line;
      logFile.LogMsg(msg,kFALSE);
    }

  }
  return kTRUE;

}


Bool_t FairDbADS::BuildValRecSet(const string& tableName,UInt_t dbNo) {

  if (    fValRecSet 
       && (    fValRecSet->GetTableName() != (tableName+"VAL") 
            || fValRecSet->GetDbNo()      != dbNo ) ) {
    DBLOG("FairDbDS", FairDbLog::kDebug) << "(Dropping Table "
                           << fValRecSet->GetTableName() << ")" << endl;
    ClearCache();
  }

  if ( ! fValRecSet ) {
    DBLOG("FairDbDS", FairDbLog::kDebug) << "Reading Table " << tableName
			   << "..." << endl;
    
    fValRecSet = new FairDbValRecordMap(tableName,dbNo);
  }

  int numRows = fValRecSet->GetNumRows();
  if ( numRows ) {
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Table " << tableName
			   << " contains " << numRows
			   << " sequence numbers" << endl;
  }
  else {
    DBLOG("FairDbDS", FairDbLog::kDebug) << "Table " << tableName
			   << " missing or empty!" << endl;
  }

  return numRows > 0;

}

Bool_t FairDbADS::CheckMemory(FairDbProcCmd *cmd) {

  if ( cmd ) {
    FairDbCmdOpts opts("",cmd);
    if ( ! opts.IsValid() ) return kFALSE;
  }

  // Clear out local cache 
  ClearCache();

  // <DB> ? FixME ?
  FairDbTableInterfaceStore::Instance().ShowStatistics();

  return kTRUE;
}


void FairDbADS::ClearCache() {

  delete fValRecSet;
  fValRecSet = 0;

}


Bool_t FairDbADS::ExportTable(const string& tableName,
                              ofstream* out,
                              FairDbLogFile& logFile,
                              list<int>seqnos) {

  DBLOG("FairDbDS", FairDbLog::kInfo) << "\nExporting table " 
                         << tableName << "..." << endl;

  if ( ! BuildValRecSet(tableName,0) ) return kFALSE;

  int numVRecs    = fValRecSet->GetNumRows();
  int numVRecsOut = 0;
  Int_t numModified = 0;

  if ( numVRecs == 0 ) {
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Nothing to do: table " 
                           << tableName << " is empty!" << endl;
    return kTRUE; 
  }

  ValTimeStamp farFuture(2030,1,1,0,0,0);
// Now is actually 5 mins ago otherwise next Export will
// pick it up again.
  ValTimeStamp now(ValTimeStamp().GetSec() - 5*60, 0);
  
  const FairDbProxy& proxy = GetDBProxy(tableName);

  Int_t numDataRows = 0;
  Bool_t writeMeta  = kTRUE;

  for ( int jvrec = 0; jvrec < numVRecs; ++jvrec ) {

    // Create an inner scope to limit the lifetime of packet
    {

      // Skip if unwanted.
      const FairDbValRecord* vrec = fValRecSet->GetObjTableMap(jvrec);
      int seqNo   = vrec->GetSeqNo();
      bool isWanted = true;
      if ( seqnos.size() > 0 ) {
        isWanted = false;
        int seqNoMin = 999999999;
        list<int>::iterator  itr(seqnos.begin()), itrEnd(seqnos.end());
        for ( ; itr != itrEnd; ++itr ) {
          int seqNoMax = *itr;
          if ( seqNoMax < 0 ) {
            seqNoMax = - seqNoMax;
            if ( seqNo >= seqNoMin && seqNo <= seqNoMax ) isWanted = true;
	  }
	  else {
            if ( seqNo == seqNoMax ) isWanted = true; 
	  }
	  seqNoMin = seqNoMax;
	}
      }
      if ( ! isWanted ) continue;
      FairDbSqlValidityData packet(*vrec);

      // Deal with far future insert dates.
      if ( vrec->GetInsertDate() > farFuture ) { 
          vrec->GetTableInterface()
                 ->GetDBProxy()
  	        .ReplaceInsertDate(now,seqNo,vrec->GetDbNo());
      }

      // Renumber local SEQNOs.
      if ( seqNo <= FairDb::kMAXLOCALSEQNO ) {
        Int_t globalSeqNo = fConnections.AllocateSeqNo(tableName,1);
        if ( globalSeqNo < FairDb::kMAXLOCALSEQNO ) {
          DBLOG("FairDb",FairDbLog::kError) << "Failed (cascader returned " <<globalSeqNo
				 << ") to get replacement for " 
                                 << seqNo << endl;
          continue;
        }
  
        DBLOG("FairDb",FairDbLog::kInfo) << "    Renumbering " << seqNo 
                              <<  " -> " << globalSeqNo << endl;
        if ( ! proxy.ReplaceSeqNo(seqNo,globalSeqNo,0) ) continue;
        else ++numModified;
        seqNo = globalSeqNo;
        packet.SetSeqNo(seqNo);
      }

      Int_t  numRows = packet.GetNumSqlStmts()-1;

      DBLOG("FairDbDS", FairDbLog::kInfo)
        << "Exporting sequence number " << seqNo
        << " with " << numRows << " rows of data" << endl;

      packet.Write(*out,writeMeta);
      writeMeta = kFALSE;
      logFile.LogRec(vrec);
      numDataRows += numRows;
      ++numVRecsOut;

    }
  
    FairDbTableInterfaceStore::Instance().PurgeCaches();
  }

  // If any sequence number has been modified flush the cache.
  if ( numModified ) ClearCache();

  DBLOG("FairDbDS", FairDbLog::kInfo)
    << "A total of " << numVRecsOut << " validity entries and "
    << numDataRows << " data rows written to file" << endl;
  // Flush out last record.
  logFile.LogRec();


  return kTRUE;

}

Bool_t FairDbADS::ExportTables(FairDbProcCmd *cmd) {

  DBLOG("FairDbDS",FairDbLog::kInfo) << "\n\nProcessing EXPORT command...\n" << endl;

  FairDbCmdOpts opts("--File:--LogFile:--SeqNo:--Since:--Table", cmd);
  if ( ! opts.IsValid() ) return kFALSE;


  // --LogFile
  string logName;
  if ( opts.TestOpt("--LogFile") ) {
    logName = opts.GetOptString("--LogFile");
  }
  FairDbLogFile logFile(logName);

  // --SeqNo
  list<int> seqnos;
  if ( opts.TestOpt("--SeqNo" ) ) opts.GetOptIntList("--SeqNo",seqnos);

  // --Since
  string insertDateCondition;
  if ( opts.TestOpt("--Since" ) ) {
    string opt = opts.GetOptString("--Since");
    insertDateCondition =   insertDateCondition + "InsertDate >'" 
                            + opt + "'";
    DBLOG("FairDbDS",FairDbLog::kInfo) << "Applying condition: " 
                          << insertDateCondition  << endl;
  }

  // --File and --Table.
  if ( ! opts.TestOpt("--Table" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "Table names  not specified." << endl;
    return kFALSE;
  }
  if ( ! opts.TestOpt("--File" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "File name  not specified." << endl;
    return kFALSE;
  }
 
  FairDbTableSelector nameFilter(opts.GetOptString("--Table"));
  DBLOG("FairDbDS",FairDbLog::kInfo) << " Export table name mask set to: " 
                        << opts.GetOptString("--Table") <<  endl;
  string fileName = opts.GetOptString("--File");


  //  Create export file.
  ofstream exportFile(fileName.c_str(),ios_base::app);
  if ( ! exportFile ) {
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Unable to open " << fileName << endl;
    return kFALSE; 
  }
  
  
  list<string> tableNames;
  this->ListTables(nameFilter,0,tableNames);
  
  //
  if ( insertDateCondition != "" ) {
    ClearCache();
    FairDbTableInterfaceStore::Instance()
                           .SetSqlCondition(insertDateCondition);
  }

  // Process all tables.
  logFile.LogCmd("export",opts);

  for ( list<string>::const_iterator itr = tableNames.begin();
        itr != tableNames.end();
        ++itr ) {
    FairDbTableInterfaceStore::Instance()
                           .SetSqlCondition(insertDateCondition);
    ExportTable(*itr,&exportFile,logFile,seqnos);
    FairDbTableInterfaceStore::Instance().SetSqlCondition("");
  }

  // Undo  SQL conditions.
  if ( insertDateCondition != "" ) {
    ClearCache();
    FairDbTableInterfaceStore::Instance().SetSqlCondition();
  }

  // Close 
  exportFile.close();

  return kTRUE;

}

const FairDbProxy& FairDbADS::GetDBProxy(const string& tableName) const {

  FairDbConfigData pet;
  return  FairDbTableInterfaceStore::Instance()
            .GetTableInterface(tableName,&pet)
               .GetDBProxy(); 

}

Bool_t FairDbADS::GlobaliseSeqNo(FairDbProcCmd *cmd) {

  string tableName;

  FairDbCmdOpts opts("--Table", cmd);
  if ( ! opts.IsValid() ) return kFALSE;

  if ( ! opts.TestOpt("--Table" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "Table name  not specified." << endl;
    return kFALSE;
  }

  tableName = opts.GetOptString("--Table");

  return GlobaliseSeqNo(tableName);

}

Bool_t FairDbADS::GlobaliseSeqNo(const string& tableName) {

  Bool_t ok         = kTRUE;
  Int_t numModified = 0;

  // Collect up all Val. Records
  if ( ! BuildValRecSet(tableName,0) ) return kFALSE;

  // is there an authorising database.
  if ( fConnections.GetAuthorisingDbNo() < 0 ) {
    DBLOG("FairDb",FairDbLog::kError) << "No authorising DB in cascade" << endl;
    return kFALSE;
  }

  // Loop over all Val. Records checking for and converting local SeqNos
  const FairDbProxy& proxy = GetDBProxy(tableName);

  DBLOG("FairDbDS", FairDbLog::kInfo) << "Scanning for local sequence numbers..." << endl;
  int numRows = fValRecSet->GetNumRows();

  for ( int irow = 0; irow < numRows; ++ irow ) {
    const FairDbValRecord* vrec = fValRecSet->GetObjTableMap(irow);
    int seqNo = vrec->GetSeqNo();
    if ( seqNo <= FairDb::kMAXLOCALSEQNO ) {
      Int_t globalSeqNo = fConnections.AllocateSeqNo(tableName,1);
      if ( globalSeqNo < FairDb::kMAXLOCALSEQNO ) {
        DBLOG("FairDb",FairDbLog::kError) << "Failed to get replacement for " 
                               << seqNo << endl;
        ok = kFALSE;
        break;
      }

      DBLOG("FairDb",FairDbLog::kInfo) << "    Renumbering " << seqNo 
                            <<  " -> " << globalSeqNo << endl;
      if ( ! proxy.ReplaceSeqNo(seqNo,globalSeqNo,0) ) ok = kFALSE;
      else ++numModified;
      
    }
  }

  DBLOG("FairDb",FairDbLog::kInfo) << "A total of " << numRows
			  << " were scanned" << endl;

  // If any sequence number has been modified flush the cache.
  if ( numModified ) {
    DBLOG("FairDb",FairDbLog::kInfo) << "A total of " << numModified
			  << " were renumbered" << endl;
    ClearCache();
    BuildValRecSet(tableName,0);
  }
  else DBLOG("FairDb",FairDbLog::kInfo) << "No local SeqNos found" << endl;
  return ok;

}

void FairDbADS::HandleCommand(FairDbProcCmd *cmd) {

  this->ClearCache();

  const char* c = cmd->PopCmd();

  if (c) {
    TString sc(c);
    sc.ToLower();
    if      (sc == "checkmemory" )    CheckMemory(cmd);
    else if (sc == "export" )         ExportTables(cmd);
    else if (sc == "globaliseseqno" ) GlobaliseSeqNo(cmd);
    else if (sc == "import" )         Import(cmd);
    else if (sc == "logentry" )       LogEntry(cmd);
    else if (sc == "help" )           Help();
    else if (sc == "query" )          Query(cmd);
    else if (sc == "setloglevel" )    SetLogLevel(cmd);
    else if (sc == "settimer" )       SetTimer(cmd);

    else {
      DBLOG("FairDbDS",FairDbLog::kWarning) << "Unknown command " << sc <<endl
           << "Type Help for valid commands." << endl;

    }
  }

}

void FairDbADS::Help() {

  string helpFileName 
     = fFileLocater.Find("doc/Help.txt");
    ifstream helpFile(helpFileName.c_str());

  if ( ! helpFile.is_open() ) {
    DBLOG("FairDbDS",FairDbLog::kInfo) << "doc/help.txt file not found!" << endl;
  }
  else {
    string line;
    while ( ! helpFile.eof() ) {
      getline(helpFile,line);
      DBLOG("FairDbDS",FairDbLog::kInfo) << line << endl;
    }
  }


}

Bool_t FairDbADS::Import(FairDbProcCmd *cmd) {

  DBLOG("FairDbDS",FairDbLog::kInfo) << "\n\nProcessing IMPORT command...\n" << endl;

// Process all options.
  FairDbCmdOpts opts("--Batch:"
                     "--DiscardConflicts:"
                     "--DatabaseNumber:"
                     "--EnableSQL:"
                     "--File:"
                     "--LogFile:"
                     "--Table:"
                     "--Test",
                     cmd);
  if ( ! opts.IsValid() ) return kFALSE;

  //  --DatabaseNumber
  UInt_t dbNo = 0;
  if ( opts.TestOpt("--DatabaseNumber" ) ) {
    dbNo = opts.GetOptInt("--DatabaseNumber");
    DBLOG("FairDbDS",FairDbLog::kInfo) << " using database number " << dbNo
			  << "  " << opts.GetOptString("--DatabaseNumber")
 		        << " for import" <<  endl;
  }

  //  --Batch
   Bool_t batch = opts.TestOpt("--Batch");
   if ( batch ) DBLOG("FairDbDS",FairDbLog::kInfo) 
       << " Batch mode: Errors will force exit at end of command." <<  endl;

  //  --DiscardConflicts
   Bool_t discardConflicts = opts.TestOpt("--DiscardConflicts");
   if ( discardConflicts ) DBLOG("FairDbDS",FairDbLog::kInfo) 
       << " Conflicts will be discarded." <<  endl;
   else DBLOG("FairDbDS",FairDbLog::kInfo) 
       << " Conflicts will be imported." <<  endl;

  //  --EnableSQL
   Bool_t enableSQL = opts.TestOpt("--EnableSQL");
   if ( enableSQL ) DBLOG("FairDbDS",FairDbLog::kInfo) 
       << " SQL fixups are enabled." <<  endl;
   else DBLOG("FairDbDS",FairDbLog::kInfo) 
       << " SQL fixups are disabled." <<  endl;

  //   --File
  if ( ! opts.TestOpt("--File" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "File name  not specified." << endl;
    if ( batch ) this->Quit();
    return kFALSE;
  }
  string fileName = opts.GetOptString("--File");

  //  --LogFile but always create a logfile even
  //  if its dummy.  That way it can be put on the stack.
  string logName;
  if ( opts.TestOpt("--LogFile") ) {
    logName = opts.GetOptString("--LogFile");
  }
  FairDbLogFile logFile(logName,dbNo);


  //  --Table
  string tableMask = "*";
  if ( opts.TestOpt("--Table") ) tableMask = opts.GetOptString("--Table");
  FairDbTableSelector nameFilter(tableMask);
  DBLOG("FairDbDS",FairDbLog::kInfo) << " Import table name mask set to: " << tableMask <<  endl;

  //  --Test
  Bool_t test = opts.TestOpt("--Test");
  if ( test ) DBLOG("FairDbDS",FairDbLog::kInfo) << " Testing only, database will "
                                     << " not be updated." <<  endl;  

  //  Open import file.
  ifstream import(fileName.c_str());
  if ( ! import ) {
    DBLOG("FairDbDS", FairDbLog::kError) << "Unable to open " << fileName << endl;
    if ( batch ) this->Quit();
    return kFALSE; 
  }

  DBLOG("FairDbDS", FairDbLog::kInfo) << "\n\nProcessing file " << fileName 
                         << " ..." << endl;

  Int_t numNewTables = 0;
  Int_t numImported  = 0;
  Int_t numImpConfl  = 0;
  Int_t numImpUpdate = 0;
  Int_t numDuplicate = 0;
  Int_t numOutOfDate = 0;
  Int_t numConflict  = 0;
  Int_t numLocal     = 0;
  Int_t numFailed    = 0;
  Int_t numTest      = 0;

  string currentTableName;

  logFile.LogCmd("import",opts);

//  Apply any SQL fixups before starting the import loop.
  if ( ! this->ApplySQLFixup(dbNo, import, logFile, enableSQL) ) {
    DBLOG("FairDbDS", FairDbLog::kError) << "Fixup failed " << endl;
    if ( batch ) this->Quit();
    return kFALSE; 
  }

// Loop repeatedly filling a Sql Val. Recors and storing it.
  FairDbSqlValidityData packetFile(import);

  Bool_t skipTable = kFALSE;

  for (;  packetFile.CanBeStored(); packetFile.Fill(import) ) {

    //  Check for table change.
    if ( currentTableName != packetFile.GetTableName() ) {
      currentTableName = packetFile.GetTableName();

      // Skip table if not in name filter.
      skipTable =  ! nameFilter.Test(currentTableName);
      if ( skipTable ) {
        if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "\nSkipping table " 
			     << currentTableName << "...\n" << endl;
        continue;
      }
      if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "\nUpdating table " 
			     << currentTableName << "...\n" << endl;

      // If it doesn't exist, try to create it.
      auto_ptr<FairDbStatement> stmtDb(fConnections.CreateStatement(dbNo));
      string sql = "select * from  ";
      sql += currentTableName + " where 1=0;";
      TSQLStatement* res = stmtDb->ExecuteQuery(sql.c_str());

      if ( ! res ) {
        if ( packetFile.CreateTable(dbNo) ) {
          DBLOG("FairDbDS", FairDbLog::kInfo) << "   Created table: "
				 << currentTableName << endl;
	  ++numNewTables;
	}
	else {
          DBLOG("FairDbDS", FairDbLog::kError) << "   failed to create table: "
				 << currentTableName << endl;
	}
      }
    }

    // 
    if ( skipTable ) continue;

    Int_t seqNo = packetFile.GetSeqNo();

    // 
    bool tryImport = false;
    bool replace   = false;

    // 
    FairDbValRecordMap vrecSet(currentTableName,dbNo,seqNo);
    const FairDbValRecord* vrec = vrecSet.GetObjTableMapBySeqNo(seqNo);

    if ( vrec ) {

      FairDbSqlValidityData::CompResult_t comp = FairDbSqlValidityData::kIdentical;
      if ( packetFile.GetCreationDate() > vrec->GetCreationDate() 
         )  comp = FairDbSqlValidityData::kUpdate;
      if ( packetFile.GetCreationDate() < vrec->GetCreationDate() 
         )  comp = FairDbSqlValidityData::kOutOfDate;

      switch ( comp ) {

      case FairDbSqlValidityData::kIdentical :
	if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "   " << seqNo 
                                      << " skipped (duplicate)" << endl;
	++numDuplicate;
	break;

      case FairDbSqlValidityData::kUpdate :
	++numImpUpdate;
	tryImport = true;
	replace   = true;
	break;

      case FairDbSqlValidityData::kOutOfDate :
	if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "   " << seqNo 
                                    << " skipped (out of date)" << endl;
	++numOutOfDate;
	break;

      // Everything else is marked as conflict!
      default :
	if ( discardConflicts ) {
	  DBLOG("FairDbDS", FairDbLog::kError) << "   " << seqNo 
                                      << " skipped (conflict!)" << endl;
	  ++numConflict;
	}
	else {
	  DBLOG("FairDbDS", FairDbLog::kError) << "   " << seqNo 
                                      << " accepting conflict!" << endl;
	  ++numImpConfl;
	  tryImport = true;
	  replace   = true;
	}
      }
    }
    else {
      if ( FairDb::NotGlobalSeqNo(seqNo) )  {
        ++numLocal;
        if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "   " << seqNo 
                                   << " skipped (local sSeqNo)" << endl;
      }

      else {
	tryImport = true;
      }
    }

    if ( vrec )  FairDbTableInterfaceStore::Instance().PurgeCaches();

    // 
    if ( tryImport ) {
      // 
      if ( test ) {
        ++numTest;
        if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "   " << seqNo 
                                              << " tested" << endl;
      }
      else {

        if ( packetFile.Store(dbNo,replace) )  {
          ++numImported;
          if ( ! batch ) DBLOG("FairDbDS", FairDbLog::kInfo) << "   " << seqNo 
                                                << " imported" << endl;
          logFile.LogRec(currentTableName,seqNo);
        }
        else {
  	++numFailed;
  	 DBLOG("FairDbDS", FairDbLog::kError) << "   " << seqNo
                                          << " failed" << endl;
	 // 
	 FairDbConfigData pet;
	 FairDbTableInterfaceStore::Instance()
           .GetTableInterface(currentTableName,&pet)
             .GetDBProxy().RemoveSeqNo(seqNo,dbNo);
        }
      }
    }
  }

  //  Read errors checking
  if ( packetFile.GetNumErrors() > 0 ) {
    ++numFailed;
     DBLOG("FairDbDS", FairDbLog::kError) << "   input error on"
                             << fileName.c_str() << endl;
    }

  //Flush last table entry in log file.
  logFile.LogRec();


  DBLOG("FairDbDS", FairDbLog::kInfo)
    << "Summary of processing of packets (validity rec + data):-" <<endl
    << "  Packets skipped (local seqNo): " << numLocal << endl
    << "  Packets skipped (duplicate):   " << numDuplicate << endl
    << "  Packets skipped (out of date): " << numOutOfDate << endl
    << "  Packets skipped (conflict):    " << numConflict  << endl
    << "  Packets failed :               " << numFailed    << endl
    << "  Packets tested (not imported): " << numTest      << endl
    << "  Packets imported :             " << numImported  << endl
    << "    including conflicts :        " << numImpConfl  << endl
    << "              updates :          " << numImpUpdate << endl;
  if ( numNewTables ) DBLOG("FairDbDS", FairDbLog::kInfo) << endl
    << "  Tables Created :               " << numNewTables << endl;

  
  ofstream* log = logFile.GetStream();
  if ( log ) { 
    *log
      << "      Summary of processing of packets (validity rec + data):-" <<endl;
    if ( numLocal ) *log << "        Packets skipped (local seqNo): " 
                         << numLocal << endl;
    if ( numDuplicate ) *log << "        Packets skipped (duplicate):   " 
                 << numDuplicate << endl;
    if ( numOutOfDate ) *log << "        Packets skipped (out of date): " 
                 << numOutOfDate << endl;
    if ( numConflict ) *log << "        Packets skipped (conflict):    " 
                 << numConflict  << endl;
    if ( numFailed ) *log << "        Packets failed :               " 
                 << numFailed    << endl;
    if ( numTest ) *log << "        Packets tested (not imported): " 
                 << numTest      << endl;
    if ( numImported ) *log << "        Packets imported :             " 
                 << numImported  << endl;
    if ( numImpConfl) *log << "          including conflicts :        " 
                 << numImpConfl  << endl;
    if ( numImpUpdate ) *log << "                    updates :          " 
                   << numImpUpdate << endl;
    if ( numNewTables ) *log << endl
      << "        Tables Created :               " << numNewTables << endl;
    if ( numConflict + numFailed > 0 ) {
      *log << "      Import failed!!"  << endl;
    }
    else {
      *log << "      Import O.K."  << endl;
    }
  }

//  Close import file (even though it will as it goes out of scope).
  import.close();
    

// Exit if in Batch mode and errors have occurred.
  if (      batch
	    && ( numConflict +  numFailed ) > 0  ) this->Quit();
  return packetFile.GetNumErrors() ? kFALSE : kTRUE;

}

Bool_t FairDbADS::LogEntry(FairDbProcCmd *cmd) {

  std::list<std::string> defaultOpts;
  defaultOpts.push_back("--Table");
  defaultOpts.push_back("--Reason");
  defaultOpts.push_back("--SeqNoMin");
  defaultOpts.push_back("--FairDbDetector");
  defaultOpts.push_back("--DataType");

  FairDbCmdOpts opts("--Table:"
                     "--Reason:"
                     "--SeqNoMin:"
                     "--FairDbDetector:"
                     "--DataType:"
                     "--SeqNoMax:"
                     "--NumSeqNo:"
                     "--Version:"
                     "--UpdateTime:"
                     "--DatabaseNumber",
                     cmd, &defaultOpts);
  if ( ! opts.IsValid() ) return kFALSE;

  // 
  if ( ! opts.TestOpt("--Table" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "Table name not specified." << endl;
    return kFALSE;
  }
  string tableName = opts.GetOptString("--Table");

  if ( ! opts.TestOpt("--Reason" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "Reason not specified." << endl;
    return kFALSE;
  }
  string reason = opts.GetOptString("--Reason");

  if ( ! opts.TestOpt("--SeqNoMin" ) ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "SeqNoMin not specified." << endl;
    return kFALSE;
  }
  Int_t logSeqNoMin = opts.GetOptInt("--SeqNoMin");

  // Optional opts.
  UInt_t dbNo = 0;
  if ( opts.TestOpt("--DatabaseNumber") ) dbNo = opts.GetOptInt("--DatabaseNumber");

  //
  int dbNoFound =   FairDbTableInterfaceStore::Instance().GetConnectionPool().GetTableDbNo(tableName);
  if ( dbNoFound < 0 || dbNoFound != (int) dbNo ) {
    DBLOG("FairDbDS",FairDbLog::kWarning) << "Sorry, cannot find table " << tableName << endl;
    return kFALSE;
  }

  //
  Int_t detMask  = 2;
  Int_t simMask  = 1;
  FairDb::Version version = 0;
 
  FairDbReader<FairDbConfigData> dataPtr(tableName,logSeqNoMin,dbNo);
  const FairDbValRecord* vrec = dataPtr.GetValidityRec();
  if ( dataPtr.GetNumRows() > 0 && vrec ) {
    DBLOG("FairDb",FairDbLog::kInfo) << "Found " << tableName << " " << logSeqNoMin << endl;
    const ValInterval& vrng = vrec->GetValInterval();
    detMask  = vrng.GetDetectorMask();
    simMask  = vrng.GetSimMask();
    version     = vrec->GetVersion();  
  }

  if ( opts.TestOpt("--FairDbDetector") ) detMask = opts.GetOptInt("--FairDbDetector");

  if ( opts.TestOpt("--DataType") ) simMask = opts.GetOptInt("--DataType");

  Int_t logSeqNoMax = logSeqNoMin;
  if ( opts.TestOpt("--SeqNoMax") ) logSeqNoMax = opts.GetOptInt("--SeqNoMax");

  Int_t logNumSeqNo = logSeqNoMax - logSeqNoMin + 1;
  if ( opts.TestOpt("--NumSeqNo") ) logNumSeqNo = opts.GetOptInt("--NumSeqNo");

  if ( opts.TestOpt("--Version") ) version = opts.GetOptInt("--Version");

  ValTimeStamp updateTime;
  if ( opts.TestOpt("--UpdateTime") 
     ) updateTime = FairDb::MakeTimeStamp(opts.GetOptString("--UpdateTime"));

  // 
  FairDbLogEntry logEntry(tableName,reason,detMask,simMask,version,
                       logSeqNoMin,logSeqNoMax,logNumSeqNo);
  logEntry.SetUpdateTime(updateTime);
  DBLOG("FairDb",FairDbLog::kInfo) << "Created:-\n " 
		        <<  logEntry << endl;
  char ans = '?';
  while ( ans != 'y' && ans != 'n' ) {
    cout << "O.K. to output ? Ans y or n: ";
    cin  >> ans;
  }

  if ( ans == 'y' ) {
    bool ioResult = logEntry.Write(dbNo);
    if ( ioResult ) DBLOG("FairDb",FairDbLog::kInfo) << "Successfully wrote entry"
					  << endl;
    return ioResult;
  }
  DBLOG("FairDb",FairDbLog::kInfo) << "Entry not written" << endl;
  return kFALSE;

}

void FairDbADS::ListTables(const FairDbTableSelector& nameFilter,
                           UInt_t dbNo,
                           std::list<string>& tableNames) const {

  //
  auto_ptr<FairDbStatement> stmtDb(fConnections.CreateStatement(dbNo));
  if ( ! stmtDb.get() ) {
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Cascade entry " << dbNo
	    << " empty! " << endl;
    return; 
  }
  TSQLStatement* stmt = stmtDb->ExecuteQuery("show tables");
  if ( ! stmt ) {
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Cannot find any tables! " << endl;
    return; 
  }

  while ( kTRUE ) {
    TString name;
    if ( ! stmt->NextResultRow() ) break;
    name = stmt->GetString(0);
    string tableName = name.Data();
    DBLOG("FairDbDS", FairDbLog::kDebug )<< "examining table " << tableName << endl;

    if (    tableName.size() > 3
         && tableName.substr(tableName.size()-3) == "VAL" ) {
      tableName.erase(tableName.size()-3);
      if ( nameFilter.Test(tableName) ){
	tableNames.push_back(tableName);
	DBLOG("FairDbDS", FairDbLog::kInfo) << "preparing to export: " << tableName << endl;
      }
    }
  }
  delete stmt;

  if(tableNames.size()==0)
	DBLOG("FairDbDS", FairDbLog::kInfo) << "error no tables found! " << endl;
}


Bool_t FairDbADS::Query(FairDbProcCmd *cmd) {

  Bool_t satisfied = kFALSE;

  // Establish defaults.
  string pattern           = "*";
  FairDbDetector::Detector_t det = FairDbDetector::kSts;
  DataType::DataType_t   sim = DataType::kData;
  ValTimeStamp date;
  FairDb::Version version = 0;

  FairDbCmdOpts opts("--Table:--FairDbDetector:--Date:--DataType:--Version", cmd);
  if ( ! opts.IsValid() ) return kFALSE;
   
  if ( opts.TestOpt("--Table") ) pattern = opts.GetOptString("--Table");
  if ( opts.TestOpt("--FairDbDetector") ) {
    string detstr = opts.GetOptString("--FairDbDetector");
    if ( detstr == "Unknown"   ) det = FairDbDetector::kUnknown;
    // <DB> Here add the contents for all detector ids
    if ( detstr == "Sts"   ) det = FairDbDetector::kSts;
    
  }
  if ( opts.TestOpt("--DataType") ) {
    string simstr = opts.GetOptString("--DataType");
    if ( simstr == "Unknown"     ) sim = DataType::kUnknown;
    if ( simstr == "Data"        ) sim = DataType::kData;
    if ( simstr == "MC"          ) sim = DataType::kMC;
  }
  if ( opts.TestOpt("--Date") ) {
    string datestr = opts.GetOptString("--Date");
    date = FairDb::MakeTimeStamp(datestr);
  }

  if ( opts.TestOpt("--Version") ) {
   version = opts.GetOptInt("--Version");
  }

  // 
  ValCondition vc(det,sim,date);
  // 
  list<string> tableNames;
  this->ListTables(pattern,0,tableNames);

  //  Loop over all tables applying query.
  for ( list<string>::const_iterator itr = tableNames.begin();
        itr != tableNames.end();
        ++itr ) {
    FairDbReader<FairDbConfigData> rptr(*itr,vc,version);
    DBLOG("FairDbDS", FairDbLog::kInfo) << "Query for table: " << *itr ;
    if ( ! rptr.GetNumRows() ) DBLOG("FairDbDS",FairDbLog::kInfo) << " failed\n";  
    else {
      DBLOG("FairDbDS",FairDbLog::kInfo) << " succeeded. " 
              << rptr.GetNumRows() << " rows"
	      << "\n  ResultKey: " << *rptr.GetKey();
      satisfied = kTRUE;
    }  
    DBLOG("FairDbDS", FairDbLog::kInfo) << "  Validity Record: " << *rptr.GetValidityRec() << endl;
    
  }

  return satisfied;  


}

void FairDbADS::Quit() const {
    DBLOG("FairDbDS", FairDbLog::kInfo)  << "Exit forced due to above errors" << endl;
    exit(1);
}

void FairDbADS::SetLogLevel(FairDbProcCmd *cmd) const {

  const char* stream = cmd->PopOpt();
  const char* level  = cmd->PopOpt();
  if ( ! stream or ! level ) {
     DBLOG("FairDbDS",FairDbLog::kWarning) 
       << "Please supply both a stream and a level to the SetLogLevel command\n" << endl;
     return;
  }

  bool set_ok = FairDbLogUtil::LogLevel(stream,level);
  DBLOG("FairDbDS",FairDbLog::kInfo) << "Setting MessageService level on stream " << stream
		  << " to level " << level
		  << ( set_ok ? " succeeded" : " failed" ) << endl;
}


void FairDbADS::SetTimer(FairDbProcCmd *cmd) const {

  bool enableTimer = true;
  const char* mode = cmd->PopOpt();
  if ( mode and ! strcmp(mode,"off") ) enableTimer = false;
  FairDbStopWatchManager::gStopWatchManager.Enable(enableTimer);
  DBLOG("FairDbDS",FairDbLog::kInfo) << "Setting Timer " << (enableTimer ? "on" : "off") << endl;
}

