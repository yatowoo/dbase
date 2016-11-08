
#include <string>
#include <iostream>

#include "TString.h"
#include "TSystem.h"

#include "FairDbConnectionPool.h"
#include "FairDbTableInterfaceStore.h"
#include "FairDbStopWatchManager.h"
#include "FairDbADS.h"
#include "FairDbQAssessor.h"
#include "FairDbProcCmd.h" 
#include "FairDbProcArgs.h"

int main(int argc, char** argv) {

// Create a process environment, so that user can specify DB pool
// using the -d -u -p options.
  FairDbProcArgs& e = FairDbProcArgs::Instance(argc, argv);
  e.IsBatch();

// Switch off the DBI timer.
  FairDbStopWatchManager::gStopWatchManager.Enable(false);

// Force all connections to be permanent.
  FairDbConnectionPool& db_pool =  FairDbTableInterfaceStore::Instance().GetConnectionPool();
  Int_t dbNo = db_pool.GetNumDb();
  while ( --dbNo >= 0 ) db_pool.SetPermanent(dbNo);

  FairDb::SetLogLevel(0);

  FairDbADS ads_driver;
  cout << "FairDb Admin Tool version  V01.01\n\n" 
       << "Enter command, type Help for help: " << endl;
  string line;
  while ( 1 ) {
    cout << "FairDbDS: ";
    getline(cin,line);
    // Remove leading blanks.
    while ( line.size() > 0 && line[0] == ' ' ) line.erase(0,1);
    // Process if there is anything left!
    if ( line.size() > 0 ) {
      TString cmd = line.c_str();
      cmd.ToLower();
      if ( cmd == "quit" ) break;
      if ( cmd == "qa" ) {
        FairDbQAssessor v(&ads_driver);
      }
      else {
        FairDbProcCmd pcmd(line.c_str());
        ads_driver.HandleCommand(&pcmd);
      }
    }
  }
  return 0;
  
}
