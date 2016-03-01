#ifndef CBMSTSDBSNOISYCHPAR_H
#define CBMSTSDBSNOISYCHPAR_H

#include <TObject.h>
#include <Rtypes.h>
#include <TObjString.h>
#include <TH1F.h>
#include <TBufferFile.h>
#include <TBits.h>

#include "FairDbDetector.h"
#include "DataType.h"
#include "FairDbParSet.h"
#include "ValCondition.h"
#include "ValInterval.h"
#include "ValTimeStamp.h"

#include <string>
#include <memory>

#include "FairDbWriter.h"
#include "FairDbReader.h"

#define NSIDE  2
#define NCHANNELS 1024

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;
class FairDbValRecord;

class CbmStsDbSNoisyChPar : public FairDbParSet
{
  public :
    CbmStsDbSNoisyChPar (const char* name="StsDbSNoisyChPar",
                     const char* title="Sts Noisy channels parameter",
                     const char* context="TestDefaultContext",
                     Bool_t own=kTRUE);

    virtual ~CbmStsDbSNoisyChPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    /** Accessor functions **/
    Int_t     GetDetId()        const {return fDetId;}
    Int_t     GetNChannels()    const {return fNChannels;}
    TBits*    GetNoisyChMap(Int_t i)      {return fNoisyCh[i];}
    Bool_t    IsChannelNoisy(Int_t side, Int_t ch)
    {return fNoisyCh[side]->TestBitNumber(ch);}

    void SetCompId(Int_t i)  {SetComboNo(i); fCompId=i;}
    void SetDetId(Int_t i)  {fDetId = i;}
    void SetNChannels(Int_t i)      {fNChannels = i;}

    void SetNoisyChMap(Int_t i, TBits* noisyCh) {
     if (fNoisyCh[i]) delete fNoisyCh[i];
     // Set the Bit array as a whole
      fNoisyCh[i] = noisyCh;
    }

    void SetNoisyChannel(Int_t side, Int_t ch){
    if (fNoisyCh[side]) fNoisyCh[side]->SetBitNumber(ch);
    }

    virtual void print() {Print();}
    void FillDummy();

    // Add-ons: SQL descriptors for the parameter class
    virtual string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
       CbmStsDbSNoisyChPar* p = new CbmStsDbSNoisyChPar();
      return dynamic_cast<FairDbObjTableMap*> (p);
    }

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

    // Global IO using run_id
    virtual void fill(UInt_t rid);
    virtual void store(UInt_t rid);

    // Validity frame definition
    virtual ValCondition GetContext(UInt_t rid) {
      return ValCondition(FairDbDetector::kSts,
                          DataType::kData,
                          ValTimeStamp(rid));
    }

    // SQL-IO Meta-Class Getters
    FairDbReader<CbmStsDbSNoisyChPar>* GetParamReader();
    FairDbWriter<CbmStsDbSNoisyChPar>* GetParamWriter();
    FairDbWriter<CbmStsDbSNoisyChPar>* ActivateWriter(Int_t rid);

  private:
    Int_t fCompId;
    Int_t  fDetId;          // Detector Id
    Int_t     fNChannels;      // Number of FE attached to top wafer side
    TBits*    fNoisyCh[2];     // data histo

    // Database Pool Index
    Int_t fDbEntry; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbSNoisyChPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbSNoisyChPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbSNoisyChPar,1); //  Tutorial5 CbmStsDbSNoisyChPar Parameter Container example
};


inline  FairDbReader<CbmStsDbSNoisyChPar>* CbmStsDbSNoisyChPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbSNoisyChPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbSNoisyChPar>* CbmStsDbSNoisyChPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbSNoisyChPar>();
    return fParam_Writer;
  }
}
#endif /* !CbmStsDbSNoisyChPar_H*/
