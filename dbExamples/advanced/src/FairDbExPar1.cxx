#include "FairDbExPar1.h"
#include "FairParamList.h"              // for FairParamList

using namespace std;

ClassImp(FairDbExPar1);

static FairDbGenericParRegistry<FairDbExPar1> qa_iv("FairDbExPar1", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<FairDbExPar1>;

#include "FairDbReader.tpl"
template class  FairDbReader<FairDbExPar1>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<FairDbExPar1>;

FairDbExPar1::FairDbExPar1(const FairDbDetector::Detector_t detid, const DataType::DataType_t dataid,
                           const char* name, const char* title, const char* context, Bool_t own)
  : FairDbGenericParSet<FairDbExPar1>(detid, dataid,name,title,context, own),
    fIdSystem(0),
    fPedestal(0.),
    fGain1(0.),
    fGain2(0.)
{
}

FairDbExPar1::~FairDbExPar1()
{
}

void FairDbExPar1::putParams(FairParamList* list)
{
  if(!list) { return; }
  list->add("id_system",  fIdSystem);
  list->add("pedestal",   fPedestal);
  list->add("gain1",      fGain1);
  list->add("gain2",      fGain2);
}

Bool_t FairDbExPar1::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("id_system", &fIdSystem)) { return kFALSE; }
  if (!list->fill("pedestal",  &fPedestal)) { return kFALSE; }
  if (!list->fill("gain1",     &fGain1))    { return kFALSE; }
  if (!list->fill("gain2",     &fGain1))    { return kFALSE; }
  return kTRUE;
}

void FairDbExPar1::clear()
{
  fIdSystem=0;
  fPedestal=fGain1=fGain2=0;

  FairDbGenericParSet<FairDbExPar1>::clear();
}

string FairDbExPar1::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  IDSYSTEM       INT,";
  sql += "  PEDESTAL       DOUBLE,";
  sql += "  GAIN1          DOUBLE,";
  sql += "  GAIN2          DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}

void FairDbExPar1::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fIdSystem  >> fPedestal  >> fGain1  >> fGain2;
}

void FairDbExPar1::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fIdSystem  << fPedestal   << fGain1  << fGain2;
}

void FairDbExPar1::Print()
{
  std::cout<<"   ExPar1   Parameters: " <<std::endl;
  std::cout<<"   Id System              = "<<fIdSystem<<std::endl;
  std::cout<<"   Pedestal               = "<<fPedestal<< std::endl;
  std::cout<<"   Gain1                  = "<<fGain1<<std::endl;
  std::cout<<"   Gain2                  = "<<fGain2<<std::endl;
}



FairDbExPar1::FairDbExPar1(const FairDbExPar1& from){
  fCompId    = from.GetCompId();
  fIdSystem  = from.fIdSystem;
  fPedestal = from.fPedestal;
  fGain1 = from. fGain1;
  fGain2 = from.fGain2;
}

FairDbExPar1& FairDbExPar1::operator=(const FairDbExPar1& from){
  fCompId    = from.GetCompId();
  fIdSystem  = from.fIdSystem;
  fPedestal = from.fPedestal;
  fGain1 = from. fGain1;
  fGain2 = from.fGain2;
  return *this;
}



