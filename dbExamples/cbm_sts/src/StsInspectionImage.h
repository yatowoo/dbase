#ifndef STSINSPECTIONIMAGE_H
#define STSINSPECTIONIMAGE_H

#include "StsOpticalInspection.h"

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsInspectionImage : public FairDbGenericParSet<StsInspectionImage>
{

using TObject::Compare;

  public :
    StsInspectionImage(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsInspectionImage", 
              const char* title = "Sts Defect Type Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsInspectionImage(void);
    StsInspectionImage(const StsInspectionImage& from);
    StsInspectionImage& operator=(const StsInspectionImage& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsInspectionImage* GetInspectionImageById(Int_t inspectionImageId, UInt_t runId = 0);
    static StsInspectionImage* GetInspectionImage(Int_t inspectionId, Int_t X, Int_t Y, UInt_t runId = 0);
    static TObjArray* GetInspectionImages(Int_t inspectionId, UInt_t runId = 0);


    StsOpticalInspection* GetInspection();
    TObjArray*            GetDefects();

    Int_t    GetId()                    const { return fId; }
    Int_t    GetInspectionId()          const { return fInspectionId; }
    Int_t    GetX()                     const { return fX; }
    Int_t    GetY()                     const { return fY; }
    string   GetImageURI()              const { return fImageURI; }
    string   GetThumbnailURI()          const { return fThumbnailURI; }
    string   GetOverlayURI()            const { return fOverlayURI; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetInspection(StsOpticalInspection &value)   { fInspection = new StsOpticalInspection(value); 
                                                        SetInspectionId(value.GetId()); }

    void SetId(Int_t  value)                         { fId = value; }
    void SetInspectionId(Int_t  value)               { fInspectionId = value; }
    void SetX(Int_t  value)                          { fX = value; }
    void SetY(Int_t  value)                          { fY = value; }
    void SetImageURI(string value)                   { fImageURI = value; }
    void SetThumbnailURI(string value)               { fThumbnailURI = value; }
    void SetOverlayURI(string value)                 { fOverlayURI = value; }

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    StsOpticalInspection* fInspection; //! Transient data member
    TObjArray*            fDefects;    //! Transient data member, might be NULL or empty in case of no defects

    Int_t                 fId;
    Int_t                 fInspectionId;
    Int_t                 fX;
    Int_t                 fY;
    string                fImageURI;
    string                fThumbnailURI;
    string                fOverlayURI;

    ClassDef(StsInspectionImage,1); // ROOT class definition
};

#endif /* !STSINSPECTIONIMAGE_H*/

