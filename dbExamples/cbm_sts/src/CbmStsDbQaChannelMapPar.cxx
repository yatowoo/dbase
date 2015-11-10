#include "CbmStsDbQaChannelMapPar.h"

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

ClassImp(CbmStsDbQaChannelMapPar);


static FairDbParRegistry<CbmStsDbQaChannelMapPar> qa_iv("CbmStsDbQaChannelMapPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbQaChannelMapPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbQaChannelMapPar>;



CbmStsDbQaChannelMapPar::CbmStsDbQaChannelMapPar(const char* name, const char* title, const char* context, Bool_t own)
  : FairDbParSet(name,title,context, own),
    fCompId(-1),
    fUID(0),
    fAC_Cap_Ok_pside(0),
    fAC_Cap_Ok_nside(0),
    fAC_Cap_Value_pside(0.),
    fAC_Cap_Value_nside(0.),
    fI_Strip_pside(0.),
    fI_Strip_nside(0.),
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


CbmStsDbQaChannelMapPar::~CbmStsDbQaChannelMapPar()
{
 
    fAC_Cap_Ok_pside = 0;
    fAC_Cap_Ok_nside = 0;
    fAC_Cap_Value_pside = 0.;
    fAC_Cap_Value_nside = 0.;
    fI_Strip_pside = 0.;
    fI_Strip_nside = 0.;
    
  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=NULL;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=NULL;
  }

}


void CbmStsDbQaChannelMapPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbQaChannelMapPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",  fCompId);
  list->add("s_uid",    fUID);

  list->add("ac_cap_ok_pside", fAC_Cap_Ok_pside);
  list->add("ac_cap_ok_nside", fAC_Cap_Ok_nside);
  list->add("ac_cap_value_pside", fAC_Cap_Value_pside);
  list->add("ac_cap_value_nside", fAC_Cap_Value_nside);
  list->add("i_strip_pside", fI_Strip_pside);
  list->add("i_strip_nside", fI_Strip_nside);
}

Bool_t CbmStsDbQaChannelMapPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id",     &fCompId)) { return kFALSE; }
  if (!list->fill("s_uid",       &fUID))    { return kFALSE; }

  if(!list->fill("ac_cap_ok_pside", &fAC_Cap_Ok_pside))       { return kFALSE; }
  if(!list->fill("ac_cap_ok_nside", &fAC_Cap_Ok_nside))       { return kFALSE; } 
  if(!list->fill("ac_cap_value_pside", &fAC_Cap_Value_pside)) { return kFALSE; }
  if(!list->fill("ac_cap_value_nside", &fAC_Cap_Value_nside)) { return kFALSE; }
  if(!list->fill("i_strip_pside", &fI_Strip_pside))           { return kFALSE; }
  if(!list->fill("i_strip_nside", &fI_Strip_nside))           { return kFALSE; }

  return kTRUE;
}

void CbmStsDbQaChannelMapPar::clear()
{


  fCompId=fUID=0;
  
  fAC_Cap_Ok_pside = 0;
  fAC_Cap_Ok_nside = 0;
  fAC_Cap_Value_pside = 0.;
  fAC_Cap_Value_nside = 0.;
  fI_Strip_pside = 0.;
  fI_Strip_nside = 0.;


  if (fParam_Writer) { fParam_Writer->Reset(); }
  if (fParam_Reader) { fParam_Reader->Reset(); }
}


string CbmStsDbQaChannelMapPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "CBMSTSDBQACHANNELMAPPAR"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  COMP_ID        INT,";
  sql += "  UID            INT,";
  sql += "  AC_CAP_OK_PSIDE             INT,";
  sql += "  AC_CAP_OK_NSIDE             INT,";
  sql += "  AC_CAP_VAL_PSIDE            DOUBLE,";
  sql += "  AC_CAP_VAL_NSIDE            DOUBLE,";
  sql += "  I_STRIP_PSIDE               DOUBLE,";
  sql += "  I_STRIP_NSIDE               DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbQaChannelMapPar::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  
   res_in >> fCompId >> fUID 
          >> fAC_Cap_Ok_pside
          >> fAC_Cap_Ok_nside
          >> fAC_Cap_Value_pside
          >> fAC_Cap_Value_nside 
          >> fI_Strip_pside
          >> fI_Strip_nside;
  
}

