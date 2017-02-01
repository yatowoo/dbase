#ifndef FAIRDBEXPAR1_H
#define FAIRDBEXPAR1_H

#include "FairDbGenericParSet.h"
#include "db_detector_def.h"            // FairDbDetector specific namespaces

using namespace std;

class FairParamList;

class FairDbExPar1 : public FairDbGenericParSet<FairDbExPar1>
{
  public :
  FairDbExPar1 (const FairDbDetector::Detector_t detid=FairDbDetector::kSts,
                const DataType::DataType_t dataid=DataType::kData,                  
                const char* name="ExPar1",
                const char* title="Tutorial  parameter",
                const char* context="TestDefaultContext",
                Bool_t own=kTRUE);

    virtual ~FairDbExPar1(void);
    FairDbExPar1(const FairDbExPar1& from);
    FairDbExPar1& operator=(const FairDbExPar1& from);
    
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    /** Accessor functions **/
    void SetIdSystem(Int_t id)     {fIdSystem = id;}
    void SetPedestal(Double_t x)   {fPedestal = x;}
    void SetGain1(Double_t x)      {fGain1 = x;}
    void SetGain2(Double_t x)      {fGain2 = x;}
    
    UInt_t    GetIndex(UInt_t /* def_index */) const {return fIdSystem + 3000000000U; }  
    Int_t     GetIdSystem()        const {return fIdSystem;}
    Double_t  GetPedestal()      const {return fPedestal;}
    Double_t    GetGain1()      const {return fGain1;}
    Double_t    GetGain2()      const {return fGain2;}


    Bool_t Compare(const FairDbExPar1& that ) const {
      return   (  fIdSystem ==  that.fIdSystem
		  &&  fPedestal  == that.fPedestal
		  &&  fGain1     == that.fGain1
		  &&  fGain2     == that.fGain2
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
    Int_t       fIdSystem;         
    Double_t    fPedestal;        
    Double_t    fGain1;          
    Double_t    fGain2;     

    ClassDef(FairDbExPar1,0); // FairDbExPar1: Example of non composite data type.
};

#endif /* !FAIRDBEXPAR1_H*/
