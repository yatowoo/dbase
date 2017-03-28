#ifndef CBMSTSWTQADATAOBJECTS_H
#define CBMSTSWTQADATAOBJECTS_H

#include <map>
#include <tuple>
#include <iostream>

#include "FairDbObjTableMap.h"
#include "FairDbConnectionPool.h"
#include "FairDbConnection.h"
#include "FairDbStatement.h"
#include "TSQLServer.h"
#include "TSQLStatement.h"
#include "FairDbTableInterfaceStore.h"
#include "FairRunIdGenerator.h"
#include "FairDbValRecord.h"
#include "ValTimeStamp.h"
#include "ValInterval.h"

#include "FairDbParFactory.h"
#include "FairDbWtParFactory.h"

#include "CbmStsDbQa.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaUtils.h"

#include <map>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cstdint>

namespace Database
{

  const int NVENDOR = 2; 

  UNSIGNED_ENUM_CLASS(Vendor, CIS = 0, HAM = 1);

  typedef map<unsigned int, string>    VendorName;
  typedef tuple <int, int, int>        IdTuple;
  typedef multimap<string, IdTuple>    BatchIdMap;
  
  class CbmStsWtQaDataObjects 
  {
    
  public:   
    static  CbmStsWtQaDataObjects& instance();
    static  Bool_t                 isActive()    { return fgInstance ? kTRUE: kFALSE; }
    const BatchIdMap&              getBatchIdMap(int vendor_id) {return fIdMap[vendor_id];}
    const string&                  getVendorName(int vendor_id) {return fVendor_name[vendor_id];}
    const CbmStsDbQa&              getStsQa();  
    int                            getNVendors() { return NVENDOR;}
    void                           initialize();
    void                           rearrange();
    const vector<IdTuple>          getBatchIdTuples(const string& vendor, const string& bacth_id);
    int                            getVendorFromName(const string& name);
    
  protected:
    CbmStsDbQa*              fStsQa; 
    FairDbConnectionPool*    fConnections;
    BatchIdMap               fIdMap[NVENDOR];
    VendorName               fVendor_name;
    
  private:
             CbmStsWtQaDataObjects();
    virtual ~CbmStsWtQaDataObjects();
    
    static   CbmStsWtQaDataObjects*  fgInstance;
  };
  
}

#endif
