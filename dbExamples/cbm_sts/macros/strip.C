//#include "Strip.h"
//#include "Test.hpp"

#if __GNUC__ != 3
#include "CbmStsDbQaStrip.h"
#include "CbmStsDbQaSUID.h"
#include "CbmStsDbQaSensor.h"
#include "FairParTSQLIo.h"
#include "TStopwatch.h"
#include "ValTimeStamp.h"
#include "ValInterval.h"
#endif

#include<string>

void StripTest(bool update=false){
//
  //
  // Purpose: - Write mechanism of non-composite data
  //            from database table
  //

  TStopwatch t;
  t.Start();
  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(0);

  //Write skipping 0 entry 
  Int_t db_entry = 0;
  
  ValTimeStamp start;
  ValTimeStamp   end = ValTimeStamp::GetEOT();
    
  ValInterval  range(FairDbDetector::kSts,
                     DataType::kData,start,end,"test");
  
  CbmStsDbQaStrip strip;
	strip.SetVersion(0);
	strip.SetDbEntry(db_entry);
	strip.SetLogTitle("Strip");
	strip.SetCompId(0);
	strip.Print();
	strip.CreateStringUID();
  strip.CreateDbTable(0);

  // Create the Writer 
  FairDbWriter<CbmStsDbQaStrip> w_strip;
  // Writer activation:  val_range   comp.  version   db_entry   logtitle

  ValTimeStamp now;

  
  if (!update) {
      w_strip.Activate(range, 0, strip.GetVersion(), strip.GetDbEntry(), strip.GetLogTitle());
      w_strip.SetComboNo(0);
      strip.SetCompId(0);
      w_strip << strip;
			strip.clear();
			strip.Print();
  } else {
    // update the last component
    int i = 199;  
    w_strip.Activate(range, i, strip.GetVersion(), strip.GetDbEntry(), strip.GetLogTitle());
    w_strip.SetComboNo(i);
    strip.SetCompId(i);
    w_strip << strip;
    
  }
  
  if(!w_strip.Close()){
      cout << "-E- IO error writing par1 " << endl;
   }
  
  t.Stop();
  t.Print();
  
  // Close DB Connection 
  //if (sql_io) sql_io->Close();
}

void strip()
{
	StripTest();
	return;
}

int main(int argc, char** argv)
{
	StripTest();
	return 0;
}
