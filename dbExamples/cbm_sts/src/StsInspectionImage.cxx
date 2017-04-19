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

ClassImp(StsInspectionImage);

static FairDbGenericParRegistry<StsInspectionImage> sts("StsInspectionImage", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsInspectionImage>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsInspectionImage>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsInspectionImage>;

StsInspectionImage::StsInspectionImage(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsInspectionImage>(detid, dataid, name, title, context, ownership),
  fId(0),
  fInspectionId(0),
  fX(0),
  fY(0),
  fImageURI(""),
  fThumbnailURI(""),
  fOverlayURI("")
{
  fInspection = NULL;
  fDefects = NULL;
}

StsInspectionImage::~StsInspectionImage()
{
  delete fInspection;
  delete fDefects;
}

void StsInspectionImage::putParams(FairParamList* list)
{
  std::cout<<"-I- StsInspectionImage::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id",                        fId);
  list->add("inspection_id",             fInspectionId);
  list->add("x",                         fX);
  list->add("y",                         fY);
  list->add("image_uri",       (Text_t*)&fImageURI);
  list->add("thumbnail_uri",   (Text_t*)&fThumbnailURI);
  list->add("overlay_uri",     (Text_t*)&fOverlayURI);
}

Bool_t StsInspectionImage::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id",                 &fId))                     { return kFALSE; }
  if (!list->fill("inspection_id",      &fInspectionId))           { return kFALSE; }
  if (!list->fill("x",                  &fX))                      { return kFALSE; }
  if (!list->fill("y",                  &fY))                      { return kFALSE; }

  Text_t image_uri[MAX_TEXT_SIZE];
  if (!list->fill("image_uri", image_uri, MAX_TEXT_SIZE))          { return kFALSE; }
  fImageURI = image_uri;

  Text_t thumbnail_uri[MAX_TEXT_SIZE];
  if (!list->fill("thumbnail_uri", thumbnail_uri, MAX_TEXT_SIZE))  { return kFALSE; }
  fThumbnailURI = thumbnail_uri;

  Text_t overlay_uri[MAX_TEXT_SIZE];
  if (!list->fill("overlay_uri", overlay_uri, MAX_TEXT_SIZE))      { return kFALSE; }
  fOverlayURI = overlay_uri;
  return kTRUE;
}

void StsInspectionImage::clear()
{
  fId = 0;
  fInspectionId = 0;
  fX = 0;
  fY = 0;
  fImageURI = "";
  fThumbnailURI = "";
  fOverlayURI = "";

  delete fInspection;
  fInspection = NULL;

  delete fDefects;
  fDefects = NULL;
}

void StsInspectionImage::Print()
{
  std::cout << "Sts Optical Inspection Image Instance:"            << std::endl;
  std::cout << "   Image Id             = "    << fId             << std::endl;
  std::cout << "   Inspection Id        = "    << fInspectionId   << std::endl;
  std::cout << "   ROI X                = "    << fX              << std::endl;
  std::cout << "   ROI Y                = "    << fY              << std::endl;
  std::cout << "   Image URI            = "    << fImageURI       << std::endl;
  std::cout << "   Thumbnail URI        = "    << fThumbnailURI   << std::endl;
  std::cout << "   Overlay URI          = "    << fOverlayURI     << std::endl;
  std::cout                                                       << std::endl;
}

StsInspectionImage* StsInspectionImage::GetInspectionImage(Int_t inspectionId, Int_t X, Int_t Y, UInt_t runId)
{
  TObjArray *images = StsInspectionImage::GetArray(inspectionId, runId);
  for (Int_t i = 0; i<images->GetEntries(); i++)
  {
    StsInspectionImage *img = (StsInspectionImage*)images->At(i);
    if (!img)
      continue;

    if (img->GetX() == X && img->GetY() == Y)
      return img;
  }

  return NULL;
}

TObjArray* StsInspectionImage::GetDefects() {
  if (!fDefects) fDefects = StsDefect::GetDefectsByImageId(fId);
  return fDefects;
}


string StsInspectionImage::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO             INT NOT NULL,";
  sql += "  ROW_ID            INT NOT NULL,";
  sql += "  ID                INT NOT NULL,";
  sql += "  INSPECTION_ID     INT NOT NULL,";
  sql += "  X                 INT,";
  sql += "  Y                 INT,";
  sql += "  IMAGE_URI         TEXT,";
  sql += "  THUMBNAIL_URI     TEXT,";
  sql += "  OVERLAY_URI       TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsInspectionImage::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in  >> fId
          >> fInspectionId
          >> fX
          >> fY
          >> fImageURI
          >> fThumbnailURI
          >> fOverlayURI;
}

void StsInspectionImage::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fInspectionId
          << fX
          << fY
          << fImageURI
          << fThumbnailURI
          << fOverlayURI;
}

StsInspectionImage::StsInspectionImage(const StsInspectionImage& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fInspectionId = from.fInspectionId;
  fX = from.fX;
  fY = from.fY;
  fImageURI = from.fImageURI;
  fThumbnailURI = from.fThumbnailURI;
  fOverlayURI = from.fOverlayURI;
}

StsInspectionImage& StsInspectionImage::operator=(const StsInspectionImage& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fInspectionId = from.fInspectionId;
  fX = from.fX;
  fY = from.fY;
  fImageURI = from.fImageURI;
  fThumbnailURI = from.fThumbnailURI;
  fOverlayURI = from.fOverlayURI;
  
  return *this;
}