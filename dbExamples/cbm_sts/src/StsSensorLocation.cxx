#include "StsSensorLocation.h"

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

ClassImp(StsSensorLocation);

static FairDbGenericParRegistry<StsSensorLocation> sts("StsSensorLocation", FairDbDetector::kSts, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<StsSensorLocation>;

#include "FairDbReader.tpl"
template class  FairDbReader<StsSensorLocation>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<StsSensorLocation>;

StsSensorLocation::StsSensorLocation(FairDbDetector::Detector_t detid, 
              DataType::DataType_t dataid, 
              const char* name, 
              const char* title, 
              const char* context, 
              Bool_t ownership):
  FairDbGenericParSet<StsSensorLocation>(detid, dataid, name, title, context, ownership),
  fId(0),
  fLocation(""),
  fOwner(""),
  fComment("")
{

}

StsSensorLocation::~StsSensorLocation()
{

}

void StsSensorLocation::putParams(FairParamList* list)
{
  std::cout<<"-I- StsSensorLocation::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("id", fId);
  list->add("location", (Text_t*)&fLocation);
  list->add("owner", (Text_t*)&fOwner);
  list->add("comment", (Text_t*)&fComment);
}

Bool_t StsSensorLocation::getParams(FairParamList* list)
{
  if (!list) { return kFALSE;}

  if (!list->fill("id", &fId))  {return kFALSE;}

  Text_t location[MAX_TEXT_SIZE];
  if (!list->fill("location", location, MAX_TEXT_SIZE)) {return kFALSE;}
  fLocation = location;

  Text_t owner[MAX_TEXT_SIZE];
  if (!list->fill("owner", owner, MAX_TEXT_SIZE)) {return kFALSE;}
  fOwner = owner;

  Text_t comment[MAX_TEXT_SIZE];
  if (!list->fill("comment", comment, MAX_TEXT_SIZE))     {return kFALSE;}
  fComment = comment;

  return kTRUE;
}

void StsSensorLocation::clear()
{
  fId = 0;
  fLocation = "";
  fOwner = "";
  fComment = "";
}


void StsSensorLocation::Print()
{
  std::cout << "Sts Sensor Location Instance:"                        << std::endl;
  std::cout << "   Sensor Location Id       = "     << fId            << std::endl;
  std::cout << "   Location                 = "     << fLocation      << std::endl;
  std::cout << "   Owner                    = "     << fOwner         << std::endl;
  std::cout << "   Comment                  = "     << fComment       << std::endl;
  std::cout                                                           << std::endl;
}

string StsSensorLocation::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO           INT NOT NULL,";
  sql += "  ROW_ID          INT NOT NULL,";
  sql += "  ID              INT NOT NULL,";
  sql += "  LOCATION        TEXT,";
  sql += "  OWNER           TEXT,";
  sql += "  COMMENT         TEXT,";
  sql += "  primary key(SEQNO,ROW_ID,ID))";
  return sql;
}

void StsSensorLocation::Fill(FairDbResultPool& res_in,
                        const FairDbValRecord* valrec)
{
  res_in >> fId
         >> fLocation
         >> fOwner
         >> fComment;
}

void StsSensorLocation::Store(FairDbOutTableBuffer& res_out,
                         const FairDbValRecord* valrec) const
{
  res_out << fId
          << fLocation
          << fOwner
          << fComment;
}

StsSensorLocation::StsSensorLocation(const StsSensorLocation& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fLocation = from.fLocation;
  fOwner = from.fOwner;
  fComment = from.fComment;
}

StsSensorLocation& StsSensorLocation::operator=(const StsSensorLocation& from){
  fCompId = from.GetCompId();
  fId = from.fId;
  fLocation = from.fLocation;
  fOwner = from.fOwner;
  fComment = from.fComment;
  
  return *this;
}