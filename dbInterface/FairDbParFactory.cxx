#include "FairDbParFactory.h"

#include <string.h>                     // for strcmp, strlen
#include <cstdlib>                      // for abort, NULL
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <vector>                       // for vector

using namespace std;

FairDbParFactory* FairDbParFactory::fgInstance  = 0;


FairDbParFactory::FairDbParFactory():
fConnectionPool(FairDbTableInterfaceStore::Instance().fConnectionPool)
{
  cout  << "-I- FairDbParFactory created " << endl;
}

FairDbParFactory::~FairDbParFactory()
{ 
  cout  << "-I- ~ Destroying ~ FairDbParFactory" << endl;

 if (fConnectionPool) {
     delete fConnectionPool;
            fConnectionPool = 0;
  }

  FairDbParFactory::fgInstance = 0;

}



FairDbParFactory& FairDbParFactory::Instance()
{
  if ( ! fgInstance ) {
         fgInstance = new FairDbParFactory();
  }

  return *fgInstance;
}




void FairDbParFactory::RegisterFactoryFunction(string name, 
  function<FairDbParSet*(void)> parFactoryFunction)
{
    // register the class factory function 
    fFactoryFunctionRegistry[name] = parFactoryFunction;
}


shared_ptr<FairDbParSet> FairDbParFactory::Create(string name)
{
    shared_ptr<FairDbParSet> shr_param = GetParam(name);
    
    if(shr_param)
    {
         return shr_param; 
    }else{

    FairDbParSet * instance = nullptr;
    // find name in the registry and call factory method.
    auto it = fFactoryFunctionRegistry.find(name);

    if(it != fFactoryFunctionRegistry.end())
         instance = it->second();
    
      if(instance != nullptr){
         shared_ptr<FairDbParSet> shr_instance(instance); 
         InsertParam(name, shr_instance);          
         return shr_instance; 
      }else{ 
        return nullptr;
      }
    }
}
