#include "StsDefectType.h"

#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairDbOutTableBuffer.h"         // for FairDbOutRowStream
#include "FairDbStatement.h"              // for FairDbStatement
#include "FairParamList.h"                // for FairParamList
#include "FairDbParFactory.h"

#include "Riosfwd.h"                     // for ostream
#include "TString.h"                     // for TString

#include <stdlib.h>                      // for exit
#include <memory>                        // for auto_ptr, etc
#include <vector>                        // for vector, vector<>::iterator

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/progress.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;
using namespace boost;
using boost::lexical_cast;
using boost::bad_lexical_cast;


#define MAX_TEXT_SIZE 255

ClassImp(StsDefectType);

static FairDbGenericParRegistry<StsDefectType> sts("StsDefectType", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsDefectType>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsDefectType>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsDefectType>;

StsDefectType::StsDefectType(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsDefectType>(detid, dataid, name, title, context, ownership),
  fId(0),
  fTypeName(""),
  fWeight(0.)
{

}

StsDefectType::~StsDefectType()
{

}

void StsDefectType::putParams(FairParamList* list)
{
  std::cout<<"-I- StsDefectType::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("type_name", (Text_t*)&fTypeName);
  list->add("weight", (Text_t*)&fWeight);
}

Bool_t StsDefectType::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t type_name[MAX_TEXT_SIZE];
  if (!list->fill("type_name", type_name, MAX_TEXT_SIZE))  {return kFALSE;}
  fTypeName = type_name;

  if (!list->fill("weight", &fWeight)) {return kFALSE;}

  return kTRUE;
}

void StsDefectType::clear()
{
  fId = 0;
  fTypeName = "";
  fWeight = 0.;
}


void StsDefectType::Print()
{
  std::cout << "Sts Defect Type Instance:"                          << std::endl;
  std::cout << "   Defect Type Id       = "     << fId              << std::endl;
  std::cout << "   Defect Type Name     = "     << fTypeName        << std::endl;
  std::cout << "   Weight               = "     << fWeight          << std::endl;
  std::cout                                                         << std::endl;
}

string StsDefectType::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO             INT NOT NULL,";
  sql += "  ROW_ID            INT NOT NULL,";
  sql += "  ID                INT NOT NULL,";
  sql += "  TYPE_NAME         TEXT,";
  sql += "  WEIGHT            DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsDefectType::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fTypeName
         >> fWeight;
}

void StsDefectType::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fTypeName
          << fWeight;
}

StsDefectType::StsDefectType(const StsDefectType& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fTypeName = from.fTypeName;
  fWeight = from.fWeight;
}

StsDefectType& StsDefectType::operator=(const StsDefectType& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fTypeName = from.fTypeName;
  fWeight = from.fWeight;
  
  return *this;
}