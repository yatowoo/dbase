#include "CbmStsDbQaStrip.h"

#include "FairRunIdGenerator.h"
#include "FairDb.h"                     // for GetValDefinition, etc
#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"            // for FairDbStatement
#include "FairDbStreamer.h"            // for FairDbStatement
#include "FairDbExceptionLog.h"         // for FairDbExceptionLog
#include "FairDbStatement.h"            // for FairDbStatement
#include "FairDbString.h"               // for FairDbString, StringTok, etc

#include "TSQLStatement.h"              // for TSQLStatement



#include "FairParamList.h"              // for FairParamList

#include "Riosfwd.h"                    // for ostream
#include "TString.h"                    // for TString
#include "TSystem.h"

#include <stdlib.h>                     // for exit
#include <memory>                       // for auto_ptr, etc
#include <vector>                       // for vector, vector<>::iterator
#include <unistd.h>
#include <sys/time.h>


using namespace std;

ClassImp(CbmStsDbQaStrip);

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaStrip>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaStrip>;




CbmStsDbQaStrip::CbmStsDbQaStrip(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fUID(0),
		fEdge(0),
		fStripId(0),
		fIstrip(0.),
		fPinhole(0.),
		fAlShortL(0.),
		fAlShortR(0.),
		fCCouplingCp(0.),
		fCompId(-1),
    fVendor(""),
    fBatchId(""),
    fBatchTime(""),
    fWaferId(-1),
    fSensorId(-1)
{
  
  // Set the default Db Entry to the first slot
  SetDbEntry(0);
  // No  Aggregation
  SetCompId(fCompId);
  // Init Version
  SetVersion(0);
  
  //  Writer Meta-Class for SQL IO
  fParam_Writer=NULL;
  //  Reader Meta-Class for SQL IO
  fParam_Reader=NULL;

  // Reset all parameters
  clear();

  // ConnectionPool
  fMultConn=FairDbTableInterfaceStore::Instance().fConnectionPool;
}


CbmStsDbQaStrip::~CbmStsDbQaStrip()
{
   
  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }
  
  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }
  
}


void CbmStsDbQaStrip::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaStrip::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("vendor",   (Text_t*) fVendor.c_str());
  list->add("batch_id", (Text_t*) fBatchId.c_str());

  list->add("batch_time", (Text_t*) fBatchTime.c_str());
  list->add("wafer_id",   fWaferId);
  list->add("sensor_id",  fSensorId);
}

Bool_t CbmStsDbQaStrip::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id", &fCompId)) { return kFALSE; }

  Text_t aName[155];
  if (!list->fill("vendor",     aName, 155 )) { return kFALSE;}
  fVendor = aName;

  Text_t bName[155];
  if (!list->fill("batch_id",   bName, 155 )) { return kFALSE; }
  fBatchId = bName;
  
  Text_t tName[155];
  if (!list->fill("batch_time", tName, 155)) { return kFALSE; }
  fBatchTime = tName;
  
  if (!list->fill("wafer_id", &fWaferId)) { return kFALSE; }
  if (!list->fill("sensor_id", &fSensorId)) { return kFALSE; }
  
  return kTRUE;
}

void CbmStsDbQaStrip::clear()
{
  fCompId=fWaferId=fSensorId=-1;
  fVendor= fBatchId=fBatchTime="";
}


string CbmStsDbQaStrip::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSQAStrip"; }
  sql += "( SEQNO                 INT NOT NULL,";
  sql += "  ROW_ID                INT NOT NULL,";
  sql += "  COMP_ID               INT,";
  sql += "  UNIQUE_ID             INT,";
  sql += "  EDGE                  INT,";
  sql += "  STRIP_ID              INT,";
  sql += "  I_STRIP               DOUBLE,";
  sql += "  PINHOLE               DOUBLE,";
  sql += "  AL_SHORT_L            DOUBLE,";
  sql += "  AL_SHORT_R            DOUBLE,";
  sql += "  CCOUPLING_CP          DOUBLE,";
 	 
	/*
	sql += "  VENDOR                CHAR(64),";
  sql += "  BATCH_ID              CHAR(64),";
  sql += "  BATCH_TIME            CHAR(64),";
  sql += "  WAFER_ID              INT,";
  sql += "  SENSOR_ID             INT,";
	*/
  sql += "  primary key(SEQNO,ROW_ID),"; 
  sql += "index(COMP_ID))";

  return sql;
}


void CbmStsDbQaStrip::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  //res_in >> fCompId  >> fVendor >> fBatchId  >> fBatchTime >> fWaferId >> fSensorId;
	res_in >> fCompId >> fUID >> fEdge >> fSensorId >> fIstrip >> fPinhole
		>> fAlShortL >> fAlShortR >> fCCouplingCp;
}

void CbmStsDbQaStrip::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
	//res_out << fCompId << fVendor << fBatchId << fBatchTime << fWaferId << fSensorId;
	res_out << fCompId << fUID << fEdge << fSensorId << fIstrip << fPinhole
		<< fAlShortL << fAlShortR << fCCouplingCp;
}


