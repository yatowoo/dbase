#ifndef CBMSTSWTQAPARFACTORY_H
#define CBMSTSWTQAPARFACTORY_H

#include "FairDbParFactory.h"             

#include <WContainerWidget>
#include <WTable>
#include <WString>

#include <map>
#include <iostream>

#include "CbmStsDbQaPar.h"
#include "CbmStsDbQaSensorPar.h"
#include "CbmStsDbQaIvPar.h"

using namespace std;
using namespace Wt;

class CbmStsWtQaParFactory
{

 struct dateInfo{
  string  t_creation;
  string  t_modified;
  string  t_end;
 
     dateInfo( const std::string& tcreation = "",
               const std::string& tmodified = "",
               const std::string& tend      = ""):
                t_creation(tcreation), 
                t_modified(tmodified),
                t_end(tend) {}
 };

 typedef map<string, dateInfo> ValDates;
 typedef FairDbReader<CbmStsDbQaSensorPar> ReaderSensor;
 typedef FairDbReader<CbmStsDbQaIvPar>     ReaderIv;


  public:
     static  CbmStsWtQaParFactory& Instance();
     static  Bool_t IsActive() { return fgInstance ? kTRUE: kFALSE; }
    
    // Main Init for all Qa Parameters.
    void InitQaContainer(int rid=0);

    template < typename T > 
    void Register(FairDbReader<T>* p_reader); 

    template < typename T >
    shared_ptr<T> GetParPtr(shared_ptr<FairDbParSet> par)
    {return dynamic_pointer}  
   

    // Create all WtTable for all Qa Parameters
    void CreateWTables(WContainerWidget* aParent);

    // Create a specific WtTable for a Parameter pName
    WTable* CreateTable(WContainerWidget* aParent,string pName); 

     
    
  private:
    CbmStsWtQaParFactory();
    virtual ~CbmStsWtQaParFactory();

    ValDates  fDatesInfo;
    map<string , FairDbParSet*> fParObjects;
    map<string , WTable*> fWTableObjects;

    ReaderSensor* fRSensor;
    ReaderIv*     fRIv; 

    CbmStsWtQaParFactory(const CbmStsWtQaParFactory&);
    CbmStsWtQaParFactory& operator=(const CbmStsWtQaParFactory&);

    static CbmStsWtQaParFactory* fgInstance;

};

#endif
