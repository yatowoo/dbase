#include "FairDbGenericParSet.h"

#include "TClass.h"
#include "TDataMember.h"
#include "TList.h"

ClassImpT(FairDbGenericParSet,T)


template<typename T>
FairDbGenericParSet<T>::FairDbGenericParSet()
: FairDbParSet("","","",kFALSE)
  ,fDet_id(FairDbDetector::kUnknown)
  ,fData_id(DataType::kUnknown)
  ,fTableName("")  
  ,fCompId(-1)
  ,fParam_Writer(NULL) //  Writer Meta-Class for SQL IO
  ,fParam_Reader(NULL) //  Reader Meta-Class for SQL IO
  ,fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)  
{
  // Set the default Db Entry to the first slot
  SetDbEntry(0);  
  // No  Aggregation
  SetCompId(fCompId);
  // Init Version 
  SetVersion(0);
}

template<typename T>
FairDbGenericParSet<T>::FairDbGenericParSet(const FairDbDetector::Detector_t detid, const DataType::DataType_t dataid,
                                            const char* name,const char* title,const char* context, Bool_t ownership)
  : FairDbParSet(name,title,context,ownership)
  ,fDet_id(detid)
  ,fData_id(dataid)
  ,fTableName(name)  
  ,fCompId(-1)
  ,fParam_Writer(NULL) //  Writer Meta-Class for SQL IO
  ,fParam_Reader(NULL) //  Reader Meta-Class for SQL IO
  ,fMultConn(FairDbTableInterfaceStore::Instance().fConnectionPool)  
{
  // Set the default Db Entry to the first slot
  SetDbEntry(0);  
  // No  Aggregation
  SetCompId(fCompId);
  // Init Version 
  SetVersion(0);
  // Log Comment
  SetLogTitle(T::Class()->GetName());
}


template<typename T>
FairDbGenericParSet<T>::FairDbGenericParSet(const FairDbGenericParSet& from)
  : FairDbParSet(from)
{
}

template<typename T>
FairDbGenericParSet<T>& FairDbGenericParSet<T>::operator=(const FairDbGenericParSet& from)
{
  if (this == &from) { return *this; }

  FairDbParSet::operator=(from);
 
  return *this; 
}  


template<typename T>
FairDbGenericParSet<T>::~FairDbGenericParSet()
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

template<typename T>
void FairDbGenericParSet<T>::clear()
{
  if (fParam_Writer) { fParam_Writer->Reset(); fParam_Writer=nullptr; }
  if (fParam_Reader) { fParam_Reader->Reset(); fParam_Reader=nullptr; }  
}


template<typename T>
void FairDbGenericParSet<T>::CreateDbTable(Int_t db_entry)
{
  // Set new DB Entry
  SetDbEntry(db_entry);
  // Boolean IO test variable
  Bool_t fail= kFALSE;
  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::CreateDbTable()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }
  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetTableName());
  atr.ToUpper();
  
  if (! fMultConn->GetConnection(GetDbEntry())->TableExists(atr.Data()) ) {
    sql_cmds.push_back(FairDb::GetValDefinition(atr.Data()).Data());
    sql_cmds.push_back(GetTableDefinition(atr.Data()));
  }
  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::CreateDbTable() * Error Executing SQL commands **  " << endl;
      exit(1);
    }
  }
  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(GetDbEntry())->SetTableExists();  
}

template<typename T>
void FairDbGenericParSet<T>::fill(UInt_t rid)
{
  // Get Reader Meta Class
  fParam_Reader=GetParamReader();
  
  // Get Context associated to par. T
  ValCondition context = GetContext(rid);
  
  // Activate reading for this Context
  fParam_Reader->Activate(context, GetVersion());
  
  // By default use the latest row entry
  // (Other rows would correspond to outdated versions)
  Int_t numRows = fParam_Reader->GetNumRows();
  if ( numRows > 1 ) { numRows = 1; }
  
  for (int i = 0; i < numRows; ++i) {
    T* cgd = (T*) fParam_Reader->GetRow(i);
    if (!cgd) { continue; }
    
    // !! <DB> date: 2 nov 2016
    // if ptr(cgd) exist the do cast and
    // self assign the object (close to std::swap)    
    // This assume that = operator and copy ctor
    // are defined in the parameter class.
    
    T par = (*cgd);
    T* copy = (T*) this;
    *copy = par;    
  }
  
}

