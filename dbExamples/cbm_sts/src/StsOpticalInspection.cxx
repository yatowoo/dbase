#include "StsOpticalInspection.h"
#include "StsInspectionImage.h"
#include "StsDefect.h"

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

ClassImp(StsOpticalInspection);

static FairDbGenericParRegistry<StsOpticalInspection> sts("StsOpticalInspection", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsOpticalInspection>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsOpticalInspection>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsOpticalInspection>;


StsOpticalInspection::StsOpticalInspection(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsOpticalInspection>(detid, dataid, name, title, context, ownership),
  fId(0),
  fSensorId(0),
  fSizeX(0),
  fSizeY(0),
  fDate(ValTimeStamp::GetBOT()),
  fSensorSide(""),
  fMaxWarp(0.),
  fThickness(0.),
  fQuality(0.),
  fQualityGrade(""),
  fPassed(kFALSE),
  fComment("")
{
  fSensor = NULL;
  fInspectionImages = NULL;
  fDefects = NULL;
  fWarp = new TObject();
}

StsOpticalInspection::~StsOpticalInspection()
{
  delete fSensor;
  delete fInspectionImages;
  delete fDefects;
  delete fWarp;
}

void StsOpticalInspection::putParams(FairParamList* list)
{
  std::cout<<"-I- StsOpticalInspection::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("inspection_id", fId);
  list->add("sensor_id", fSensorId);
  list->add("size_x", fSizeX);
  list->add("size_y", fSizeY);
  list->add("date", FairDb::MakeDateTimeString(fDate).Data());
  list->add("sensor_side",     (Text_t*)&fSensorSide);
  list->add("max_warp", fMaxWarp);
  list->addObject("warp", fWarp);
  list->add("thickness", fThickness);
  list->add("quality", fQuality);
  list->add("quality_grade", (Text_t*)&fQualityGrade);
  list->add("passed", fPassed);
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsOpticalInspection::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("inspection_id", &fId))  {return kFALSE;}
  if (!list->fill("sensor_id", &fSensorId))  {return kFALSE;}
  if (!list->fill("size_x", &fSizeX))  {return kFALSE;}
  if (!list->fill("size_y", &fSizeY))  {return kFALSE;}

  Text_t date[MAX_TEXT_SIZE];
  if (!list->fill("date", date, MAX_TEXT_SIZE)) {return kFALSE;}
  fDate = FairDb::MakeTimeStamp(date);

  Text_t sensor_side[MAX_TEXT_SIZE];
  if (!list->fill("sensor_side", sensor_side, MAX_TEXT_SIZE))  {return kFALSE;}
  fSensorSide = sensor_side;

  if (!list->fill("max_warp", &fMaxWarp))  {return kFALSE;}
  if (!list->fillObject("warp", fWarp))  {return kFALSE;}
  if (!list->fill("thickness", &fThickness))  {return kFALSE;}
  if (!list->fill("quality", &fQuality)) {return kFALSE;}

  Text_t quality_grade[MAX_TEXT_SIZE];
  if (!list->fill("quality_grade", quality_grade, MAX_TEXT_SIZE))     {return kFALSE;}
  fQualityGrade = quality_grade;

  if (!list->fill("passed", &fPassed))  {return kFALSE;}
  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsOpticalInspection::clear()
{
  fId = 0;
  fSensorId = 0;
  fSizeX = 0;
  fSizeY = 0;
  fDate = ValTimeStamp::GetBOT();
  fSensorSide = "";
  fMaxWarp = 0.;
  fThickness = 0.;
  fQuality = 0.;
  fQualityGrade = "";
  fPassed = kFALSE;
  fComment = "";

  delete fSensor;
  fSensor = NULL;

  delete fInspectionImages;
  fInspectionImages = NULL;

  delete fDefects;
  fInspectionImages = NULL;

  delete fWarp;
  fWarp = NULL;
}

void StsOpticalInspection::Print()
{
  std::cout << "Sts Sensor Optical Inspection Instance:"         << std::endl;
  std::cout << "   Inspection Id          = "   << fId           << std::endl;
  std::cout << "   Sensor Id              = "   << fSensorId     << std::endl;
  std::cout << "   Size X                 = "   << fSizeX        << std::endl;
  std::cout << "   Size Y                 = "   << fSizeY        << std::endl;
  std::cout << "   Date                   = "   << fDate         << std::endl;
  std::cout << "   Sensor Side            = "   << fSensorSide   << std::endl;
  std::cout << "   Maximal Warp           = "   << fMaxWarp      << std::endl;
  std::cout << "   Warp                   = "   << fWarp         << std::endl;
  std::cout << "   Thickness              = "   << fThickness    << std::endl;
  std::cout << "   Quality                = "   << fQuality      << std::endl;
  std::cout << "   Quality Grade          = "   << fQualityGrade << std::endl;
  std::cout << "   Passed                 = "   << fPassed       << std::endl;
  std::cout << "   Comment                = "   << fComment      << std::endl;
  std::cout                                                      << std::endl;
}

StsSensor* StsOpticalInspection::GetSensor() {
  if (!fSensor) fSensor = StsSensor::GetSensorById(fSensorId);
  return fSensor;
}

TObjArray* StsOpticalInspection::GetInspectionImages() {
  if (!fInspectionImages) fInspectionImages = StsInspectionImage::GetInspectionImages(fId);
  return fInspectionImages;
}

TObjArray* StsOpticalInspection::GetDefects() {
  if (!fDefects) fDefects = StsDefect::GetDefectsByInspectionId(fId);
  return fDefects;
}

StsOpticalInspection* StsOpticalInspection::GetInspectionById(Int_t inspectionId, UInt_t runId)
{
  StsOpticalInspection inspection;
  FairDbReader<StsOpticalInspection> r_inspection = *inspection.GetParamReader();

  ValTimeStamp ts;
  if (runId)
    ts = ValTimeStamp(runId);

  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  r_inspection.Activate(context, inspection.GetVersion());
  return (StsOpticalInspection *)r_inspection.GetRowByIndex(inspectionId);
}

TObjArray* StsOpticalInspection::GetInspections(Int_t sensorId, UInt_t runId)
{
  StsOpticalInspection inspection;
  FairDbReader<StsOpticalInspection> r_inspection;

  ValTimeStamp ts;
  if (runId)
    ts = ValTimeStamp(runId);
  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  r_inspection.Activate(context, inspection.GetVersion());
  Int_t numRows = r_inspection.GetNumRows();
  if (!numRows)
    return NULL;

  TObjArray* inspections = new TObjArray(numRows);
  for (Int_t i=0; i < numRows; i++)
  {
    StsOpticalInspection *insp = (StsOpticalInspection*)r_inspection.GetRow(i);
    if (!insp)
      continue;

    if (insp->GetSensorId() == sensorId)
      inspections->Add(insp);
  }

  return inspections;
}

string StsOpticalInspection::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  INSPECTION_ID   INT NOT NULL,";
  sql += "  SENSOR_ID       INT NOT NULL,";
  sql += "  SIZE_X          INT,";
  sql += "  SIZE_Y          INT,";
  sql += "  DATE            DATETIME,";
  sql += "  SENSOR_SIDE     TEXT,";
  sql += "  MAX_WARP        DOUBLE,";
  sql += "  WARP            TEXT,";
  sql += "  THICKNESS       DOUBLE,";
  sql += "  QUALITY         DOUBLE,";
  sql += "  QUALITY_GRADE   TEXT,";
  sql += "  PASSED          INT,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,INSPECTION_ID))";
  return sql;
}

void StsOpticalInspection::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  FairDbStreamer warpStreamer(fWarp);
  res_in >> fId
         >> fSensorId
         >> fSizeX
         >> fSizeY
         >> fDate
         >> fSensorSide
         >> fMaxWarp
         >> warpStreamer
         >> fThickness
         >> fQuality
         >> fQualityGrade
         >> fPassed
         >> fComment;
}

