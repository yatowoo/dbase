#include "FairDbExPar2.h"
#include "FairParamList.h"              // for FairParamList

using namespace std;

ClassImp(FairDbExPar2);

static FairDbGenericParRegistry<FairDbExPar2> qa_iv("FairDbExPar2", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<FairDbExPar2>;


#include "FairDbReader.tpl"
template class  FairDbReader<FairDbExPar2>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<FairDbExPar2>;



FairDbExPar2::FairDbExPar2(const FairDbDetector::Detector_t detid, const DataType::DataType_t dataid,
                           const char* name, const char* title, const char* context, Bool_t own)
  : FairDbGenericParSet<FairDbExPar2>(detid, dataid,name,title,context, own),
    fIdSystem(0),
    fData(0.)
{
}


FairDbExPar2::~FairDbExPar2()
{

}

void FairDbExPar2::putParams(FairParamList* list)
{
  std::cout<<"-I- FairDbExPar2::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("comp_id",    fCompId);
  list->add("id_system",  fIdSystem);
  list->add("data",      fData);
}

Bool_t FairDbExPar2::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("comp_id",   &fCompId)) { return kFALSE; }
  if (!list->fill("id_system", &fIdSystem)) { return kFALSE; }
  if (!list->fill("data",     &fData))    { return kFALSE; }
  return kTRUE;
}

void FairDbExPar2::clear()
{
  fIdSystem=0;
  fData=0.;

  FairDbGenericParSet<FairDbExPar2>::clear();
}


string FairDbExPar2::GetTableDefinition(const char* Name)
{

  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  COMP_ID        INT,";
  sql += "  IDSYSTEM       INT,";
  sql += "  DATA           DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void FairDbExPar2::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fCompId  >> fIdSystem >> fData;
}

void FairDbExPar2::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fCompId  << fIdSystem  << fData;
}


void FairDbExPar2::Print()
{
  std::cout<<"   ExPar2   Parameters: " <<std::endl;
  std::cout<<"   CompId                 = "<< GetCompId() << std::endl;
  std::cout<<"   Id System              = "<< fIdSystem <<std::endl;
  std::cout<<"   Data                   = "<< fData <<std::endl;
}

FairDbExPar2::FairDbExPar2(const FairDbExPar2& from){
  fCompId    = from.GetCompId();
  fIdSystem  = from.fIdSystem;
  fData      = from.fData;

}

FairDbExPar2& FairDbExPar2::operator=(const FairDbExPar2& from){
  fCompId    = from.GetCompId();
  fIdSystem  = from.fIdSystem;
  fData      = from.fData;
  return *this;
}