template<typename T>
TObjArray* FairDbGenericParSet<T>::FillArray(Int_t compId, UInt_t rid)
{
  T instance;
  FairDbReader<T> paramReader;

  paramReader.Activate(instance.GetContext(rid), instance.GetVersion());
  Int_t numRows = paramReader.GetNumRows();
  if (!numRows)
    return NULL;

  TObjArray* result = new TObjArray(numRows);
  for (Int_t i=0; i < numRows; i++)
  {
    T *inst = (T*)paramReader.GetRow(i);
    if (!inst)
      continue;

    // Read grouped
    if (compId != -1)
    {
      const FairDbValRecord *valRecord = paramReader.GetValidityRec(inst);
      if (valRecord->GetAggregateNo() != compId)
        continue;
    }
    result->Add(inst);
  }

  return result;
}

template<typename T>
void FairDbGenericParSet<T>::store(UInt_t rid)
{
  // Boolean IO test variable
  Bool_t fail= kFALSE;
  // Create a unique statement on choosen DB entry
  auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
  if ( ! stmtDbn.get() ) {
    DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::Store()  Cannot create statement for Database_id: " << GetDbEntry()
         << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }
  // Check if for this DB entry the table already exists.
  // If not call the corresponding Table Definition Function
  std::vector<std::string> sql_cmds;
  TString atr(GetTableName());

  if (! fMultConn->GetConnection(GetDbEntry())->TableExists(atr.Data()) ) {
    sql_cmds.push_back(FairDb::GetValDefinition(atr.Data()).Data());
    sql_cmds.push_back(GetTableDefinition(atr.Data()));
  }
  // Packed SQL commands executed internally via SQL processor
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->Commit(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::Store() ******* Error Executing SQL commands ***********  " << endl;
      exit(1);
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
  
  fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(), atr.Data());
  // Object to Table mapping
  // DB CHECK ME !!!!!!
  T* par = (T*) (this);
  *fParam_Writer << (*par);
  // Check for eventual IO problems
  if ( !fParam_Writer->Close() ) {
    fail = true;
    DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::Store() ******** Cannot do IO on Table: " << GetTableName() <<  endl;
    exit(1);
  }
}

template <typename T>
void FairDbGenericParSet<T>::StoreArray(TObjArray *array, Int_t compId, UInt_t rid)
{
  Int_t numRows = array->GetEntries();
  if (!numRows)
    return;

  T* parSet = (T*) array->At(0);
  if (!parSet)
    return;

  // Store grouped
  if (compId != -1)
  {
    parSet->SetCompId(compId);
    FairDbWriter<T> *paramWriter = parSet->ActivateWriter(rid);
    paramWriter->SetComboNo(compId);

    for (Int_t i=0; i<numRows; i++)
    {
      parSet = (T*) array->At(i);
      if (!parSet)
        continue;

      parSet->SetCompId(compId);
      *paramWriter << *parSet;
    }

    paramWriter->Close();
  } else {
    // Store individually
    for (Int_t i=0; i<numRows; i++)
    {
      parSet = (T*) array->At(i);
      if (!parSet)
        continue;

      parSet->store(rid);
    }
  }
}

template<typename T>
FairDbWriter<T>* FairDbGenericParSet<T>::ActivateWriter(Int_t rid)
{
  // delete if already existing
  if (fParam_Writer) { delete fParam_Writer; fParam_Writer=NULL; }
  else {
    // Create according to IoV
    Bool_t fail= kFALSE;
    // Create a unique statement on choosen DB entry
    auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(GetDbEntry()));
    if ( ! stmtDbn.get() ) {
      DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::ActivateWriter()  Cannot create statement for Database_id: " << GetDbEntry()
           << "\n    Please check the FAIRDB_TSQL_* environment.  Quitting ... " << endl;
      exit(1);
    }
    // Check if for this DB entry the table already exists.
    // If not call the corresponding Table Definition Function
    std::vector<std::string> sql_cmds;
    TString atr(GetTableName());

    if (! fMultConn->GetConnection(GetDbEntry())->TableExists(atr.Data()) ) {
      sql_cmds.push_back(FairDb::GetValDefinition(atr.Data()).Data());
      sql_cmds.push_back(GetTableDefinition(atr.Data()));
    }

    // Packed SQL commands executed internally via SQL processor
    std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
    while( itr != itrEnd ) {
      std::string& sql_cmd(*itr++);
      stmtDbn->Commit(sql_cmd.c_str());
      if ( stmtDbn->PrintExceptions() ) {
        fail = true;
        TString cl(ClassName());
        DBLOG("FairDb",FairDbLog::kFatal) << "FairDbGenericParSet::ActivateWriter() ******* Error Executing SQL commands ***********  " << endl;
        exit(1);
      }
    }

    // Refresh list of tables in connected database
    // for the choosen DB entry
    fMultConn->GetConnection(GetDbEntry())->SetTableExists();

    // Writer Meta-Class Instance
    fParam_Writer = GetParamWriter();
    fParam_Writer->Activate(GetValInterval(rid),GetComboNo(), GetVersion(),GetDbEntry(),atr.Data()); 
    return fParam_Writer;
  }
  return NULL;
}

