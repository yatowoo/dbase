#ifndef CBMSTSDBSUID_H
#define CBMSTSDBSUID_H

#include "TObject.h"
#include "Rtypes.h"
#include "FairDbConnectionPool.h"

class CbmStsDbSUID : public TObject
{

  public:
    static CbmStsDbSUID& Instance();
    static       Bool_t IsActive() { return fgInstance ? kTRUE: kFALSE; }

  protected:
    CbmStsDbSUID();
    virtual ~CbmStsDbSUID();

    Int_t Init(Int_t dbNo);

  private:
    CbmStsDbSUID(const CbmStsDbSUID&);
    CbmStsDbSUID& operator=(const CbmStsDbSUID&);

    static CbmStsDbSUID* fgInstance;

    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    
    ClassDef(CbmStsDbSUID,0)   // Singleton storing Sensor UID

};


#endif  // CBMSTSDBSUID
