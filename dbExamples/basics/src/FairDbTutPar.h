/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** 
 *  FairDbTutPar.h 
 * 
 *  created @ 12-01-2017 
 *  by         D.Bertini  
 */ 


#ifndef FAIRDBTUTPAR_H
#define FAIRDBTUTPAR_H


#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for FairDbDetector, etc
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string


#include "FairDbGenericParSet.h"

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;


class FairDbTutPar : public FairDbGenericParSet<FairDbTutPar>
{
 public :
  FairDbTutPar (const FairDbDetector::Detector_t detid=FairDbDetector::kSts,
                const DataType::DataType_t dataid=DataType::kData,
                const char* name="ExTutPar",
                const char* title="Tutorial  parameter",
                const char* context="TestDefaultContext",
                Bool_t own=kTRUE
                );

  virtual ~FairDbTutPar();
  FairDbTutPar(const FairDbTutPar& from);
  FairDbTutPar& operator=(const FairDbTutPar& from);
  

  void clear(void);
  void   putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);
  
  void Print();
  /** Accessor functions **/
  Double_t GetTopPitch()        const {return fTopPitch;}
  Double_t GetTopAnchor()       const {return fTopAnchor;}
  Int_t    GetNrTopFE()         const {return fTopNrFE;}
  std::string   GetFeType()           const {return fFeType;}
  
  void SetTopPitch(Double_t x)  {fTopPitch = x;}
  void SetTopAnchor(Double_t x) {fTopAnchor = x;}
  void SetNrTopFE(Int_t x)      {fTopNrFE = x;}
  void SetFeType(std::string x)     {fFeType = x;}
  
  // Add-ons: SQL descriptors for the parameter class
  virtual std::string GetTableDefinition(const char* Name = 0);
  
  // Atomic IO (intrinsic)
  virtual void Fill(FairDbResultPool& res_in,
                    const FairDbValRecord* valrec);
  virtual void Store(FairDbOutTableBuffer& res_out,
                     const FairDbValRecord* valrec) const;

   
 private:
  // Strip Data Parameters
  Double_t fTopPitch;         // Strip pitch on top wafer side
  Double_t fTopAnchor;        // Anchor point of top strip#0
  Int_t    fTopNrFE;          // Number of FE attached to top wafer side
  std::string    fFeType;     // Frontend type name
  
  ClassDef(FairDbTutPar,1); // Basics FairDbTutPar Parameter Container example
};

#endif /* !FAIRDBTUTPAR_H*/