void StsOpticalInspection::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{  
  FairDbStreamer warpStreamer(fWarp);
  res_out << fId
          << fSensorId
          << fSizeX
          << fSizeY
          << fDate
          << fSensorSide
          << fMaxWarp
          << warpStreamer
          << fThickness
          << fQuality
          << fQualityGrade
          << fPassed
          << fComment;

  warpStreamer.Fill(fWarp);
}

StsOpticalInspection::StsOpticalInspection(const StsOpticalInspection& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fSensorId = from.fSensorId;
  fSizeX = from.fSizeX;
  fSizeY = from.fSizeY;
  fDate = from.fDate;
  fSensorSide = from.fSensorSide;
  fMaxWarp = from.fMaxWarp;
  fWarp = from.fWarp;
  fThickness = from.fThickness;
  fQuality = from.fQuality;
  fQualityGrade = from.fQualityGrade;
  fPassed = from.fPassed;
  fComment = from.fComment;
}

StsOpticalInspection& StsOpticalInspection::operator=(const StsOpticalInspection& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fSensorId = from.fSensorId;
  fSizeX = from.fSizeX;
  fSizeY = from.fSizeY;
  fDate = from.fDate;
  fSensorSide = from.fSensorSide;
  fMaxWarp = from.fMaxWarp;
  fWarp = from.fWarp;
  fThickness = from.fThickness;
  fQuality = from.fQuality;
  fQualityGrade = from.fQualityGrade;
  fPassed = from.fPassed;
  fComment = from.fComment;

  return *this;
}