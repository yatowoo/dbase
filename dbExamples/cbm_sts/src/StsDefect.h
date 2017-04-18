#ifndef STSDEFECT_H
#define STSDEFECT_H

#include "StsOpticalInspection.h"
#include "StsDefectType.h"
#include "StsDefectContext.h"
#include "StsInspectionImage.h"

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsDefect : public FairDbGenericParSet<StsDefect>
{

using TObject::Compare;

  public :
    StsDefect(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsDefect", 
              const char* title = "Sts Defect Type Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsDefect(void);
    StsDefect(const StsDefect& from);
    StsDefect& operator=(const StsDefect& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsDefect* GetDefectById(Int_t defectId, UInt_t runId = 0);
    static TObjArray* GetDefectsByImageId(Int_t inspectionImageId, UInt_t runId = 0);
    static TObjArray* GetDefectsByInspectionId(Int_t inspection, UInt_t runId = 0);


    StsOpticalInspection* GetInspection() {
      if (!fInspection) fInspection = StsOpticalInspection::GetInspectionById(fInspectionId);
      return fInspection;
    }

    StsDefectType* GetType() {
      if (!fType) fType = StsDefectType::GetDefectTypeById(fTypeId);
      return fType;
    }

    StsDefectContext* GetContext() {
      if (!fContext) fContext = StsDefectContext::GetDefectContextById(fContextId);
      return fContext;
    }

    StsInspectionImage* GetImage() {
      if (!fImage) fImage = StsInspectionImage::GetInspectionImageById(fImageId);
      return fImage;
    }

    Int_t      GetId()                const { return fId; }
    Int_t      GetInspectionId()      const { return fInspectionId; }
    Int_t      GetTypeId()            const { return fTypeId; }
    Int_t      GetContextId()         const { return fContextId; }
    Int_t      GetAlgorithmId()       const { return fAlgorithmId; }
    Int_t      GetImageId()           const { return fImageId; }
    Double_t   GetLeft()              const { return fLeft; }
    Double_t   GetTop()               const { return fTop; }
    Double_t   GetWidth()             const { return fWidth; }
    Double_t   GetHeight()            const { return fHeight; }
    Double_t   GetCenterMassX()       const { return fCenterMassX; }
    Double_t   GetCenterMassY()       const { return fCenterMassY; }
    Double_t   GetArea()              const { return fArea; }
    Double_t   GetPerimeter()         const { return fPerimeter; }
    Double_t   GetHydraulicRadius()   const { return fHydraulicRadius; }
    Double_t   GetAreaRatio()         const { return fAreaRatio; }
    Double_t   GetElongation()        const { return fElongation; }
    Double_t   GetCompactness()       const { return fCompactness; }
    Double_t   GetCircularity()       const { return fCircularity; }
    string     GetComment()           const { return fComment; }
    

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetInspection(StsOpticalInspection& value) { fInspection = new StsOpticalInspection(value);
                                                      fInspectionId = value.GetId(); }
    void SetType(StsDefectType& value)              { fType = new StsDefectType(value);
                                                      fTypeId = value.GetId(); }
    void SetContext(StsDefectContext& value)        { fContext = new StsDefectContext(value);
                                                      fContextId = value.GetId(); }
    void SetImage(StsInspectionImage& value)        { fImage = new StsInspectionImage(value);
                                                      fImageId = value.GetId(); }

    void SetId(Int_t value)                 { fId = value; }
    void SetInspectionId(Int_t value)       { fInspectionId = value; }
    void SetTypeId(Int_t value)             { fTypeId = value; }
    void SetContextId(Int_t value)          { fContextId = value; }
    void SetAlgorithmId(Int_t value)        { fAlgorithmId = value; }
    void SetImageId(Int_t value)            { fImageId = value; }
    void SetLeft(Double_t value)            { fLeft = value; }
    void SetTop(Double_t value)             { fTop = value; }
    void SetWidth(Double_t value)           { fWidth = value; }
    void SetHeight(Double_t value)          { fHeight = value; }
    void SetCenterMassX(Double_t value)     { fCenterMassX = value; }
    void SetCenterMassY(Double_t value)     { fCenterMassY = value; }
    void SetArea(Double_t value)            { fArea = value; }
    void SetPerimeter(Double_t value)       { fPerimeter = value; }
    void SetHydraulicRadius(Double_t value) { fHydraulicRadius = value; }
    void SetAreaRatio(Double_t value)       { fAreaRatio = value; }
    void SetElongation(Double_t value)      { fElongation = value; }
    void SetCompactness(Double_t value)     { fCompactness = value; }
    void SetCircularity(Double_t value)     { fCircularity = value; }
    void SetComment(string value)           { fComment = value; }



    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    StsOpticalInspection* fInspection; //! Transient data member
    StsDefectType*        fType;       //! Transient data member
    StsDefectContext*     fContext;    //! Transient data member
    StsInspectionImage*   fImage;      //! Transient data member

    Int_t                 fId;
    Int_t                 fInspectionId;
    Int_t                 fTypeId;
    Int_t                 fContextId;
    Int_t                 fAlgorithmId;
    Int_t                 fImageId;
    Double_t              fLeft;
    Double_t              fTop;
    Double_t              fWidth;
    Double_t              fHeight;
    Double_t              fCenterMassX;
    Double_t              fCenterMassY;
    Double_t              fArea;
    Double_t              fPerimeter;
    Double_t              fHydraulicRadius;
    Double_t              fAreaRatio;
    Double_t              fElongation;
    Double_t              fCompactness;
    Double_t              fCircularity;
    string                fComment;

    ClassDef(StsDefect,1); // ROOT class definition
};

#endif /* !STSDEFECT_H*/

