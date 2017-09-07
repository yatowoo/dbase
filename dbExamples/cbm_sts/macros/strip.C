//#include "Strip.h"
//#include "Test.hpp"
/*
#include "CbmStsDbQaStrip.h"
#include "CbmStsDbQaSUID.h"
#include "CbmStsDbQaSensor.h"
#include "FairParTSQLIo.h"
#include "TStopwatch.h"
#include "ValTimeStamp.h"
#include "ValInterval.h"
*/
#include<fstream>
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
  w_strip.Activate(range, 0, strip.GetVersion(), strip.GetDbEntry(), strip.GetLogTitle());
  // Writer activation:  val_range   comp.  version   db_entry   logtitle

  ValTimeStamp now;

	const char* dataFileName = 
			"strip-test_123456/170713_11-24-08_P_Measure.dat";
	const char* outputFileName = "defective_strips.txt";
 	ifstream fin(dataFileName);
	if(!fin.is_open()){
		cout << "File not found or failed to open." << endl;
		return 1;
	}
	ofstream fout(outputFileName);
	if(!fout.is_open()){
		cout << "-X- Failed to open output file - " << outputFileName << endl;
		return 1;
	}
	fout << "# Data import from -\n\t" << dataFileName << endl
		<< "# Strip ID" << endl;


	// Read data
	char buf[1024];
	fin.getline(buf,1024);
	int compid = 0;
	while(fin.is_open()){
			fin.getline(buf,1024);
			if(strlen(buf) < 10)
				break;
			compid++;
      w_strip.SetComboNo(compid);
      Int_t sid = strip.Import(compid,buf);
			if(sid != -1) // defect found
				fout << sid << endl;
      w_strip << strip;
			strip.Print();
  }

	// End
 	fin.close();
	fout.close();
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
