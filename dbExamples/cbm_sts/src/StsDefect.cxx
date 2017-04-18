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

ClassImp(StsDefect);

static FairDbGenericParRegistry<StsDefect> sts("StsDefect", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsDefect>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsDefect>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsDefect>;

StsDefect::StsDefect(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsDefect>(detid, dataid, name, title, context, ownership),
  fId(0),
  fInspectionId(0),
  fTypeId(0),
  fContextId(0),
  fAlgorithmId(0),
  fImageId(0),
  fLeft(0.),
  fTop(0.),
  fWidth(0.),
  fHeight(0.),
  fCenterMassX(0.),
  fCenterMassY(0.),
  fArea(0.),
  fPerimeter(0.),
  fHydraulicRadius(0.),
  fAreaRatio(0.),
  fElongation(0.),
  fCompactness(0.),
  fCircularity(0.),
  fComment("")
{
  fInspection = NULL;
  fType = NULL;
  fContext = NULL;
  fImage = NULL;
}

StsDefect::~StsDefect()
{
  delete fInspection;
  delete fType;
  delete fContext;
  delete fImage;
}

void StsDefect::putParams(FairParamList* list)
{
  std::cout<<"-I- StsDefect::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id",                        fId);
  list->add("inspection_id",             fInspectionId);
  list->add("type_id",                   fTypeId);
  list->add("context_id",                fContextId);
  list->add("algorithm_id",              fAlgorithmId);
  list->add("image_id",                  fImageId);
  list->add("left",                      fLeft);
  list->add("top",                       fTop);
  list->add("width",                     fWidth);
  list->add("height",                    fHeight);
  list->add("centermass_x",              fCenterMassX);
  list->add("centermass_y",              fCenterMassY);
  list->add("area",                      fArea);
  list->add("perimeter",                 fPerimeter);
  list->add("hydraulic_radius",          fHydraulicRadius);
  list->add("area_ratio",                fAreaRatio);
  list->add("elongation",                fElongation);
  list->add("compactness",               fCompactness);
  list->add("circularity",               fCircularity);
  list->add("comment",         (Text_t*)&fComment);
}

Bool_t StsDefect::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id",                  &fId))                 { return kFALSE; }
  if (!list->fill("inspection_id",       &fInspectionId))       { return kFALSE; }
  if (!list->fill("type_id",             &fTypeId))             { return kFALSE; }
  if (!list->fill("context_id",          &fContextId))          { return kFALSE; }
  if (!list->fill("algorithm_id",        &fAlgorithmId))        { return kFALSE; }
  if (!list->fill("image_id",            &fImageId))            { return kFALSE; }
  if (!list->fill("left",                &fLeft))               { return kFALSE; }
  if (!list->fill("top",                 &fTop))                { return kFALSE; }
  if (!list->fill("width",               &fWidth))              { return kFALSE; }
  if (!list->fill("height",              &fHeight))             { return kFALSE; }
  if (!list->fill("centermass_x",        &fCenterMassX))        { return kFALSE; }
  if (!list->fill("centermass_y",        &fCenterMassY))        { return kFALSE; }
  if (!list->fill("area",                &fArea))               { return kFALSE; }
  if (!list->fill("perimeter",           &fPerimeter))          { return kFALSE; }
  if (!list->fill("hydraulic_radius",    &fHydraulicRadius))    { return kFALSE; }
  if (!list->fill("area_ratio",          &fAreaRatio))          { return kFALSE; }
  if (!list->fill("elongation",          &fElongation))         { return kFALSE; }
  if (!list->fill("compactness",         &fCompactness))        { return kFALSE; }
  if (!list->fill("circularity",         &fCircularity))        { return kFALSE; }

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))  {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsDefect::clear()
{
  fId = 0;
  fInspectionId = 0;
  fTypeId = 0;
  fContextId = 0;
  fAlgorithmId = 0;
  fImageId = 0;
  fLeft = 0.;
  fTop = 0.;
  fWidth = 0.;
  fHeight = 0.;
  fCenterMassX = 0.;
  fCenterMassY = 0.;
  fArea = 0.;
  fPerimeter = 0.;
  fHydraulicRadius = 0.;
  fAreaRatio = 0.;
  fElongation = 0.;
  fCompactness = 0.;
  fCircularity = 0.;
  fComment = "";

  delete fInspection;
  fInspection = NULL;
  
  delete fType;
  fType = NULL;

  delete fContext;
  fContext = NULL;

  delete fImage;
  fImage = NULL;
}


void StsDefect::Print()
{
  std::cout << "Sts Defect Instance:"                                          << std::endl;
  std::cout << "   Defect Id                        = "    << fId              << std::endl;
  std::cout << "   Inspection Id                    = "    << fInspectionId    << std::endl;
  std::cout << "   Defect Type Id                   = "    << fTypeId          << std::endl;
  std::cout << "   Defect Context Id                = "    << fContextId       << std::endl;
  std::cout << "   Defect Finder Algorithm id       = "    << fAlgorithmId     << std::endl;
  std::cout << "   Defect Image id                  = "    << fImageId         << std::endl;
  std::cout << "   Left                             = "    << fLeft            << std::endl;
  std::cout << "   Top                              = "    << fTop             << std::endl;
  std::cout << "   Width                            = "    << fWidth           << std::endl;
  std::cout << "   Height                           = "    << fHeight          << std::endl;
  std::cout << "   Center of Mass X                 = "    << fCenterMassX     << std::endl;
  std::cout << "   Center of Mass Y                 = "    << fCenterMassY     << std::endl;
  std::cout << "   Area                             = "    << fArea            << std::endl;
  std::cout << "   Perimeter                        = "    << fPerimeter       << std::endl;
  std::cout << "   Hydraulic Radius                 = "    << fHydraulicRadius << std::endl;
  std::cout << "   Defect area to Image area ratio  = "    << fAreaRatio       << std::endl;
  std::cout << "   Elongation                       = "    << fElongation      << std::endl;
  std::cout << "   Compactness                      = "    << fCompactness     << std::endl;
  std::cout << "   Circularity                      = "    << fCircularity     << std::endl;
  std::cout << "   Comment                          = "    << fComment         << std::endl;
  std::cout                                                                    << std::endl;
}

TObjArray* StsDefect::GetDefectsByImageId(Int_t inspectionImageId, UInt_t runId)
{
  StsInspectionImage *image = StsInspectionImage::GetById(inspectionImageId, runId);
  TObjArray *defects = StsDefect::GetDefectsByInspectionId(image->GetInspectionId(), runId);
  Int_t numRows = defects->GetEntries();

  TObjArray *result = new TObjArray(numRows);
  for (Int_t i=0; i < numRows; i++)
  {
    StsDefect *def = (StsDefect*)defects->At(i);
    if (!def)
      continue;

    if (def->GetImageId() == inspectionImageId) {
      result->Add(def);
    }
  }

  if (result->GetEntries()){
    return result;
  } else {
    delete result;
    return NULL;
  }
}

string StsDefect::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO             INT NOT NULL,";
  sql += "  ROW_ID            INT NOT NULL,";
  sql += "  ID                INT NOT NULL,";
  sql += "  INSPECTION_ID     INT NOT NULL,";
  sql += "  TYPE_ID           INT NOT NULL,";
  sql += "  CONTEXT_ID        INT NOT NULL,";
  sql += "  ALGORITHM_ID      INT NOT NULL,";
  sql += "  IMAGE_ID          INT NOT NULL,";
  sql += "  LEFT              DOUBLE,";
  sql += "  TOP               DOUBLE,";
  sql += "  WIDTH             DOUBLE,";
  sql += "  HEIGHT            DOUBLE,";
  sql += "  CENTERMASS_X      DOUBLE,";
  sql += "  CENTERMASS_Y      DOUBLE,";
  sql += "  AREA              DOUBLE,";
  sql += "  PERIMETER         DOUBLE,";
  sql += "  HYDRAULIC_RADIUS  DOUBLE,";
  sql += "  AREA_RATIO        DOUBLE,";
  sql += "  ELONGATION        DOUBLE,";
  sql += "  COMPACTNESS       DOUBLE,";
  sql += "  CIRCULARITY       DOUBLE,";
  sql += "  COMMENT           TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsDefect::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fInspectionId
         >> fTypeId
         >> fContextId
         >> fAlgorithmId
         >> fImageId
         >> fLeft
         >> fTop
         >> fWidth
         >> fHeight
         >> fCenterMassX
         >> fCenterMassY
         >> fArea
         >> fPerimeter
         >> fHydraulicRadius
         >> fAreaRatio
         >> fElongation
         >> fCompactness
         >> fCircularity
         >> fComment;
}