void CbmStsDbQaChannelMapPar::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{ 
  res_out << fCompId << fUID 
          << fAC_Cap_Ok_pside
          << fAC_Cap_Ok_nside
          << fAC_Cap_Value_pside
          << fAC_Cap_Value_nside 
          << fI_Strip_pside
          << fI_Strip_nside;
}


void CbmStsDbQaChannelMapPar::fill(UInt_t rid)
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
    CbmStsDbQaChannelMapPar* cgd = (CbmStsDbQaChannelMapPar*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    fCompId = cgd->GetCompId(); 
    fUID    = cgd->GetUID();

    fAC_Cap_Ok_pside = cgd->GetAC_Cap_Ok_pside();   
    fAC_Cap_Ok_nside = cgd->GetAC_Cap_Ok_nside();    
    fAC_Cap_Value_pside = cgd->GetAC_Cap_Value_pside();
    fAC_Cap_Value_nside = cgd->GetAC_Cap_Value_nside();
    fI_Strip_pside = cgd->GetI_Strip_pside();
    fI_Strip_nside = cgd->GetI_Strip_nside(); 

  }

}


void CbmStsDbQaChannelMapPar::store(UInt_t rid)
{

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbQaChannelMapPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQACHANNELMAPPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQACHANNELMAPPAR").Data());
    sql_cmds.push_back(CbmStsDbQaChannelMapPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbQaChannelMapPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Channel Map parameter");

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- CbmStsDbQaChannelMapPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

void CbmStsDbQaChannelMapPar::Print()
{

  std::cout<<"   STS QA Channel Map Paramters <UID> "<< fUID <<  " <comp_Id> " << fCompId << std::endl;
  std::cout<<"   AC_CAP_OK_PSIDE    = " << fAC_Cap_Ok_pside << std::endl;
  std::cout<<"   AC_CAP_OK_NSIDE    = " << fAC_Cap_Ok_nside << std::endl;
  std::cout<<"   AC_CAP_VAL_PSIDE   = " << fAC_Cap_Value_pside << std::endl;
  std::cout<<"   AC_CAP_VAL_NSIDE   = " << fAC_Cap_Value_nside << std::endl;
  std::cout<<"   I_STRIP_PSIDE      = " << fI_Strip_pside << std::endl;
  std::cout<<"   I_STRIP_NSIDE      = " << fI_Strip_nside << std::endl;

}



Bool_t CbmStsDbQaChannelMapPar::Compare(const CbmStsDbQaChannelMapPar& that ) const {
  
  Bool_t test_h =  
	      (fUID      == that.fUID)
	  &&  (fCompId   == that.fCompId)
    &&  (fAC_Cap_Ok_pside == that.fAC_Cap_Ok_pside)
    &&  (fAC_Cap_Ok_nside == that.fAC_Cap_Ok_nside)
    &&  (fAC_Cap_Value_pside == that.fAC_Cap_Value_pside)
    &&  (fAC_Cap_Value_nside == that.fAC_Cap_Value_nside)
    &&  (fI_Strip_pside == that.fI_Strip_pside)
    &&  (fI_Strip_nside == that.fI_Strip_nside);
  
  return ( test_h ); 
}

FairDbWriter<CbmStsDbQaChannelMapPar>* CbmStsDbQaChannelMapPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
 
   else {
         // Create according to IoV
         Bool_t fail= kFALSE;
        
         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbQaChannelMapPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }
        
         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();
        
         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("CBMSTSDBQACHANNELMAPPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("CBMSTSDBQACHANNELMAPPAR").Data());
           sql_cmds.push_back(CbmStsDbQaChannelMapPar::GetTableDefinition());
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
                
         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS QA Channel Map Parameter"); 
         return fParam_Writer;
        
   }
 
   return NULL;
}



