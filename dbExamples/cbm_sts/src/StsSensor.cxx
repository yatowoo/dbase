#include "StsSensor.h"
#include "StsOpticalInspection.h"

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

ClassImp(StsSensor);

static FairDbGenericParRegistry<StsSensor> sts("StsSensor", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsSensor>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsSensor>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsSensor>;


StsSensor::StsSensor(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsSensor>(detid, dataid, name, title, context, ownership),
  fId(0),
  fBatchId(0),
  fLocationId(0),
  fTypeId(0),
  fVendorId(0),
  fReticleName(""),
  fWaferNumber(0),
  fDate(ValTimeStamp::GetBOT()),
  fQuality(0.),
  fQualityGrade(""),
  fComment("")
{
  fOpticalInspections = NULL;
  fBatch = NULL;
  fLocation = NULL;
  fType = NULL;
  fVendor = NULL;
}

StsSensor::~StsSensor()
{
  delete fOpticalInspections;
  delete fBatch;
  delete fLocation;
  delete fType;
  delete fVendor;
}

void StsSensor::putParams(FairParamList* list)
{
  std::cout<<"-I- StsSensor::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("batch_id", fBatchId);
  list->add("location_id", fLocationId);
  list->add("type_id", fTypeId);
  list->add("vendor_id", fVendorId);
  list->add("reticle_name", (Text_t*)&fReticleName);
  list->add("wafer_number", fWaferNumber);
  list->add("date", FairDb::MakeDateTimeString(fDate).Data());
  list->add("quality", fQuality);
  list->add("quality_grade", (Text_t*)&fQualityGrade);
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsSensor::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}
  if (!list->fill("batch_id", &fBatchId))  {return kFALSE;}
  if (!list->fill("location_id", &fLocationId))  {return kFALSE;}
  if (!list->fill("type_id", &fTypeId))  {return kFALSE;}
  if (!list->fill("vendor_id", &fVendorId))  {return kFALSE;}

  Text_t reticle_name[MAX_TEXT_SIZE];
  if (!list->fill("reticle_name", reticle_name, MAX_TEXT_SIZE)) {return kFALSE;}
  fReticleName = reticle_name;

  if (!list->fill("wafer_number", &fWaferNumber)) {return kFALSE;}
  
  Text_t date[MAX_TEXT_SIZE];
  if (!list->fill("date", date, MAX_TEXT_SIZE)) {return kFALSE;}
  fDate = FairDb::MakeTimeStamp(date);
  
  if (!list->fill("quality", &fQuality)) {return kFALSE;}

  Text_t quality_grade[MAX_TEXT_SIZE];
  if (!list->fill("quality_grade", quality_grade, MAX_TEXT_SIZE)) {return kFALSE;}
  fQualityGrade = quality_grade;

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsSensor::clear()
{
  fId = 0;
  fBatchId = 0;
  fLocationId = 0;
  fTypeId = 0;
  fVendorId = 0;
  fReticleName = "";
  fWaferNumber = 0;
  fDate = ValTimeStamp::GetBOT();
  fQuality = 0.;
  fQualityGrade = "";
  fComment = "";

  delete fOpticalInspections;
  fOpticalInspections = NULL;

  delete fBatch;
  fBatch = NULL;

  delete fLocation;
  fLocation = NULL;

  delete fType;
  fType = NULL;

  delete fVendor;
  fVendor = NULL;
}


void StsSensor::Print()
{
  std::cout << "Sts Sensor Instance:"                       << std::endl;
  std::cout << "   Sensor Id     = "     << fId             << std::endl;
  std::cout << "   Batch Id      = "     << fBatchId        << std::endl;
  std::cout << "   Location Id   = "     << fLocationId     << std::endl;
  std::cout << "   Type Id       = "     << fTypeId         << std::endl;
  std::cout << "   Vendor Id     = "     << fVendorId       << std::endl;
  std::cout << "   Reticle name  = "     << fReticleName    << std::endl;
  std::cout << "   Wafer Number  = "     << fWaferNumber    << std::endl;
  std::cout << "   Date          = "     << fDate           << std::endl;
  std::cout << "   Quality       = "     << fQuality        << std::endl;
  std::cout << "   Quality Grade = "     << fQualityGrade   << std::endl;
  std::cout << "   Comment       = "     << fComment        << std::endl;
  std::cout                                                 << std::endl;
}

TObjArray* StsSensor::GetOpticalInspections()
{
  if (!fOpticalInspections) fOpticalInspections = StsOpticalInspection::GetInspections(fId);
  return fOpticalInspections;
}

string StsSensor::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  ID              INT,";
  sql += "  BATCH_ID        INT,";
  sql += "  LOCATION_ID     INT,";
  sql += "  TYPE_ID         INT,";
  sql += "  VENDOR_ID       INT,";
  sql += "  RETICLE_NAME    TEXT,";
  sql += "  WAFER_NUMBER    INT,";
  sql += "  DATE            DATETIME,";
  sql += "  QUALITY         DOUBLE,";
  sql += "  QUALITY_GRADE   TEXT,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}

void StsSensor::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fBatchId
         >> fLocationId
         >> fTypeId
         >> fVendorId
         >> fReticleName
         >> fWaferNumber
         >> fDate
         >> fQuality
         >> fQualityGrade
         >> fComment;
}

void StsSensor::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fBatchId
          << fLocationId
          << fTypeId
          << fVendorId
          << fReticleName
          << fWaferNumber
          << fDate
          << fQuality
          << fQualityGrade
          << fComment;
}

StsSensor::StsSensor(const StsSensor& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fBatchId = from.fBatchId;
  fLocationId = from.fLocationId;
  fTypeId = from.fTypeId;
  fVendorId = from.fVendorId;
  fReticleName = from.fReticleName;
  fWaferNumber = from.fWaferNumber;
  fDate = from.fDate;
  fQuality = from.fQuality;
  fQualityGrade = from.fQualityGrade;
  fComment = from.fComment;
}

StsSensor& StsSensor::operator=(const StsSensor& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fBatchId = from.fBatchId;
  fLocationId = from.fLocationId;
  fTypeId = from.fTypeId;
  fVendorId = from.fVendorId;
  fReticleName = from.fReticleName;
  fWaferNumber = from.fWaferNumber;
  fDate = from.fDate;
  fQuality = from.fQuality;
  fQualityGrade = from.fQualityGrade;
  fComment = from.fComment;

  return *this;
}