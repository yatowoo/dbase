#include "FairDbLogFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

#include "FairDbCmdOpts.h"
#include "FairDb.h"
#include "FairDbLogEntry.h"
#include "FairDbReader.h"
#include "FairDbTableInterface.h"
#include "FairDbValRecord.h"
#include "FairDbLogService.h"
#include "ValTimeStamp.h"

ClassImp(FairDbLogFile)


FairDbLogFile::FairDbLogFile(const string& fileName,UInt_t dbNo) :
fDbNo(dbNo),
fLogFile(0)
{

  DBLOG("FairDbDS", FairDbLog::kVerbose) << "FairDbLogFile() ctor" << endl;

  if ( fileName == "") return;
  
  fLogFile = new ofstream(fileName.c_str(),ios::app);
  if ( ! fLogFile->is_open() ) {
    DBLOG("FairDbDS",FairDbLog::kError) << "Cannot open file " << fileName << endl;
    return;
  }    
}

FairDbLogFile::~FairDbLogFile() {

  DBLOG("FairDbDS", FairDbLog::kVerbose) << " ~FairDbLogFile() dtor " << endl;

  LogRec(0);
  delete fLogFile;
  fLogFile = 0;

}

void FairDbLogFile::LogCmd(const string& cmd,
                        const FairDbCmdOpts& opts)  {

  if ( ! IsValid() ) return;
  this->LogMsg("",kFALSE);
  string msg(cmd);
  msg += " ";
  msg += opts.GetOpts();

  string insert("+\n      ");
  Int_t locOpt = 0;
  while (  (locOpt = msg.find("--",locOpt)) != string::npos ) {
    // do insert if option found
    msg.insert(locOpt,insert);
    // locOpt is the position from where to look for.
    // has then to be shifted to new pos.
    locOpt += insert.size() + 2;
  }
  this->LogMsg(msg);
  return;
}

void FairDbLogFile::LogMsg(const string& msg,
                        Bool_t addDateStamp)  {
  if ( ! IsValid() ) return;
  if ( addDateStamp ) {
    ValTimeStamp now;
    (*fLogFile) << FairDb::MakeDateTimeString( now )
	      << " ";
  }
  (*fLogFile) << msg <<  endl;
  return;
}

void FairDbLogFile::LogRec(const FairDbValRecord* vrec ) {

  if ( ! IsValid() ) return;
  string tableName =  vrec ? vrec->GetTableInterface()->GetTableName() : "";
  UInt_t seqNo =  vrec ? vrec->GetSeqNo() : 0;  
  LogRec(tableName,seqNo);
}

void FairDbLogFile::LogRec(const string tableName, UInt_t seqNo) {

  if ( ! IsValid() ) return;
  //  Check change of table name output any buffered log info.
  if ( tableName != fLogTableName ) {
    if ( fLogDBSrcNos.size() ) {
      list<UInt_t>::const_iterator itr    = fLogDBSrcNos.begin();
      list<UInt_t>::const_iterator itrEnd = fLogDBSrcNos.end();
      while ( itr != itrEnd ) {
        UInt_t dbSrcNo = *itr;
        (*fLogFile) << "      "
	            << fLogTableName << " "
	            << fLogNumSeqNo[dbSrcNo] << " recs: "
	            << fLogSeqNoMin[dbSrcNo];
        if ( fLogSeqNoMin[dbSrcNo] != fLogSeqNoMax[dbSrcNo]
             ) (*fLogFile) << ".." << fLogSeqNoMax[dbSrcNo];
        (*fLogFile) << endl;
        ++itr;
      }
    }
    if ( tableName == "FAIRDBLOGENTRY" 
	 ) (*fLogFile) << "Start of update log entries..." << endl;
    else if ( fLogTableName == "FAIRDBLOGENTRY" 
              ) (*fLogFile) << "End of update log entries..." << endl;
    
    fLogTableName =  tableName;
    fLogDBSrcNos.clear();
    fLogNumSeqNo.clear();
    fLogSeqNoMin.clear();
    fLogSeqNoMax.clear();
  }
  
  // Buffer info for next the report summary.
  if ( tableName != "" ) {
    UInt_t dbSrcNo = seqNo/(FairDb::kMAXLOCALSEQNO+1);
    if (     find(fLogDBSrcNos.begin(), fLogDBSrcNos.end(), dbSrcNo)
             == fLogDBSrcNos.end() ) {
      fLogDBSrcNos.push_back(dbSrcNo);
      fLogSeqNoMin[dbSrcNo] = seqNo;
      fLogSeqNoMax[dbSrcNo] = seqNo;
      fLogNumSeqNo[dbSrcNo] = 1;
    }
    else {
      if ( fLogSeqNoMin[dbSrcNo] > seqNo ) fLogSeqNoMin[dbSrcNo] = seqNo;
      if ( fLogSeqNoMax[dbSrcNo] < seqNo ) fLogSeqNoMax[dbSrcNo] = seqNo;
      ++fLogNumSeqNo[dbSrcNo];
    }
  }
  
  // Store LogEntries directly into the file.
  if ( tableName == "FAIRDBLOGENTRY" ) {
    FairDbReader<FairDbLogEntry> rp("FAIRDBLOGENTRY",seqNo,fDbNo);
    const FairDbLogEntry* le = rp.GetRow(0);
    if ( le ) (*fLogFile) << *le << endl;
  }

}

