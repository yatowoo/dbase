/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** 
 *  FairDbTutParBin.h 
 * 
 *  created @ 12-01-2017 
 *  by         D.Bertini  
 */ 


#ifndef FAIRDBTUTPARBIN_H
#define FAIRDBTUTPARBIN_H


#include "TObject.h"
#include "Rtypes.h"
#include <TVector3.h>
#include <TObjString.h>
#include <TH1F.h>
#include "TBufferFile.h"
#include "FairDbDetector.h"
#include "DataType.h"
#include "FairDbParSet.h"
#include "ValCondition.h"
#include "ValInterval.h"
#include "ValTimeStamp.h"
#include <string>
#include <memory>

#include "FairDbGenericParSet.h"

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;
class FairDbValRecord;


class FairDbTutParBin : public FairDbGenericParSet<FairDbTutParBin>
{
 public :
  FairDbTutParBin (  const FairDbDetector::Detector_t detid=FairDbDetector::kGfi,
                     const DataType::DataType_t dataid=DataType::kData,                     
                     const char* name="ExTutParBin",
                     const char* title="Tutorial  parameter",
                     const char* context="TestDefaultContext",
                     Bool_t own=kTRUE);
  
  virtual ~FairDbTutParBin();
  FairDbTutParBin(const FairDbTutParBin& from);
  FairDbTutParBin& operator=(const FairDbTutParBin& from);
  
  void clear(void);
  void   putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);
  
  void Print();
  
  /** Accessor functions **/
  Double_t GetTopPitch()        const {return fTopPitch;}
  Double_t GetTopAnchor()       const {return fTopAnchor;}
  Int_t    GetNrTopFE()         const {return fTopNrFE;}
  string   GetFeType()          const {return fFeType;}
  Int_t*   GetMyIArray()         {return fMyIArray;}
  Double_t* GetMyDArray()        {return fMyDArray;}
  TH1F*      GetMyHisto()         {return fMyHisto;}
  
  void SetTopPitch(Double_t x)  {fTopPitch = x;}
  void SetTopAnchor(Double_t x) {fTopAnchor = x;}
  void SetNrTopFE(Int_t x)      {fTopNrFE = x;}
  void SetFeType(string x)     {fFeType = x;}
  void SetMyIArray(Int_t val, Int_t i) {fMyIArray[i]= val;}
  void SetMyDArray(Double_t val, Int_t i) {fMyDArray[i]= val;}
  
  virtual void print() {Print();}
  void FillDummy();
  
  // Add-ons: SQL descriptors for the parameter class
  virtual string GetTableDefinition(const char* Name = 0);
  
  // Atomic IO (intrinsic)
  virtual void Fill(FairDbResultPool& res_in,
                    const FairDbValRecord* valrec);
  virtual void Store(FairDbOutTableBuffer& res_out,
                     const FairDbValRecord* valrec) const;
  
  
 private:
  // Example Strip Parameters
  Double_t fTopPitch;     // Strip pitch on top wafer side
  Double_t fTopAnchor;    // Anchor point of top strip#0
  Int_t    fTopNrFE;      // Number of FE attached to top wafer side
  string   fFeType;       // Frontend type name
  Int_t    fMyIArray[1024];   // Array of Int_t fixed values
  Double_t fMyDArray[1024];  // Array of Double_t fixed values
  TH1F*    fMyHisto;  // data histo
  
  ClassDef(FairDbTutParBin,1); //  FairDbTutParBin Parameter Container example
};

#endif /* !FAIRDBTUTPARBIN_H*/
