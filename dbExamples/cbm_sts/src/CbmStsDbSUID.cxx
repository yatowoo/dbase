#include "CbmStsDbSUID.h"

#include "FairDb.h"                     // for GetValDefinition, etc
#include "FairDbTableInterfaceStore.h"
#include "FairDbLogService.h"
#include "FairDbExceptionLog.h"         // for FairDbExceptionLog
#include "FairDbStatement.h"            // for FairDbStatement
#include "FairDbString.h"               // for FairDbString, StringTok, etc

#include "TSQLStatement.h"              // for TSQLStatement
#include "TString.h"                    // for TString
#include "TSystem.h"                    // for TSystem, gSystem

#include <ctype.h>                      // for islower
#include <cstdlib>                      // for exit
#include <iostream>                     // for cout
#include <memory>                       // for auto_ptr, etc
#include <utility>                      // for pair

using std::cout;
using std::endl;
using std::ostringstream;
using std::map;
using std::auto_ptr;
using std::string;

ClassImp(CbmStsDbSUID)


CbmStsDbSUID* CbmStsDbSUID::fgInstance       = 0;


CbmStsDbSUID::CbmStsDbSUID()
  : TObject()                           
  , fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  DBLOG("FairDb",FairDbLog::kDebug) << "CbmStsDbSUID singleton is created  \n"<< endl;
}

CbmStsDbSUID::~CbmStsDbSUID()
{
  DBLOG("FairDb",FairDbLog::kDebug) << " ~ Destroying ~ CbmStsDbSUID" << endl;
  CbmStsDbSUID::fgInstance = 0;
}

CbmStsDbSUID& CbmStsDbSUID::Instance()
{
  if ( ! fgInstance ) {
    fgInstance = new CbmStsDbSUID();
  }
  return *fgInstance;
}

Int_t CbmStsDbSUID::Init(Int_t dbNo)
{
  DBLOG("FairDb",FairDbLog::kInfo) << " Init Sensor UID table  on db_entry: " << dbNo << endl;
  
  FairDbString sql;
  
  string suidTableName = "STSDBSUID";
  
  auto_ptr<FairDbStatement> stmtDb(fMultConn->CreateStatement(dbNo) );
  if ( ! stmtDb.get() ) { return -1; }
  
  bool suidTableNameExists = fMultConn->TableExists(suidTableName,dbNo);
  if ( ! suidTableNameExists ) {
    DBLOG("FairDb",FairDbLog::kInfo) << " Creating Sensor UID table  on db_entry: " << dbNo << endl;
    sql.Clear();
    sql << "CREATE TABLE " << suidTableName
        << "(BATCHID      CHAR(64) NOT NULL PRIMARY KEY,\n"
        << "VENDOR        CHAR(64),\n"
        << "WAFERID       INT,\n"
        << "SENSORID      INT,\n"      
          << " SUID  INT )";

      DBLOG("FairDb",FairDbLog::kInfo)<< "Database: " << dbNo
                                      << " create local Sensor UID table sql_cmd: " << sql.c_str() << endl;
      stmtDb->ExecuteUpdate(sql.c_str());
      if ( stmtDb->PrintExceptions() ) { return -1; }
      sql.Clear();
    }
  return 0;
}
  
