#include "StsSensorType.h"

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

ClassImp(StsSensorType);

static FairDbGenericParRegistry<StsSensorType> sts("StsSensorType", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsSensorType>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsSensorType>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsSensorType>;

StsSensorType::StsSensorType(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsSensorType>(detid, dataid, name, title, context, ownership),
  fId(0),
  fTypeName(""),
  fProcessing(""),
  fWidth(0.),
  fHeight(0.),
  fAliMarkWidth(0.),
  fAliMarkHeight(0.),
  fStripsPerSide(0),
  fPitch(0.),
  fThickness(0.),
  fStereoAngleP(0.),
  fStereoAngleN(0.),
  fComment("")
{

}

StsSensorType::~StsSensorType()
{

}

void StsSensorType::putParams(FairParamList* list)
{
  std::cout<<"-I- StsSensorType::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("type_name", (Text_t*)&fTypeName);
  list->add("processing", (Text_t*)&fProcessing);
  list->add("widht", fWidth);
  list->add("height", fHeight);
  list->add("alimark_widht", fAliMarkWidth);
  list->add("alimark_height", fAliMarkHeight);
  list->add("strips_per_side", fStripsPerSide);
  list->add("pitch", fPitch);
  list->add("thickness", fThickness);
  list->add("stereo_angle_p", fStereoAngleP);
  list->add("stereo_angle_n", fStereoAngleN);
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsSensorType::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t name[MAX_TEXT_SIZE];
  if (!list->fill("type_name", name, MAX_TEXT_SIZE))  {return kFALSE;}
  fTypeName = name;

  Text_t processing[MAX_TEXT_SIZE];
  if (!list->fill("processing", processing, MAX_TEXT_SIZE)) {return kFALSE;}
  fProcessing = processing;

  if (!list->fill("width", &fWidth)) {return kFALSE;}
  if (!list->fill("height", &fHeight)) {return kFALSE;}
  if (!list->fill("alimark_widht", &fAliMarkWidth)) {return kFALSE;}
  if (!list->fill("alimark_height", &fAliMarkHeight)) {return kFALSE;}
  if (!list->fill("strips_per_side", &fStripsPerSide)) {return kFALSE;}
  if (!list->fill("pitch", &fPitch)) {return kFALSE;}
  if (!list->fill("thickness", &fThickness)) {return kFALSE;}
  if (!list->fill("stereo_angle_p", &fStereoAngleP)) {return kFALSE;}
  if (!list->fill("stereo_angle_n", &fStereoAngleN)) {return kFALSE;}

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsSensorType::clear()
{
  fId = 0;
  fTypeName = "";
  fProcessing = "";
  fWidth = 0.;
  fHeight = 0.;
  fAliMarkWidth = 0.;
  fAliMarkHeight = 0.;
  fStripsPerSide = 0;
  fPitch = 0.;
  fThickness = 0.;
  fStereoAngleP = 0.;
  fStereoAngleN = 0.;
  fComment = "";
}

StsSensorType* StsSensorType::GetTypeById(Int_t typeId, UInt_t runId)
{
  StsSensorType type;
  FairDbReader<StsSensorType> r_type = *type.GetParamReader();

  ValTimeStamp ts;
  if (runId)
    ts = ValTimeStamp(runId);

  ValCondition context(FairDbDetector::kSts,DataType::kData,ts);

  r_type.Activate(context, type.GetVersion());
  return (StsSensorType *)r_type.GetRowByIndex(typeId);
}

void StsSensorType::Print()
{
  std::cout << "Sts Sensor Type Instance:"                          << std::endl;
  std::cout << "   Sensor Type Id         = "     << fId            << std::endl;
  std::cout << "   Type Name              = "     << fTypeName      << std::endl;
  std::cout << "   Processing             = "     << fProcessing    << std::endl;
  std::cout << "   Widht                  = "     << fWidth         << std::endl;
  std::cout << "   Height                 = "     << fHeight        << std::endl;
  std::cout << "   Alignment Mark Width   = "     << fAliMarkWidth  << std::endl;
  std::cout << "   Alignment Mark Height  = "     << fAliMarkHeight << std::endl;
  std::cout << "   Strips per side        = "     << fStripsPerSide << std::endl;
  std::cout << "   Pitch                  = "     << fPitch         << std::endl;
  std::cout << "   Thickness              = "     << fThickness     << std::endl;
  std::cout << "   Stereo angle P         = "     << fStereoAngleP  << std::endl;
  std::cout << "   Stereo angle N         = "     << fStereoAngleN  << std::endl;
  std::cout << "   Comment                = "     << fComment       << std::endl;
  std::cout                                                         << std::endl;
}

string StsSensorType::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  ID              INT NOT NULL,";
  sql += "  TYPE_NAME       TEXT,";
  sql += "  PROCESSING      TEXT,";
  sql += "  WIDTH           DOUBLE,";
  sql += "  HEIGHT          DOUBLE,";
  sql += "  ALIMARK_WIDTH   DOUBLE,";
  sql += "  ALIMARK_HEIGHT  DOUBLE,";
  sql += "  STRIPS_PER_SIDE INT,";
  sql += "  PITCH           DOUBLE,";
  sql += "  THICKNESS       DOUBLE,";
  sql += "  STEREO_ANGLE_P  DOUBLE,";
  sql += "  STEREO_ANGLE_N  DOUBLE,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsSensorType::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fTypeName
         >> fProcessing
         >> fWidth
         >> fHeight
         >> fAliMarkWidth
         >> fAliMarkHeight
         >> fStripsPerSide
         >> fPitch
         >> fThickness
         >> fStereoAngleP
         >> fStereoAngleN
         >> fComment;
}

void StsSensorType::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fTypeName
          << fProcessing
          << fWidth
          << fHeight
          << fAliMarkWidth
          << fAliMarkHeight
          << fStripsPerSide
          << fPitch
          << fThickness
          << fStereoAngleP
          << fStereoAngleN
          << fComment;
}

StsSensorType::StsSensorType(const StsSensorType& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fTypeName = from.fTypeName;
  fProcessing = from.fProcessing;
  fWidth = from.fWidth;
  fHeight = from.fHeight;
  fAliMarkWidth = from.fAliMarkWidth;
  fAliMarkHeight = from.fAliMarkHeight;
  fStripsPerSide = from.fStripsPerSide;
  fPitch = from.fPitch;
  fThickness = from.fThickness;
  fStereoAngleP = from.fStereoAngleP;
  fStereoAngleN = from.fStereoAngleN;
  fComment = from.fComment;
}

StsSensorType& StsSensorType::operator=(const StsSensorType& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fTypeName = from.fTypeName;
  fProcessing = from.fProcessing;
  fWidth = from.fWidth;
  fHeight = from.fHeight;
  fAliMarkWidth = from.fAliMarkWidth;
  fAliMarkHeight = from.fAliMarkHeight;
  fStripsPerSide = from.fStripsPerSide;
  fPitch = from.fPitch;
  fThickness = from.fThickness;
  fStereoAngleP = from.fStereoAngleP;
  fStereoAngleN = from.fStereoAngleN;
  fComment = from.fComment;
  
  return *this;
}