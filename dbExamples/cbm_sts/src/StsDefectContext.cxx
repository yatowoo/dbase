#include "StsDefectContext.h"

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

ClassImp(StsDefectContext);

static FairDbGenericParRegistry<StsDefectContext> sts("StsDefectContext", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsDefectContext>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsDefectContext>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsDefectContext>;

StsDefectContext::StsDefectContext(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsDefectContext>(detid, dataid, name, title, context, ownership),
  fId(0),
  fContextName(""),
  fWeight(0.)
{

}

StsDefectContext::~StsDefectContext()
{

}

void StsDefectContext::putParams(FairParamList* list)
{
  std::cout<<"-I- StsDefectContext::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("context_name", (Text_t*)&fContextName);
  list->add("weight", (Text_t*)&fWeight);
}

Bool_t StsDefectContext::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t context_name[MAX_TEXT_SIZE];
  if (!list->fill("context_name", context_name, MAX_TEXT_SIZE))  {return kFALSE;}
  fContextName = context_name;

  if (!list->fill("weight", &fWeight)) {return kFALSE;}

  return kTRUE;
}

void StsDefectContext::clear()
{
  fId = 0;
  fContextName = "";
  fWeight = 0.;
}


void StsDefectContext::Print()
{
  std::cout << "Sts Defect Type Instance:"                          << std::endl;
  std::cout << "   Defect Type Id       = "     << fId              << std::endl;
  std::cout << "   Defect Context Name  = "     << fContextName     << std::endl;
  std::cout << "   Weight               = "     << fWeight          << std::endl;
  std::cout                                                         << std::endl;
}

StsDefectContext* StsDefectContext::GetDefectContextById(Int_t contextId, UInt_t runId)
{
  StsDefectContext defcontext;
  FairDbReader<StsDefectContext> r_defcontext = *defcontext.GetParamReader();

  ValTimeStamp ts;
  if (runId)
    ts = ValTimeStamp(runId);

  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  r_defcontext.Activate(context, defcontext.GetVersion());
  return (StsDefectContext *)r_defcontext.GetRowByIndex(contextId);
}

string StsDefectContext::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO             INT NOT NULL,";
  sql += "  ROW_ID            INT NOT NULL,";
  sql += "  ID                INT NOT NULL,";
  sql += "  CONTEXT_NAME      TEXT,";
  sql += "  WEIGHT            DOUBLE,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsDefectContext::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fContextName
         >> fWeight;
}

void StsDefectContext::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fContextName
          << fWeight;
}

StsDefectContext::StsDefectContext(const StsDefectContext& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fContextName = from.fContextName;
  fWeight = from.fWeight;
}

StsDefectContext& StsDefectContext::operator=(const StsDefectContext& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fContextName = from.fContextName;
  fWeight = from.fWeight;
  
  return *this;
}