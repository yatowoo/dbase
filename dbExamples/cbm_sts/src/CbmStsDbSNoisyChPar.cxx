#include "CbmStsDbSNoisyChPar.h"

#include "FairParamList.h"
#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairUtilStream.h"
#include "FairDbOutTableBuffer.h"
#include "FairDbStatement.h"
#include "FairDbStreamer.h"
#include "FairDbParFactory.h"

#include <iostream>

using std::cout;
using std::endl;


ClassImp(CbmStsDbSNoisyChPar);

static FairDbParRegistry<CbmStsDbSNoisyChPar> qa_iv("StsDbSNoisyChPar");

#include "FairDbReader.tpl"
template class  FairDbReader<CbmStsDbSNoisyChPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<CbmStsDbSNoisyChPar>;



CbmStsDbSNoisyChPar::CbmStsDbSNoisyChPar(const char* name, const char* title, const char* context,Bool_t own)
  : FairDbParSet(name,title,context,own)
    ,fCompId(-1)
    ,fDetId(-1)
    ,fNChannels(NCHANNELS)
    ,fParam_Writer(nullptr) //  Writer Meta-Class for SQL IO
    ,fParam_Reader(nullptr) //  Reader Meta-Class for SQL IO
    ,fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  // Constructor
  for(Int_t i=0;i<NSIDE;i++) fNoisyCh[i] = new TBits(NCHANNELS);
  // Set the default Db Entry to the first slot
  SetDbEntry(0);
  // No  Aggregation
  SetCompId(fCompId);
  // Init Version
  SetVersion(0);
}

CbmStsDbSNoisyChPar::~CbmStsDbSNoisyChPar()
{
  for(Int_t i=0;i<NSIDE;i++)
  if (fNoisyCh[i]) { delete fNoisyCh[i]; fNoisyCh[i]=nullptr; }

  if (fParam_Writer) {
    delete fParam_Writer;
    fParam_Writer=nullptr;
  }

  if (fParam_Reader) {
    delete fParam_Reader;
    fParam_Reader=nullptr;
  }
}

string CbmStsDbSNoisyChPar::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += "STSDBSNOISYCHPAR"; }
  sql += "( SEQNO            INT NOT NULL,";
  sql += "  ROW_ID           INT NOT NULL,";
  sql += "  DETID            INT,";
  sql += "  NCH              INT,";
  sql += "  NOISYCH_N        TEXT,";
  sql += "  NOISYCH_P        TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void CbmStsDbSNoisyChPar::putParams(FairParamList* list)
{
  std::cout<<"-I- CbmStsDbSNoisyChPar::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("det_id",  fDetId);
  list->add("n_ch",  fNChannels);
  list->addObject("NoisyCh_n", fNoisyCh[0]);
  list->addObject("NoisyCh_p", fNoisyCh[1]);
}

Bool_t CbmStsDbSNoisyChPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("det_id", &fDetId)) { return kFALSE; }
  if (!list->fill("n_ch", &fNChannels)) { return kFALSE; }
  if (!list->fillObject("NoisyCh_n", fNoisyCh[0])) {return kFALSE;}
  if (!list->fillObject("NoisyCh_p", fNoisyCh[1])) {return kFALSE;}
  return kTRUE;
}

void CbmStsDbSNoisyChPar::clear()
{
  fDetId=fNChannels=-1;
  for(Int_t i=0;i<NSIDE;i++){
  if (fNoisyCh[i])
      fNoisyCh[i]->ResetAllBits(kFALSE);
 }
}

void CbmStsDbSNoisyChPar::Fill(FairDbResultPool& res_in,
                           const FairDbValRecord* valrec)
{
  // Instanciate  & clear() the Data Contents
  clear();
  FairDbStreamer noisyCh_n(fNoisyCh[0]);
  FairDbStreamer noisyCh_p(fNoisyCh[1]);
  res_in >> fDetId >> fNChannels >> noisyCh_n >> noisyCh_p;
  noisyCh_n.Fill(fNoisyCh[0]);
  noisyCh_p.Fill(fNoisyCh[1]);
}

