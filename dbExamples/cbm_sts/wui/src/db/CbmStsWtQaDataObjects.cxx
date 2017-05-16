#include "CbmStsWtQaDataObjects.h"


namespace Database
{
  
  CbmStsWtQaDataObjects* CbmStsWtQaDataObjects::fgInstance  = 0;
  
  CbmStsWtQaDataObjects& CbmStsWtQaDataObjects::instance()
  {
    if ( ! fgInstance ) {
      fgInstance = new CbmStsWtQaDataObjects();
    }
    return *fgInstance;
  }

  CbmStsWtQaDataObjects::CbmStsWtQaDataObjects()
  {
    QA_LOG(DB,DEBUG) << "CbmStsWtQaDataObjects::ctor()";

    // Get the first connected DB instance    
    fConnections = FairDbTableInterfaceStore::Instance().fConnectionPool;  

    // Init Vendor Map 
    fVendor_name[+Vendor::CIS] = "CiS";
    fVendor_name[+Vendor::HAM] = "Hamamatsu";

}

  CbmStsWtQaDataObjects::~CbmStsWtQaDataObjects()
  {
    QA_LOG(DB,DEBUG) << "CbmStsWtQaDataObjects::dtor()"; 
  }

  void
  CbmStsWtQaDataObjects::initialize()
  {
    // Initialise the relevant parameter
    auto_ptr<FairDbStatement> stmtDb(fConnections->CreateStatement(0));
    if ( ! stmtDb.get() ) {
      QA_LOG(DB,ERROR) << "Cannot get a statement for cascade entry 0.  \n"
                       << "Please check the ENV_TSQL_* environment.  Quitting ... ";
    }
    
    fStsQa = FairDbWtParFactory::Instance().InitContainer<CbmStsDbQa>().get();
    
    if(fStsQa)
      {
        FairDbReader<CbmStsDbQaSensor>* r_Sensor = fStsQa->GetReaderSensor();
        FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaSensor>(r_Sensor);
      }
    
    QA_LOG(DB,INFO) <<"CbmStsDbQa successfully initialised ...";

    // Rearrange the data in internal buffers
    rearrange();    
  }

  const CbmStsDbQa&
  CbmStsWtQaDataObjects::getStsQa()
  {
    if (fStsQa) {
      return *(fStsQa);
    }else
      {
        QA_LOG(DB,FATAL) << "CbmStsWtQaDataObjects::getStsQa() \
                            no CbmStsDbQa intialized ... quitting !";
        exit(0);
      }
  }
  
  void
  CbmStsWtQaDataObjects::rearrange()
  {
    
    // Rearrange Qa parameter data in batch_id subsets
    if (fStsQa)
      {
        QA_LOG(DB,DEBUG) << "CbmStsWtDataObjects n_suid: "
                               << fStsQa->GetNumSUID() << " n_sensors: "
                               << fStsQa->GetNumSensors(); 
        
        // Fetch all SUID object
        int n_suid = fStsQa->GetNumSUID();
        for(int i=0; i<n_suid; i++){
          CbmStsDbQaSUID* a_suid = fStsQa->GetSUIDAt(i);
          if (a_suid) {        
            string vendor_id  = a_suid->GetVendor();
            string batch_id   = a_suid->GetBatchId();
            int    wafer_id   = a_suid->GetWaferId();
            int    sensor_id  = a_suid->GetSensorId();
            int    comp_id    = a_suid->GetCompId();
            std::ostringstream str_id;
            str_id << vendor_id << "_" << batch_id; 
            string prod_id = str_id.str();
            
            // Change after with proper getVendorIndex function 
            if (vendor_id.compare(fVendor_name[+Vendor::CIS]) == 0){
              fIdMap[+Vendor::CIS].insert(pair<string, IdTuple>(batch_id,IdTuple(wafer_id, sensor_id, comp_id)));        
            }else if (vendor_id.compare(fVendor_name[+Vendor::HAM]) == 0){
              fIdMap[+Vendor::HAM].insert(pair<string, IdTuple>(batch_id,IdTuple(wafer_id, sensor_id, comp_id)));        
            }
          }
        }     
      }
  }

  int
  CbmStsWtQaDataObjects::getVendorFromName(const string& name)
  {
    if (name.compare(fVendor_name[+Vendor::CIS]) == 0 )
      {
        return static_cast<int> (+Vendor::CIS);
      }
    else if (name.compare(fVendor_name[+Vendor::HAM]) == 0 )
      {
        return static_cast<int> (+Vendor::HAM);
      }
    return -1;
  }
  
  const vector<IdTuple>
  CbmStsWtQaDataObjects::getBatchIdTuples(const string& vendor, const string& batch_id)
  {  
    // Giving a vendor_id and batch_id generate the relevant
    // list as vector of tuples
    
    std::vector<IdTuple> values;

    BatchIdMap::size_type countPerKey;
    int vendor_id = getVendorFromName(vendor); 

    if (vendor_id < 0 ) {
      QA_LOG(DB,WARNING) << "CbmStsWtDataObjects::getBatchIdTuples  no vendor_id found, returning empty vector";
      return values;
    }
    // Count the number of values associated to the given batch_id.
    countPerKey = fIdMap[vendor_id].count(batch_id);
    QA_LOG(DB,WARNING) << "CbmStsWtQaDataObjects::getBatchIdTuples  batch_id: "
      << vendor_id << " contains: " << countPerKey << " tuples";
    
    // Bounds of a range that includes all the elements in the container which have a key  
    auto range = fIdMap[vendor_id].equal_range(batch_id);
    // Loop on subset and create the vector
    for(auto i = range.first; i != range.second; ++i){
      values.push_back(i->second);
      IdTuple val = i->second;
      // cout << " selected   tuple: " <<  std::get<0>(val) 
      //     << " : "    << std::get<1>(val)     << " : " << std::get<2>(val)  <<endl;     
    }//!(for)
    
    return values;     
  }

}

