#ifndef CBMSTSDBSINFOPAR_H
#define CBMSTSDBSINFOPAR_H

#include "FairDbParSet.h"          // for FairDbParSet

#include "TObjArray.h"
#include "TGeoMaterial.h"
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

#include "CbmStsDbSNoisyChPar.h"

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;

class CbmStsDbSInfoPar : public FairDbParSet
{
  using TObject::Compare;

  public :
  CbmStsDbSInfoPar (const char* name="StsDbSInfoPar",
  const char* title="Cbm Sts Sensor Database",
  const char* context="TestDefaultContext",
  Bool_t own=kTRUE);
  virtual ~CbmStsDbSInfoPar(void);

  // RuntimeDB interfaces
  void   clear(void);
  void   putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);
  // Global IO using run_id
  virtual void fill(UInt_t rid);
  virtual void store(UInt_t rid);


  // Lists handling
  void   AddNoisyChInfo(CbmStsDbSNoisyChPar *s){fSNoisyCh->AddAt(s, s->GetDetId());}
  void   AddNoisyChInfoAt(CbmStsDbSNoisyChPar *s, int idx){fSNoisyCh->AddAt(s,idx);}
  TObjArray* GetListOfSNoisyChInfo() {return fSNoisyCh;}
  Int_t GetNumSNoisyChInfo() {return fSNoisyCh->GetEntries();}
  CbmStsDbSNoisyChPar* GetSNoisyChInfoAt(Int_t idx){return (CbmStsDbSNoisyChPar*) fSNoisyCh->At(idx);}

  void   Print();

  FairDbReader<CbmStsDbSNoisyChPar>* GetReaderSNoisyCh(){return fReader_SNoisyCh;}

  private:
  TObjArray* fSNoisyCh;
  FairDbReader<CbmStsDbSNoisyChPar>* fReader_SNoisyCh; //!

  ClassDef(CbmStsDbSInfoPar,1); // CbmStsDbSInfoPar Parameter Container
};

#endif /* !CBMSTSDBSINFOPAR_H*/
