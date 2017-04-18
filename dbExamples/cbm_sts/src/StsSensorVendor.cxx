#include "StsSensorVendor.h"

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

ClassImp(StsSensorVendor);

static FairDbGenericParRegistry<StsSensorVendor> sts("StsSensorVendor", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsSensorVendor>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsSensorVendor>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsSensorVendor>;

StsSensorVendor::StsSensorVendor(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsSensorVendor>(detid, dataid, name, title, context, ownership),
  fId(0),
  fVendorName(""),
  fLocation(""),
  fContactInfo(""),
  fComment("")
{

}

StsSensorVendor::~StsSensorVendor()
{

}

void StsSensorVendor::putParams(FairParamList* list)
{
  std::cout<<"-I- StsSensorVendor::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("vendor_name", (Text_t*)&fVendorName);
  list->add("location", (Text_t*)&fLocation);
  list->add("contact_info", (Text_t*)&fContactInfo);
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsSensorVendor::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t vendor_name[MAX_TEXT_SIZE];
  if (!list->fill("vendor_name", vendor_name, MAX_TEXT_SIZE))  {return kFALSE;}
  fVendorName = vendor_name;

  Text_t location[MAX_TEXT_SIZE];
  if (!list->fill("location", location, MAX_TEXT_SIZE)) {return kFALSE;}
  fLocation = location;

  Text_t contact_info[MAX_TEXT_SIZE];
  if (!list->fill("contact_info", contact_info, MAX_TEXT_SIZE)) {return kFALSE;}
  fContactInfo = contact_info;

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsSensorVendor::clear()
{
  fId = 0;
  fVendorName = "";
  fLocation = "";
  fContactInfo = "";
  fComment = "";
}


void StsSensorVendor::Print()
{
  std::cout << "Sts Sensor Vendor Instance:"                          << std::endl;
  std::cout << "   Sensor Vendor Id       = "     << fId            << std::endl;
  std::cout << "   Vendor Name            = "     << fVendorName    << std::endl;
  std::cout << "   Location               = "     << fLocation      << std::endl;
  std::cout << "   Contact info           = "     << fContactInfo   << std::endl;
  std::cout << "   Comment                = "     << fComment       << std::endl;
  std::cout                                                         << std::endl;
}

StsSensorVendor* StsSensorVendor::GetVendorByName(string vendorName, UInt_t runId)
{
  TObjArray *vendors = StsSensorVendor::GetArray(-1, runId);
  for (Int_t i = 0; i<vendors->GetEntries(); i++)
  {
    StsSensorVendor *vendor = (StsSensorVendor*)vendors->At(i);
    if (!vendor)
      continue;

    if (vendor->GetVendorName() == vendorName)
      return vendor;
  }

  return NULL;
}

string StsSensorVendor::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  ID              INT NOT NULL,";
  sql += "  VENDOR_NAME     TEXT,";
  sql += "  LOCATION        TEXT,";
  sql += "  CONTACT_INFO    TEXT,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsSensorVendor::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fVendorName
         >> fLocation
         >> fContactInfo
         >> fComment;
}

void StsSensorVendor::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fVendorName
          << fLocation
          << fContactInfo
          << fComment;
}

StsSensorVendor::StsSensorVendor(const StsSensorVendor& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fVendorName = from.fVendorName;
  fLocation = from.fLocation;
  fContactInfo = from.fContactInfo;
  fComment = from.fComment;
}

StsSensorVendor& StsSensorVendor::operator=(const StsSensorVendor& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fVendorName = from.fVendorName;
  fLocation = from.fLocation;
  fContactInfo = from.fContactInfo;
  fComment = from.fComment;
  
  return *this;
}