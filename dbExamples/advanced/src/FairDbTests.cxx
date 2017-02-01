#include "FairDbTests.h"
#include "FairDbLogService.h"

#include "FairDbConnection.h"
#include "FairDbConnectionPool.h"
#include "FairDbStatement.h"
#include "FairDbTableInterfaceStore.h"
#include "FairDbTableInterface.h"
#include "FairDbCache.h"
#include "FairDbReader.h"
#include "FairDbWriter.h"


#include "TObjectTable.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TSQLServer.h"
#include "TSQLStatement.h"


#include <fstream>
#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <math.h>
#include <vector>



ClassImp(FairDbTests)


FairDbTests::FairDbTests() :
  fNumFail(0),
  fNumPass(0),
  fGoodPool(false),
  fSql_io( new FairParTSQLIo() )
{

  DBLOG("FairDb",FairDbLog::kInfo) << "FairDbTests ctor " << this << "\n";

  // Set Log Verbosity (Info level)
  fSql_io->SetVerbosity(1);
  // Prepare the DBs
  //  this->PrepareDatabases();
}


FairDbTests::~FairDbTests() {
  DBLOG("FairDb",FairDbLog::kInfo) << "FairDbTests dtor at " << this << "\n";

  // Clean the DBs 
  this->CleanDatabases();

}

