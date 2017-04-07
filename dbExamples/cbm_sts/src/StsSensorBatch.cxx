#include "StsSensorBatch.h"
#include "StsSensor.h"

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

ClassImp(StsSensorBatch);

static FairDbGenericParRegistry<StsSensorBatch> sts("StsSensorBatch", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsSensorBatch>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsSensorBatch>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsSensorBatch>;

StsSensorBatch::StsSensorBatch(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsSensorBatch>(detid, dataid, name, title, context, ownership),
  fId(0),
  fNumber(""),
  fDate(ValTimeStamp::GetBOT()),
  fComment("")
{
  fSensors = NULL;
}

StsSensorBatch::~StsSensorBatch()
{
  delete fSensors;
}

void StsSensorBatch::putParams(FairParamList* list)
{
  std::cout<<"-I- StsSensorBatch::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("number", (Text_t*)&fNumber);
  list->add("date",   FairDb::MakeDateTimeString(fDate).Data());
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsSensorBatch::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t number[MAX_TEXT_SIZE];
  if (!list->fill("number", number, MAX_TEXT_SIZE))  {return kFALSE;}
  fNumber = number;

  Text_t date[MAX_TEXT_SIZE];
  if (!list->fill("date", date, MAX_TEXT_SIZE)) {return kFALSE;}
  fDate = FairDb::MakeTimeStamp(date);

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsSensorBatch::clear()
{
  fId = 0;
  fNumber = "";
  fDate = ValTimeStamp::GetBOT();
  fComment = "";

  delete fSensors;
  fSensors = NULL;
}


void StsSensorBatch::Print()
{
  std::cout << "Sts Sensor Batch Instance:"                         << std::endl;
  std::cout << "   Sensor Batch Id       = "     << fId             << std::endl;
  std::cout << "   Batch Number          = "     << fNumber         << std::endl;
  std::cout << "   Date                  = "     << fDate           << std::endl;
  std::cout << "   Comment               = "     << fComment        << std::endl;
  std::cout                                                         << std::endl;
}

StsSensorBatch* StsSensorBatch::GetBatchById(Int_t batchId, UInt_t runId)
{
  StsSensorBatch batch;
  FairDbReader<StsSensorBatch> r_batch = *batch.GetParamReader();

  ValTimeStamp ts;
  if (runId)
    ts = ValTimeStamp(runId);

  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  r_batch.Activate(context, batch.GetVersion());
  return (StsSensorBatch *)r_batch.GetRowByIndex(batchId);
}

TObjArray* StsSensorBatch::GetSensors()
{
  if (!fSensors)
  {
    fSensors = StsSensor::GetSensors(fId);
  }

  return fSensors;
}

string StsSensorBatch::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  ID              INT NOT NULL,";
  sql += "  NUMBER          TEXT,";
  sql += "  DATE            DATETIME,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsSensorBatch::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in  >> fId
          >> fNumber
          >> fDate
          >> fComment;
}

void StsSensorBatch::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fNumber
          << fDate
          << fComment;
}

StsSensorBatch::StsSensorBatch(const StsSensorBatch& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fNumber = from.fNumber;
  fDate = from.fDate;
  fComment = from.fComment;
}

StsSensorBatch& StsSensorBatch::operator=(const StsSensorBatch& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fNumber = from.fNumber;
  fDate = from.fDate;
  fComment = from.fComment;

  return *this;
}