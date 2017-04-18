#ifndef STSOPTICALINSPECTION_H
#define STSOPTICALINSPECTION_H

#include "StsSensor.h"

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TGraph2D.h"
#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc
#include "TObjArray.h"


#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsOpticalInspection : public FairDbGenericParSet<StsOpticalInspection>
{

using TObject::Compare;

  public :
    StsOpticalInspection(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsOpticalInspection", 
              const char* title = "Sts Sensor Optical Inspection Entity", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsOpticalInspection(void);
    StsOpticalInspection(const StsOpticalInspection& from);
    StsOpticalInspection& operator=(const StsOpticalInspection& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    StsSensor* GetSensor();
    TObjArray* GetInspectionImages();
    TObjArray* GetDefects();

    static TObjArray* GetInspections(Int_t sensorId, UInt_t runId = 0);


    Int_t GetId()            const { return fId; }
    Int_t GetSensorId()      const { return fSensorId; }
    Int_t GetSizeX()         const { return fSizeX; }
    Int_t GetSizeY()         const { return fSizeY; }
    ValTimeStamp GetDate()   const { return fDate; }
    string GetSensorSide()   const { return fSensorSide; }
    Double_t GetMaxWarp()    const { return fMaxWarp; }
    TGraph2D* GetWarp()       const { return fWarp; }
    Double_t GetThickness()  const { return fThickness; }
    Double_t GetQuality()    const { return fQuality; }
    string GetQualityGrade() const { return fQualityGrade; }
    Int_t GetPassed()        const { return fPassed; }
    string GetComment()      const { return fComment; } 


    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetSensor(StsSensor &value)   { fSensor = new StsSensor(value); SetSensorId(value.GetId()); }

    void SetId(Int_t value)            { fId = value; }
    void SetSensorId(Int_t value)      { fSensorId = value; }
    void SetSizeX(Int_t value)         { fSizeX = value; }
    void SetSizeY(Int_t value)         { fSizeY = value; }
    void SetDate(ValTimeStamp value)   { fDate = value; }
    void SetSensorSide(string value)   { fSensorSide = value; }
    void SetMaxWarp(Double_t value)    { fMaxWarp = value; }
    void SetWarp(TGraph2D* value)      { if (fWarp) delete fWarp; fWarp = value ? new TGraph2D(*value) : NULL; }
    void SetThickness(Double_t value)  { fThickness = value; }
    void SetQuality(Double_t value)    { fQuality = value; } 
    void SetQualityGrade(string value) { fQualityGrade = value; }
    void SetPassed(Int_t value)        { fPassed = value; }
    void SetComment(string value)      { fComment = value; }


    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    StsSensor *fSensor;           //! Transient data member
    TObjArray *fInspectionImages; //! Transient data member
    TObjArray *fDefects;          //! Transient data member

    Int_t fId;
    Int_t fSensorId;
    Int_t fSizeX;
    Int_t fSizeY;
    ValTimeStamp fDate;
    string fSensorSide;
    Double_t fMaxWarp;
    TGraph2D* fWarp;
    Double_t fThickness;
    Double_t fQuality;
    string fQualityGrade;
    Int_t fPassed;
    string fComment;

    ClassDef(StsOpticalInspection,1); // ROOT class definition
};

#endif /* !STSOPTICALINSPECTION_H*/