void CbmStsDbSNoisyChPar::Store(FairDbOutTableBuffer& res_out,
                            const FairDbValRecord* valrec) const
{
  FairDbStreamer noisyCh_n(fNoisyCh[0]);
  FairDbStreamer noisyCh_p(fNoisyCh[1]);
  res_out << fDetId  << fNChannels << noisyCh_n << noisyCh_p;
}

void CbmStsDbSNoisyChPar::fill(UInt_t rid)
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
  // Just use the latest row entry
  if ( numRows > 1 ) { numRows = 1; }

  for (Int_t j = 0; j < numRows; ++j) {
    CbmStsDbSNoisyChPar* cgd = (CbmStsDbSNoisyChPar*) fParam_Reader->GetRow(j);
    if (!cgd) { continue; }
    fDetId = cgd->GetDetId();
    fNChannels =  cgd->GetNChannels();
    for(Int_t ch=0;ch<NSIDE;ch++)
    fNoisyCh[ch] = cgd->GetNoisyChMap(ch);
  }
}


void CbmStsDbSNoisyChPar::store(UInt_t rid)
{

  DBLOG("FairDb", FairDbLog::kWarning) << "CbmStsDbSNoisyChPar Store() " << endl;

  // Boolean IO test variable
  Bool_t fail= kFALSE;

  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    cout << "-E-  CbmStsDbSNoisyChPar::Store()  Cannot create a statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSDBSNOISYCHPAR") ) {
    sql_cmds.push_back(FairDb::GetValDefinition("STSDBSNOISYCHPAR").Data());
    sql_cmds.push_back(CbmStsDbSNoisyChPar::GetTableDefinition());
  }

  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      cout << "-E- CbmStsDbSNoisyChPar::Store() ******* Error Executing SQL commands ***********  " << endl;
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

  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),GetLogTitle());

  // Object to Table mapping
  *fParam_Writer<< (*this);

  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    cout << "-E- FairDbTutPar::Store() ******** Cannot do IO on class: " << GetName() <<  endl;
  }

}

FairDbWriter<CbmStsDbSNoisyChPar>* CbmStsDbSNoisyChPar::ActivateWriter(Int_t rid)
{
   // delete if already existing
   if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }

   else {
         // Create according to IoV
         Bool_t fail= kFALSE;

         // Create a unique statement on choosen DB entry
         auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
         if ( ! stmtDbn.get() ) {
           cout << "-E-  CbmStsDbSNoisyChPar::Store()  Cannot create statement for Database_id: " << GetDbEntry()
                    << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
           exit(1);
         }

         // Check if for this DB entry the table already exists.
         // If not call the corresponding Table Definition Function
         std::vector<std::string> sql_cmds;
         TString atr(GetName());
         atr.ToUpper();

         if (! fMultConn->GetConnection(GetDbEntry())->TableExists("STSDBSNOISYCHPAR") ) {
           sql_cmds.push_back(FairDb::GetValDefinition("STSDBSNOISYCHPAR").Data());
           sql_cmds.push_back(CbmStsDbSNoisyChPar::GetTableDefinition());
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

         fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),"STS DB Noisy Channels Parameter");
         return fParam_Writer;

   }

   return NULL;
}

void CbmStsDbSNoisyChPar::FillDummy()
{

}

void CbmStsDbSNoisyChPar::Print()
{

  std::cout<<" ---------------------------------"<<std::endl;
  std::cout<<"    DetId        = "    << fDetId     << std::endl;
  std::cout<<"    NChannels    = "    << fNChannels << std::endl;
  std::cout<<"    Nr of NoisyChannels (n) = " << fNoisyCh[0]->CountBits() <<std::endl;
  std::cout<<"    Nr of NoisyChannels (p) = " << fNoisyCh[1]->CountBits() <<std::endl;
  // print the bit FairDbWtFileTreeTableNode
  for(Int_t i=0;i<NSIDE;i++)
  fNoisyCh[i]->Print();
}