void FairDbTests::PrepareDatabases(){
  // Prepare databases by creating required data

  cout << " -I- preparing database ---- " << endl;
  
  DBLOG("FairDb",FairDbLog::kInfo) << "PrepareDatabases() " << "\n";

  // Boolean test
  Bool_t fail= kFALSE;
  
  // Establish Connections
  fSql_io->open();

  // Get Connection pool
  FairDbConnectionPool c_pool = fSql_io->GetConnections();

  // Create a SQL statement class
  auto_ptr<FairDbStatement> stmtDb(c_pool.CreateStatement(0));

  // Check if the URLs settings ar O.K 
  if ( ! stmtDb.get() ) {
    DBLOG("FairDb",FairDbLog::kError) << "Cannot get a statement for db_entry# 0  \n"
			   << "Please check the ENV_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }
  
  TSQLStatement* stmtTSQL = stmtDb->ExecuteQuery("select count(*) from FAIRDBLOGENTRY;");
  if ( stmtTSQL ) {
    stmtTSQL->NextResultRow();
    int numEntries = stmtTSQL->GetInt(0);
    if ( numEntries > 8)  {
    DBLOG("FairDb",FairDbLog::kError) << "FAIRDBLOGENTRY at entry 0 has " <<  numEntries 
			   << " entries but should have no more than 8 \n"
			   << "    will quit rather than risk damage to this table\n" << endl;
    exit(1);
   }
  }

  // O.K. the cascade looks good.
  fGoodPool = true;
 

  // now prime the two db entries
  DBLOG("FairDb",FairDbLog::kInfo) << " Adding tables to connected DBs " << endl;


 for (int iEntry = 0; iEntry <= 1; ++iEntry) {

    // Locate the commands to recreate the databases.
    ostringstream os;
    os << "/basics/sql/input_db_" << iEntry << ".sql";

    string env_str(gSystem->Getenv("VMCWORKDIR"));
    string fileName = env_str + os.str();

    // ROOT AccessPathName returns true if cannot access!
    if ( gSystem->AccessPathName(fileName.c_str()) ){
      DBLOG("FairDb",FairDbLog::kError) << " cannot open SQL file: " << fileName << endl;
    }
       
    // Open the Sql cmd file and issue cmds
    ifstream inp(fileName.c_str());
    if ( ! inp.is_open() ) {
      DBLOG("FairDb",FairDbLog::kError) << "cannot read DB priming file " << fileName << endl;
      exit(1);
    }

    // Get a statement through which to pass the commands.
    auto_ptr<FairDbStatement> stmtDbn(c_pool.CreateStatement(iEntry));
    if ( ! stmtDbn.get() ) {
      DBLOG("FairDb",FairDbLog::kError) << "Cannot get a statement for db entry# "
					<< iEntry 
			                << "\n  "
	                                << "Please check the ENV_TSQL_* environment. Quitting... "
					<< endl;
      exit(1);
    }

    // Loop over lines assembling SQL commands terminated by ;
    std::vector<std::string> sql_cmds;
    string sql; 
    while ( ! inp.eof() ) {
      string line;
      getline(inp,line);

      // Remove trailing spaces.
      while ( line.size() > 0 && *(line.rbegin()) == ' ' ) line.erase(line.size()-1);

      // Ignore empty lines and comments
      if ( line.size() == 0 || line[0] == '#' ) continue;

      sql += line;

      // If terminating semicolon found execute command.
      if ( *(line.rbegin()) == ';' ) {
        sql_cmds.push_back(sql);
        sql = "";
      }
    }

    // Now execute the assemble list of SQL commands.

    std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
    while( itr != itrEnd ) {
      std::string& sql_cmd(*itr++);
        DBLOG("FairDb",FairDbLog::kInfo) << "Priming DB "
					 << iEntry << "  SQL:" << sql_cmd << endl;
        stmtDbn->ExecuteUpdate(sql_cmd.c_str());
        if ( stmtDbn->PrintExceptions() ) fail = kTRUE;
    }

    // Refresh list of tables in connected database
    c_pool.GetConnection(iEntry)->SetTableExists();

  }//!for(iEntry...)
 
  // Force the pool to use entry 0 as the authorising DB
 
  c_pool.SetAuthorisingEntry(0);

  DBLOG("FairDb",FairDbLog::kInfo) << "DBs preparation complete." << endl;   

  cout << " -I- prepare database completed ... " << endl;
}

void FairDbTests::CleanDatabases(Bool_t printWarnings){

}


Bool_t FairDbTests::RunExample(Int_t i_ex){

  switch ( i_ex ) {
  case  0: return test_caching();
  }
  
  DBLOG("FairDb",FairDbLog::kError) << "Undefined Example: " << i_ex << "\n";
  return kFALSE;
  
}


Bool_t FairDbTests::RunAllExamples(){
  return kTRUE;
}


Bool_t FairDbTests::test_caching(){
//
//
//  Arguments: None.
//  Return:    = kTRUE if all tests are O.K., kFALSE otherwise.  
//
//  Purpose:
//  o Check caching mechanism of FairDbExPar1.
//
 
  Bool_t ok = kTRUE;
  
  struct Test_t {
    Int_t    dateCntx;
    Int_t    timeCntx;
  };
  
  Test_t tests[] = {
    {20140130,120000},
    {20140131,110000},
    {20150102,100000},
    {20150121, 90000},
  };
  
  
  // Initially the cache should have not have any FairDbReader with clients
  // so we can force a purge that will empty it.
  
  FairDbCache* cache = const_cast<FairDbTableInterface&>
                        ( FairDbReader<FairDbExPar1>::GetTableInterface() )
                        .GetCache();

  
  cache->Purge();

   if ( cache->GetCurSize() != 0 ) {
     cout
       << "Initial cache size is  " <<  cache->GetCurSize()
       << " but should be 0 " << endl;
     ok = kFALSE;
   }
   unsigned int numResused = cache->GetNumReused();

   // Force cache to be overfull by requesting multiple different
   // validities simultaneously.
  list<FairDbReader<FairDbExPar1>*> pResPtrs;
  for (int itest=0; itest<4; ++itest ) {
    Test_t& test = tests[itest];
    Int_t date = test.dateCntx;
    Int_t time = test.timeCntx;
    ValTimeStamp tstamp(date/10000, date/100%100, date%100,
                        time/10000, time/100%100, time%100);
    ValCondition vc(FairDbDetector::kSts,DataType::kData,tstamp);

    // Create multiple requests to test cache sharing
    pResPtrs.push_back( new FairDbReader<FairDbExPar1>(vc,0) );
    pResPtrs.push_back( new FairDbReader<FairDbExPar1>(vc,0) );
    pResPtrs.push_back( new FairDbReader<FairDbExPar1>(vc,0) );
  }

// There should now be 4 in the cache, each reused twice.
  numResused = cache->GetNumReused() - numResused;
  if (    cache->GetCurSize() != 4  
       || numResused != 8 ) {
      
    cout  << "-E- Final cache size is  " <<  cache->GetCurSize()
	  << " but should be 4 " 
	  << ", resuse count is "<< numResused
	  << " but should be 8 " << endl;
    ok = kFALSE;
  }


// Now delete all the FairDbReader, purge  and the cache should drop
// back to zero.

  for ( list<FairDbReader<FairDbExPar1>*>::iterator itr
             = pResPtrs.begin();
        itr != pResPtrs.end();
        ++itr ) delete *itr;

  cache->Purge();

   if ( cache->GetCurSize() != 0 ) {
     cout
       << "-E- Emptied cache size is  " <<  cache->GetCurSize()
       << " but should be 0 " << endl;
     ok = kFALSE;
   }
   
// Test marking of cache as stale to force reloading.
   for (Int_t itry =1; itry <= 10; ++itry ) {
     Test_t& test = tests[0];
     Int_t date = test.dateCntx;
     Int_t time = test.timeCntx;
     ValTimeStamp tstamp(date/10000, date/100%100, date%100,
                         time/10000, time/100%100, time%100);
     ValCondition vc(FairDbDetector::kSts,DataType::kData,tstamp);
     cache->SetStale();
     FairDbReader<FairDbExPar1> pr1(vc,0);
   }

  numResused = cache->GetNumReused() - numResused;
  if (    cache->GetCurSize() != 1  
          || numResused != 0 ) {
     
    cout   << "-E- SetStale test: Final cache size is  " <<  cache->GetCurSize()
	   << " but should be 1 " 
	   << ", resuse count is "<< numResused
	   << " but should be 0 " << endl;
    ok = kFALSE;
  }

  
  cout  << "Test completed: Caching of FairDbReader                  ...... ";
  cout  << ( ok ? "O.K." : "Failed") ;
  cout  << "\n";


  if ( ok ) fNumPass++; else fNumFail++;

  return ok;
   
}

