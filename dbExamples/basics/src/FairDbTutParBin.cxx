/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** 
 *  FairDbTutParBin.cxx 
 * 
 *  created @ 16-01-2017 
 *  by         D.Bertini  
 */ 


#include "FairDbTutParBin.h"

#include "FairParamList.h"
#include "FairDbParFactory.h"
#include "FairDbLogFormat.h"
#include "FairDbLogService.h"
#include "FairUtilStream.h"
#include "FairDbOutTableBuffer.h"
#include "FairDbStatement.h"
#include "FairDbStreamer.h"
#include "TMath.h"
#include <iostream>

using std::cout;
using std::endl;

ClassImp(FairDbTutParBin);

static FairDbGenericParRegistry<FairDbTutParBin> qa_iv("FairDbTutParBin", FairDbDetector::kGfi, DataType::kData);

#include "FairDbGenericParSet.tpl"
template class  FairDbGenericParSet<FairDbTutParBin>;

#include "FairDbReader.tpl"
template class  FairDbReader<FairDbTutParBin>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<FairDbTutParBin>;



FairDbTutParBin::FairDbTutParBin( const FairDbDetector::Detector_t detid, const DataType::DataType_t dataid,
                                  const char* name, const char* title, const char* context,Bool_t own)
  : FairDbGenericParSet<FairDbTutParBin>(detid, dataid, name, title, context, own),
    fTopPitch(0.),
    fTopAnchor(0.),
    fTopNrFE(0),
    fFeType("")
{
  // Constructor
  // Create heap based TH1 object and fill with dummy
  // value

  fMyHisto = new TH1F("test_histo","test_histo",10,0,10);
  // Reset all parameters
  clear();
}


FairDbTutParBin::~FairDbTutParBin()
{
  if (fMyHisto) { delete fMyHisto; fMyHisto=NULL; }
}

string FairDbTutParBin::GetTableDefinition(const char* Name)
{
  string sql("create table ");
  if ( Name ) { sql += Name; }
  else { sql += GetTableName(); }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_ID         INT NOT NULL,";
  sql += "  TOPPITCH       DOUBLE,";
  sql += "  TOPANCHOR      DOUBLE,";
  sql += "  TOPNRFE        INT,";
  sql += "  FETYPE         TEXT,";
  sql += "  MYIARRAY       TEXT,";
  sql += "  MYDARRAY       TEXT,";
  sql += "  MYHISTO        TEXT,";
  sql += "  primary key(SEQNO,ROW_ID))";
  return sql;
}


void FairDbTutParBin::putParams(FairParamList* list)
{
  std::cout<<"-I- FairDbTutParBin::putParams() called"<<std::endl;
  if(!list) { return; }
  list->add("top_pitch",  fTopPitch);
  list->add("top_anchor", fTopAnchor);
  list->add("nr_fe_top",  fTopNrFE);
  list->add("fe_Type",    (Text_t*) &fFeType);
}

Bool_t FairDbTutParBin::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("top_pitch", &fTopPitch)) { return kFALSE; }
  if (!list->fill("top_anchor", &fTopAnchor)) { return kFALSE; }
  if (!list->fill("nr_fe_top", &fTopNrFE)) { return kFALSE; }
  Text_t feName[80];
  if (!list->fill("fe_Type",feName,80)) { return kFALSE; }
  fFeType = feName;
  return kTRUE;
}

void FairDbTutParBin::clear()
{

  fTopPitch=fTopAnchor=0.0;
  fTopNrFE=0;
  fFeType="";
  for(Int_t i=0; i<1024; i++) { fMyIArray[i]=-1; }
  for(Int_t i=0; i<1024; i++) { fMyDArray[i]=-1.0; }
  if(fMyHisto) { (fMyHisto)->Reset(); }
  
  FairDbGenericParSet<FairDbTutParBin>::clear();
}



void FairDbTutParBin::Fill(FairDbResultPool& res_in,
                           const FairDbValRecord* valrec)
{
  // Instanciate  & clear() the Data Contents
  clear();
  FairDbStreamer dbIArray(fMyIArray,1024);
  FairDbStreamer dbDArray(fMyDArray,1024);
  FairDbStreamer dbHisto(fMyHisto);

  res_in >> fTopPitch  >> fTopAnchor   >> fTopNrFE  >> fFeType >> dbIArray >> dbDArray>> dbHisto;

  // Update data members
  dbIArray.Fill(fMyIArray);
  dbDArray.Fill(fMyDArray);
  dbHisto.Fill(fMyHisto);
}

void FairDbTutParBin::Store(FairDbOutTableBuffer& res_out,
                            const FairDbValRecord* valrec) const
{
  FairDbStreamer dbIArray(fMyIArray,1024);
  FairDbStreamer dbDArray(fMyDArray,1024);
  FairDbStreamer dbHisto(fMyHisto);
  res_out << fTopPitch  << fTopAnchor   << fTopNrFE  << fFeType << dbIArray << dbDArray << dbHisto;
}


void FairDbTutParBin::FillDummy()
{
  for(Int_t i=0; i<1024; i++) { fMyIArray[i]=(i+1)*10.; }
  for(Int_t i=0; i<1024; i++) { fMyDArray[i]=0.1*(i+1); }

  fMyHisto->Fill(1);
  fMyHisto->Fill(2);
  fMyHisto->Fill(3);
}


void FairDbTutParBin::Print()
{
  std::cout<<""<<std::endl;
  std::cout<<"    Top Pitch    = "<<fTopPitch<<std::endl;
  std::cout<<"    Top Anchor   = "<<fTopAnchor<< std::endl;
  std::cout<<"    Nr of Frontends (Top Side)   = "<<fTopNrFE<<std::endl;
  std::cout<<"    Frontend type name is        = "<<fFeType<<std::endl;

  std::cout<<"    Array of <Int>               : "<<std::endl;
  for(Int_t i=0; i<1024; i++) {
    std::cout<<"                   arr[ "<< i <<  " ] = " << fMyIArray[i]<< std::endl;
  }

  std::cout<<"    Array of <Double>               : "<<std::endl;
  for(Int_t i=0; i<1024; i++) {
    std::cout<<"                   arr[ "<< i <<  " ] = " << fMyDArray[i]<< std::endl;
  }

  cout << "      TH1F  histogram ptr# : "  << fMyHisto << endl;
  if (fMyHisto) { fMyHisto->Print(); }
}


FairDbTutParBin::FairDbTutParBin(const FairDbTutParBin& from){
  // Copy ctor 
  fTopPitch  = from.fTopPitch;
  fTopAnchor = from.fTopAnchor;
  fTopNrFE = from. fTopNrFE;
  fFeType = from.fFeType;
  for (int i=0; i< 1024 ; i++){
    fMyIArray[i] = from.fMyIArray[i];
    fMyDArray[i] = from.fMyDArray[i]; 
  }
  fMyHisto = from.fMyHisto;
}

FairDbTutParBin& FairDbTutParBin::operator=(const FairDbTutParBin& from){
  fTopPitch  = from.fTopPitch;
  fTopAnchor = from.fTopAnchor;
  fTopNrFE = from. fTopNrFE;
  fFeType = from.fFeType;
  
  for (int i=0; i< 1024 ; i++){
    fMyIArray[i] = from.fMyIArray[i];
    fMyDArray[i] = from.fMyDArray[i]; 
  }
  
  fMyHisto = from.fMyHisto;
  return *this;
}