void CbmStsDbQaStrip::fill(UInt_t rid)
{
  // Get Reader Meta Class
  fParam_Reader=GetParamReader();

  // Define a Context
  ValTimeStamp ts(rid);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  // Activate reading for this Context
  fParam_Reader->Activate(context, GetVersion());

  // By default use the latest row entry
  // (Other rows would correspond to outdated versions)
  Int_t numRows = fParam_Reader->GetNumRows();
  if ( numRows > 1 ) { numRows = 1; }

  for (int i = 0; i < numRows; ++i) {
    CbmStsDbQaStrip* cgd = (CbmStsDbQaStrip*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId = cgd->GetCompId();
    

  }

}


void CbmStsDbQaStrip::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaStrip::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAStrip") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQAStrip").Data());
    sql_cmds.push_back(CbmStsDbQaStrip::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaStrip::Store() ******* Error Executing SQL commands ***********  " << endl;
    }

  }

  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();

  // Writer Meta-Class Instance
  fParam_Writer = GetParamWriter();


  // Activate Writer Meta-Class with the proper
  // Validity Time Interval (run_id encapsulated)

  // Writer Activate() arguments list
  //                      <Arguments>                   <Type>                  <Comments>
  //
  //                      Validity Interval,            ValInterval
  //                      Composition      ,            Int_t                   set via cont. factory
  //                      Version          ,            Int_t                   set via cont. factory
  //                      DbEntry          ,            Int_t                   set via cont. factory
  //                      LogTitle         ,            std::string             set via cont. factory

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS Sensor UID");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaStrip::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaStrip::Print()
{
  std::cout<<"   CbmStsDbQaStrip Parameters: " << std::endl;
  std::cout<<"   CompId:    "<<  fCompId    <<  std::endl;
  std::cout<<"   Vendor:    "<<  fVendor   <<  std::endl;
  std::cout<<"   BatchId:   "<<  fBatchId   <<  std::endl;
  std::cout<<"   BatchTime: "<<  fBatchTime <<  std::endl;
  std::cout<<"   WaferId:   "<<  fWaferId   <<  std::endl;
  std::cout<<"   SensorId:  "<<  fSensorId  <<  std::endl;
}


Bool_t CbmStsDbQaStrip::Compare(const CbmStsDbQaStrip& that ) const {
  //
  Bool_t test_h =
    (   fCompId     == that.fCompId) 
    && (fBatchId    == that.fBatchId)
    && (fBatchTime  == that.fBatchTime)
    && (fWaferId    == that.fWaferId)
    && (fSensorId   == that.fSensorId);
  return test_h;
}



FairDbWriter<CbmStsDbQaStrip>* CbmStsDbQaStrip::ActivateWriter(Int_t rid)
{
  if (rid == 0 ) {
    FairRunIdGenerator runID;
    rid =  runID.generateId();
  }
  
  // delete if already existing
  if (fParam_Writer) {
    cout << "-E- CbmStsDbQaStrip::ActivateWriter() already in use ---> reset Writer Template." << endl;
    delete fParam_Writer; fParam_Writer=NULL;
  }else {
    // Create according to IoV
    Bool_t fail= kFALSE;
    
    // Create a unique statement on choosen DB entry
    auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
    if ( ! stmtDbn.get() ) {
      cout << "-E-  CbmStsDbQaStrip::Store()  Cannot create statement for Database_id: " << GetDbEntry()
           << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
      exit(1);
    }
    
    // Check if for this DB entry the table already exists.
    // If not call the corresponding Table Definition Function
    std::vector<std::string> sql_cmds;
    TString atr(GetName());
    atr.ToUpper();
    
    if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAStrip") ) {
      sql_cmds.push_back(FairDb::GetValDefinition("STSQAStrip").Data());
      sql_cmds.push_back(CbmStsDbQaStrip::GetTableDefinition());
    }
    
    // Packed SQL commands executed internally via SQL processor
    std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
    while( itr != itrEnd ) {
      std::string& sql_cmd(*itr++);
      stmtDbn->Commit(sql_cmd.c_str());
      if ( stmtDbn->PrintExceptions() ) {
        fail = true;
        cout << "-E- CbmStsDbQaStrip::Store() ******* Error Executing SQL commands ***********  " << endl;
      }
      
    }
    
    // Refresh list of tables in connected database
    // for the choosen DB entry
    fMultConn->GetConnection(GetDbEntry())->SetTableExists();
    
    // Writer Meta-Class Instance
    fParam_Writer = GetParamWriter();
    fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS Sensor UID");
    return fParam_Writer;
  }
  return NULL;
}


void CbmStsDbQaStrip::CreateDbTable(Int_t db_entry)
{
  // Set new DB Entry
  SetDbEntry(db_entry);
  
  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
   if ( ! stmtDbn.get() ) {
    cout << "-E-  FairDbDemoPar1::CreateDbTable()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }


  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSQAStrip") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSQAStrip").Data());
    sql_cmds.push_back(CbmStsDbQaStrip::GetTableDefinition());
  }
  

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaStrip::CreateDbTable() * Error Executing SQL commands **  " << endl;
    }

  }

  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();
  
}


string CbmStsDbQaStrip::CreateStringUID(){
  // Generate string based ID
  std::ostringstream str_id;
  str_id << fVendor << "_" << fBatchId << "_" << fWaferId << "_" << fSensorId;   
  string res_id = str_id.str();
  //cout << "-I- CbmStsDbQaStrip:CreateStringUID ----> " << res_id << endl;
  return res_id;
}


Bool_t CbmStsDbQaStrip::IsEqual(const TObject* that ) const {
  
  CbmStsDbQaStrip *cthat = (CbmStsDbQaStrip*) that;  
  Bool_t test_h = kFALSE;
  // DB I remove here the test with batch time
  test_h =
       (fBatchId    == cthat->fBatchId)
    && (fWaferId    == cthat->fWaferId)
    && (fSensorId   == cthat->fSensorId);
  
  return test_h;
}

