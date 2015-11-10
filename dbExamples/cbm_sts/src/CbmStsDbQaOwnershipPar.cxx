#include "CbmStsDbQaOwnershipPar.h"

#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"              // for FairDbStatement
#include "FairDbStreamer.h"               // for FairDbStreamer
#include "FairParamList.h"              // for FairParamList
#include "FairDbParFactory.h"


#include "Riosfwd.h"                    // for ostream
#include "TString.h"                    // for TString

#include <stdlib.h>                     // for exit
#include <memory>                       // for auto_ptr, etc
#include <vector>                       // for vector, vector<>::iterator


using namespace std;

ClassImp(CbmStsDbQaOwnershipPar);


static FairDbParRegistry<CbmStsDbQaOwnershipPar> qa_iv("CbmStsDbQaOwnershipPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaOwnershipPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaOwnershipPar>;



CbmStsDbQaOwnershipPar::CbmStsDbQaOwnershipPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own), 
    fCompId(-1),
    fUID(0),
    fVendor(""),
    fType(""),
	  fReticleName(""),
    fYear(""),
    fOwner(""),
    fLocation(""), 
    fParam_Writer(NULL), //  Writer Meta-Class for SQL IO
    fParam_Reader(NULL), //  Reader Meta-Class for SQL IO
    fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{

     // Set the default Db Entry to the first slot
     SetDbEntry(0);  
     // No  Aggregation
     SetCompId(fCompId);
     // Init Version 
     SetVersion(0);
}


CbmStsDbQaOwnershipPar::~CbmStsDbQaOwnershipPar()
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


void CbmStsDbQaOwnershipPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaOwnershipPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);
  list->add("vendor",   (Text_t*) fVendor.c_str());
  list->add("type",     (Text_t*) fType.c_str());
  list->add("reticle_name",  (Text_t*) fReticleName.c_str());
  list->add("year",          (Text_t*) fYear.c_str());
  list->add("owner",         (Text_t*) fOwner.c_str());
  list->add("location",      (Text_t*) fLocation.c_str());  
}

Bool_t CbmStsDbQaOwnershipPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  Text_t aName[155]; 
  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }
  if (!list->fill("vendor",    aName, 155 )) { return kFALSE;}
  fVendor = aName;
  if (!list->fill("type",    aName, 155 )) { return kFALSE;}
  fType = aName;
  if (!list->fill("reticle_name",    aName, 155 )) { return kFALSE;}
  fReticleName= aName;
  if (!list->fill("year",    aName, 155 )) { return kFALSE;}
  fYear = aName;
  if (!list->fill("owner",    aName, 155 )) { return kFALSE;}
  fOwner = aName;
  if (!list->fill("location",    aName, 155 )) { return kFALSE;}
  fLocation = aName;
  return kTRUE;
}

void CbmStsDbQaOwnershipPar::clear()
{ 
  fCompId=fUID=0;
 
  fVendor="";
  fType="";
  fReticleName="";
  fYear="";
  fOwner="";
  fLocation="";

  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaOwnershipPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "CBMSTSDBQAOWNERSHIPPAR"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  COMP_ID        INT,";
  sql += "  UID            INT,";
  sql += "  VENDOR         TEXT,";
  sql += "  TYPE           TEXT,";
  sql += "  RETICLE        TEXT,";
  sql += "  YEAR           TEXT,";
  sql += "  OWNER          TEXT,";
  sql += "  LOCATION       TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaOwnershipPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
   res_in >> fCompId 
          >> fUID 
          >> fVendor 
          >> fType
          >> fReticleName 
          >> fYear 
          >> fOwner
          >> fLocation;
}

void CbmStsDbQaOwnershipPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fCompId 
          << fUID 
          << fVendor 
          << fType
          << fReticleName 
          << fYear 
          << fOwner
          << fLocation;
}


void CbmStsDbQaOwnershipPar::fill(UInt_t rid)
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
    CbmStsDbQaOwnershipPar* cgd = (CbmStsDbQaOwnershipPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId       = cgd->GetCompId(); 
    fUID          = cgd->GetUID();
    fVendor       = cgd->GetVendor(); 
    fType         = cgd->GetType();         
    fReticleName  = cgd->GetReticleName(); 
    fYear         = cgd->GetYear(); 
    fOwner        = cgd->GetOwner(); 
    fLocation     = cgd->GetLocation();  
  }

}


void CbmStsDbQaOwnershipPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaOwnershipPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQAOWNERSHIPPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQAOWNERSHIPPAR").Data());
    sql_cmds.push_back(CbmStsDbQaOwnershipPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaOwnershipPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Ownership Parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaOwnershipPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaOwnershipPar::Print()
{
  
  std::cout<<"   STS QA Ownership Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   Vendor        = " << fVendor  << std::endl;
  std::cout<<"   Type          = " << fType    << std::endl;
  std::cout<<"   ReticleName   = " << fReticleName  << std::endl;
  std::cout<<"   Year          = " << fYear      << std::endl;
  std::cout<<"   Owner         = " << fOwner     << std::endl;
  std::cout<<"   Location      = " << fLocation  << std::endl;

}


Bool_t CbmStsDbQaOwnershipPar::Compare(const CbmStsDbQaOwnershipPar& that ) const {
  
  Bool_t test_h =  
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
    &&  (fVendor.compare(that.fVendor)==0)
    &&  (fType.compare(that.fType)==0)
    &&  (fReticleName.compare(that.fReticleName)==0)
    &&  (fYear.compare(that.fYear)==0)
    &&  (fOwner.compare(that.fOwner)==0)
    &&  (fLocation.compare(that.fLocation)==0);
  
  return (test_h); 
}

FairDbWriter<CbmStsDbQaOwnershipPar>* CbmStsDbQaOwnershipPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaOwnershipPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQAOWNERSHIPPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQAOWNERSHIPPAR").Data());
           sql_cmds.push_back(CbmStsDbQaOwnershipPar::GetTableDefinition());
         }
        
         // Packed SQL commands executed internally via SQL processor
         std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
         while( itr != itrEnd ) {
           std::string& sql_cmd(*itr++);
           stmtDbn->Commit(sql_cmd.c_str());
           if ( stmtDbn->PrintExceptions() ) {
                 fail = true;
                 cout << "-E- CbmStsDbQaSensorPar::ActivateWriter() ******* Error Executing SQL commands ***********  " << endl;
           }
          
         }
        
         // Refresh list of tables in connected database
         // for the choosen DB entry
         fMultConn->GetConnection(GetDbEntry())->SetTableExists();
        
         // Writer Meta-Class Instance
         fParam_Writer = GetParamWriter();
                
         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Sensor Ownership Parameter"); 
         return fParam_Writer;
        
   }
 
   return NULL;
}