void StsDefect::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fInspectionId
          << fTypeId
          << fContextId
          << fAlgorithmId
          << fImageId
          << fLeft
          << fTop
          << fWidth
          << fHeight
          << fCenterMassX
          << fCenterMassY
          << fArea
          << fPerimeter
          << fHydraulicRadius
          << fAreaRatio
          << fElongation
          << fCompactness
          << fCircularity
          << fComment;
}


StsDefect::StsDefect(const StsDefect& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fInspectionId = from.fInspectionId;
  fTypeId = from.fTypeId;
  fContextId = from.fContextId;
  fAlgorithmId = from.fAlgorithmId;
  fImageId = from.fImageId;
  fLeft = from.fLeft;
  fTop = from.fTop;
  fWidth = from.fWidth;
  fHeight = from.fHeight;
  fCenterMassX = from.fCenterMassX;
  fCenterMassY = from.fCenterMassY;
  fArea = from.fArea;
  fPerimeter = from.fPerimeter;
  fHydraulicRadius = from.fHydraulicRadius;
  fAreaRatio = from.fAreaRatio;
  fElongation = from.fElongation;
  fCompactness = from.fCompactness;
  fCircularity = from.fCircularity;
  fComment = from.fComment;
}

StsDefect& StsDefect::operator=(const StsDefect& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fInspectionId = from.fInspectionId;
  fTypeId = from.fTypeId;
  fContextId = from.fContextId;
  fAlgorithmId = from.fAlgorithmId;
  fImageId = from.fImageId;
  fLeft = from.fLeft;
  fTop = from.fTop;
  fWidth = from.fWidth;
  fHeight = from.fHeight;
  fCenterMassX = from.fCenterMassX;
  fCenterMassY = from.fCenterMassY;
  fArea = from.fArea;
  fPerimeter = from.fPerimeter;
  fHydraulicRadius = from.fHydraulicRadius;
  fAreaRatio = from.fAreaRatio;
  fElongation = from.fElongation;
  fCompactness = from.fCompactness;
  fCircularity = from.fCircularity;
  fComment = from.fComment;
  
  return *this;
}