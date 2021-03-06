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
		fCCouplingDF(0.),
		fCompId(-1),
		fPassed(kFALSE)
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
  list->add("unique_id", fUID);
	list->add("edge", fEdge);
	list->add("strip_id", fStripId);
	list->add("i_strip", fIstrip);
	list->add("pinhole", fPinhole);
	list->add("al_short_L", fAlShortL);
	list->add("al_short_R", fAlShortR);
	list->add("ccoupling_cp", fCCouplingCp);
	list->add("ccoupling_df", fCCouplingDF);
	list->add("passed", fPassed);
}

Bool_t CbmStsDbQaStrip::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id", &fCompId)) { return kFALSE; }
  if (!list->fill("unique_id", &fUID)){ return kFALSE;}
	if (!list->fill("edge", &fEdge)){ return kFALSE;}
	if (!list->fill("strip_id", &fStripId)){ return kFALSE;}
	if (!list->fill("i_strip", &fIstrip)){ return kFALSE;}
	if (!list->fill("pinhole", &fPinhole)){ return kFALSE;}
	if (!list->fill("al_short_L", &fAlShortL)){ return kFALSE;}
	if (!list->fill("al_short_R", &fAlShortR)){ return kFALSE;}
	if (!list->fill("ccoupling_cp", &fCCouplingCp)){ return kFALSE;}
	if (!list->fill("ccoupling_df", &fCCouplingDF)){ return kFALSE;}
	if (!list->fill("passed", &fPassed)){ return kFALSE;}

  return kTRUE;
}

void CbmStsDbQaStrip::clear()
{
  fCompId=fEdge=fStripId=-1;
	fIstrip=fPinhole=fAlShortL=fAlShortR=fCCouplingCp=fCCouplingDF=0.;
	fPassed=kFALSE;
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
  sql += "  CCOUPLING_DF          DOUBLE,";
  sql += "  PASSED                INT,";
  sql += "  primary key(SEQNO,ROW_ID),"; 
  sql += "index(COMP_ID))";

  return sql;
}


void CbmStsDbQaStrip::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
	res_in >> fCompId >> fUID >> fEdge >> fStripId >> fIstrip >> fPinhole
		>> fAlShortL >> fAlShortR >> fCCouplingCp >> fCCouplingDF >> fPassed;
}

void CbmStsDbQaStrip::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
	res_out << fCompId << fUID << fEdge << fStripId << fIstrip << fPinhole
		<< fAlShortL << fAlShortR << fCCouplingCp << fCCouplingDF << fPassed;
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
  	fUID    = cgd->GetUID();
		fEdge   = cgd->GetEdge();	
		fStripId= cgd->GetStripId();	
		fIstrip = cgd->GetStripCurrent();	
		fPinhole= cgd->GetPinhole();	
		fAlShortL= cgd->GetAlShortLeft();	
		fAlShortR= cgd->GetAlShortRight();	
		fCCouplingCp= cgd->GetCCouplingCp();	
		fCCouplingDF= cgd->GetCCouplingDF();
	  fPassed = cgd->GetPassed();	
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
	std::cout<<"-O- CBM-STS QA Strip Test " << std::endl;
	std::cout<<" Unique-ID: " << fUID << "\tCompNo : " << fCompId << std::endl;
  std::cout<<"   Edge        : "<<  fEdge       <<  std::endl;
  std::cout<<"   Strip_ID    : "<<  fStripId    <<  std::endl;
  std::cout<<"   I_Strip     : "<<  fIstrip     <<  std::endl;
  std::cout<<"   Pinhole     : "<<  fPinhole    <<  std::endl;
  std::cout<<"   Al_Short_L  : "<<  fAlShortL   <<  std::endl;
  std::cout<<"   Al_Short_R  : "<<  fAlShortR   <<  std::endl;
  std::cout<<"   CCoupling_Cp: "<<  fCCouplingCp<<  std::endl;
  std::cout<<"   CCoupling_DF: "<<  fCCouplingDF<<  std::endl;
  std::cout<<"   Passed      : "<<  fPassed     <<  std::endl;
}


Bool_t CbmStsDbQaStrip::Compare(const CbmStsDbQaStrip& that ) const {
  //
  Bool_t test_h =
    (   fCompId     == that.fCompId) 
    && (fEdge       == that.fEdge)
    && (fStripId    == that.fStripId)
    && (fIstrip     == that.fIstrip)
    && (fPinhole    == that.fPinhole)
    && (fAlShortL   == that.fAlShortL)
    && (fAlShortR   == that.fAlShortR)
    && (fCCouplingCp== that.fCCouplingCp)
    && (fCCouplingDF== that.fCCouplingDF)
		&& (fPassed     == that.fPassed);
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
    fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS Strip UID");
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
  str_id << fUID << "_" << fEdge << "_" << fStripId;   
  string res_id = str_id.str();
	std::cout << "-I- CbmStsDbQaStrip:CreateStringUID ----> " << res_id << std::endl;
  return res_id;
}


Bool_t CbmStsDbQaStrip::IsEqual(const TObject* that ) const {
  
  CbmStsDbQaStrip *cthat = (CbmStsDbQaStrip*) that;  
  Bool_t test_h = kFALSE;
  // DB I remove here the test with batch time
  test_h =
       (fUID        == cthat->fUID)
    && (fEdge       == cthat->fEdge)
    && (fStripId    == cthat->fStripId);
  
  return test_h;
}

#include<sstream>
Int_t CbmStsDbQaStrip::Import(Int_t compid, string value)
{

	fCompId = compid;

	stringstream sstr;
	sstr << value;
	
	// TODO : Check value
	sstr >> fEdge >> fStripId >> fIstrip >> fPinhole
		>> fAlShortL >> fAlShortR >> fCCouplingCp >> fCCouplingDF;
	fPassed = CheckValue();
	if(!fPassed){
		std::cout << "-X- Strip : " << fEdge << "-" << fStripId 
			<< " is defect" << std::endl;
		return fStripId;
	}
	
	return -1;
}

#include "TMath.h"

Bool_t CbmStsDbQaStrip::CheckValue()
{
	using TMath::Abs;
	static const Double_t I_SENSOR = 10e-6;  // Unit : A
	static const Double_t LENGTH_STRIP = 6.; // Unit : cm
	Bool_t isDefect = 
	    (Abs(fIstrip) >= 5 * I_SENSOR / 1024) 
	 || (Abs(fPinhole) >= 10e-9) 
	 || (Abs(fCCouplingCp) <= 10e-12 * LENGTH_STRIP);
	
	return (!isDefect);
}

