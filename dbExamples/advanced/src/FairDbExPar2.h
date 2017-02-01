#ifndef FAIRDBEXPAR2_H
#define FAIRDBEXPAR2_H

#include "FairDbGenericParSet.h"
#include "db_detector_def.h"            // FairDbDetector specific namespaces

using namespace std;

class FairParamList;

class FairDbExPar2 : public FairDbGenericParSet<FairDbExPar2>
{
  using TObject::Compare;
  
 public :
  FairDbExPar2 (const FairDbDetector::Detector_t detid=FairDbDetector::kSts,
                const DataType::DataType_t dataid=DataType::kData,                  
                const char* name="ExPar2",
                const char* title="Tutorial  parameter",
                const char* context="TestDefaultContext",
                Bool_t own=kTRUE);
  
  virtual ~FairDbExPar2(void);
  FairDbExPar2(const FairDbExPar2& from);
  FairDbExPar2& operator=(const FairDbExPar2& from);
  
  void clear(void);
  void   putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);
  
  void Print();
  
  /** Accessor functions **/
  void SetIdSystem(Int_t id)     {fIdSystem = id;}
  void SetData(Double_t x)       {fData = x;}
  
  UInt_t   GetIndex(UInt_t /*def_index*/) const {return fIdSystem/10; }  
  Int_t    GetIdSystem()        const {return fIdSystem;}
  Double_t GetData()      const {return fData;}
  
  Bool_t Compare(const FairDbExPar2& that ) const {
    return   (      fIdSystem ==  that.fIdSystem
                &&  fCompId   == that.fCompId
                &&  fData     == that.fData
                );
  }
  
  // Add-ons: SQL descriptors for the parameter class
  virtual std::string GetTableDefinition(const char* Name = 0);
  
  
  // Atomic IO (intrinsic)
  virtual void Fill(FairDbResultPool& res_in,
		    const FairDbValRecord* valrec);
  virtual void Store(FairDbOutTableBuffer& res_out,
		     const FairDbValRecord* valrec) const;
  
 private:
  Int_t       fCompId;        
  Int_t       fIdSystem;         
  Double_t    fData;          
  
  ClassDef(FairDbExPar2,0); // FairDbExPar2: Example of composite data type.
};

#endif /* !FAIRDBEXPAR2_H*/
