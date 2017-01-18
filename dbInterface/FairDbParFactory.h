#ifndef FAIRDBPARFACTORY_H
#define FAIRDBPARFACTORY_H

#include "FairDbParSet.h"               // for FairDbParSet
#include "FairDbConnectionPool.h"
#include "FairDbTableInterface.h"
#include "FairDbTableInterfaceStore.h"
#include "FairDbLogService.h"


#include <memory>
#include <functional>
#include <map>                          // for map
#include <string>                       // for string
#include <algorithm>
#include <iostream>

using namespace std;



class FairDbParFactory
{

  public:
     static  FairDbParFactory& Instance();
     static  Bool_t IsActive() { return fgInstance ? kTRUE: kFALSE; }
     FairDbConnectionPool* GetConnections() { return fConnectionPool; }

    
    void RegisterFactoryFunction(string name, 
      function<FairDbParSet*(void)> parFactoryFunction);

    // create an instance of a registered class
    shared_ptr<FairDbParSet> Create(string name);

    void InsertParam(string handle, shared_ptr<FairDbParSet> obj){
          fParMap[handle] = obj;    
        }
    shared_ptr<FairDbParSet> GetParam(string handle){
            return fParMap[handle];
        }


  private:
     FairDbParFactory();
     virtual ~FairDbParFactory();

  #ifndef __CINT__  
     map<string, function<FairDbParSet*(void)>> fFactoryFunctionRegistry;
     map<string, shared_ptr<FairDbParSet>> fParMap; 
  #endif

     FairDbConnectionPool* fConnectionPool;
     FairDbParFactory(const FairDbParFactory&);
     FairDbParFactory& operator=(const FairDbParFactory&);
     static FairDbParFactory* fgInstance;

};


template<class T>
class FairDbParRegistry {
public:
  FairDbParRegistry(const char* parName)
    {
      // register the class factory function 
      FairDbParFactory::Instance().RegisterFactoryFunction(parName,
                                                           [parName](void) -> T* 
                                                           { 
                                                             string pName(parName);
                                                             std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
                                                             const char * pname = pName.c_str();
                                                             DBLOG("FairDb",FairDbLog::kInfo) << "FairDbParRegistry: Lambda functor created parameter -> "
                                                                                              << pName << endl;
                                                               return (new T(pname,pname,pname,kTRUE));
                                                           }
                                                           );
    }
};

template<class T>
class FairDbGenericParRegistry {
public:
  FairDbGenericParRegistry(const char* parName,
                           const FairDbDetector::Detector_t detid,
                           const DataType::DataType_t dataid)
    {
      // register the class factory function 
      FairDbParFactory::Instance().RegisterFactoryFunction(parName,
                                                           [parName, detid, dataid](void) -> T* 
                                                           { 
                                                             string pName(parName);
                                                             std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
                                                             const char * pname = pName.c_str();
                                                             DBLOG("FairDb",FairDbLog::kInfo) << "FairDbGenericParRegistry: Lambda functor created parameter -> "
                                                                  << pName << endl;
                                                             return (new T(detid,dataid,pname,pname,pname,kTRUE));
                                                           }
                                                           );
    }
};


#endif  // FAIRDBPARFACTORY_H

