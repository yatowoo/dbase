#ifndef FAIRDBWTPARFACTORY_H
#define FAIRDBWTPARFACTORY_H

#include "FairDbParFactory.h"             
#include "FairRunIdGenerator.h"
#include "FairDbValRecord.h"
#include "ValInterval.h"

#include "FairDbReader.tpl"
#include "FairDbWriter.tpl"

#include <WContainerWidget>
#include <WTable>
#include <WString>

#include <map>
#include <iostream>
#include <boost/any.hpp>

using namespace std;
using boost::any_cast;
using namespace Wt;


typedef std::map<std::string, boost::any> anyStore;


class FairDbWtParFactory
{


 template <typename T>  
          using mStore = std::map<string,T>;
 template <typename T> 
          using Reader = FairDbReader<T>;
 template <typename T> 
          using Writer = FairDbWriter<T>;


  public:

 struct parInfo{
  string  fCreation;
  string  fModified;
  string  fEnd;
  FairDbTableInterface* fTableInterface;
 
     parInfo(  const std::string& tcreation = "",
               const std::string& tmodified = "",
               const std::string& tend      = "",
               FairDbTableInterface* tinterface=NULL):
                fCreation(tcreation), 
                fModified(tmodified),
                fEnd(tend),
                fTableInterface(tinterface) {}
 };



     static  FairDbWtParFactory& Instance();
     static  Bool_t IsActive() { return fgInstance ? kTRUE: kFALSE; }
    
    
    template <typename T>
    const string GetTableName()
    {
      string tname = FairDbReader<T>::GetTableInterface().GetTableName(); 
      std::transform(tname.begin(), tname.end(), tname.begin(), ::tolower);
      return (tname);
    }  



    // Main Init for all Qa Parameters.
    // ( templated version )
    template <typename T> 
    shared_ptr<T> InitContainer(int rid=0, string pName="", 
                                string logtitle="standard",
                                int dbEntry=0, int version=0, 
                                int comboNr = -1)
    {
     
     // <DB> if no name choosen, take default one
     if (pName == "") 
     {
      T a; 
      pName = a.GetName();
     }

     cout << "------- FairDbWtParFactory::InitContainer() : " << pName <<  endl; 
     auto pQaPar = FairDbParFactory::Instance().Create(pName);
 
     const std::type_info& r1 = typeid(*pQaPar);

     //cout << "---------- TYPEID " << r1.name() << endl;

     if (pQaPar == nullptr) 
     {
      cout << "-E- FairDbWtParFactory::initContainer(): can not create: " << pName << endl;
      return nullptr;
     }

     // Define a runId from process execution time
     FairRunIdGenerator runID;
     UInt_t runId =  runID.generateId();

     // Set Parameter Meta Data 
     pQaPar->SetVersion(version);
     pQaPar->SetComboNo(comboNr);
     pQaPar->SetDbEntry(dbEntry);
     pQaPar->SetLogTitle(logtitle);

     if (rid==0)
     {
           //Init at Date Now() !
           pQaPar->fill(runId); 
     }else{
           pQaPar->fill(rid); 
     }

    fParObjects[pName]=pQaPar;
   
    return GetParamPtr<T> (pQaPar);
    }


    // Main Init for all Qa Parameters.
    // ( standard version )
    shared_ptr<FairDbParSet> InitContainer(string pName, int rid=0, 
                                string logtitle="standard",
                                int dbEntry=0, int version=0, 
                                int comboNr = -1)
    {
     
     
     cout << "------- FairDbWtParFactory::InitContainer() : " << pName <<  endl; 
     auto pQaPar = FairDbParFactory::Instance().Create(pName);
 
     const std::type_info& r1 = typeid(*pQaPar);

     cout << "---------- TYPEID " << r1.name() << endl;

     if (pQaPar == nullptr) 
     {
      cout << "-E- FairDbWtParFactory::initContainer(): can not create: " << pName << endl;
      return nullptr;
     }

     // Define a runId from process execution time
     FairRunIdGenerator runID;
     UInt_t runId =  runID.generateId();

     // Set Parameter Meta Data 
     pQaPar->SetVersion(version);
     pQaPar->SetComboNo(comboNr);
     pQaPar->SetDbEntry(dbEntry);
     pQaPar->SetLogTitle(logtitle);

     if (rid==0)
     {
           //Init at Date Now() !
           pQaPar->fill(runId); 
     }else{
           pQaPar->fill(rid); 
     }

    fParObjects[pName]=pQaPar;
   

    return (pQaPar);
    }


    template <typename T>
    void RegisterReader(FairDbReader<T>* p_reader)
    {
     const string tname = GetTableName<T>(); 
     if(p_reader)
      {    
       const FairDbValRecord *p1_rec = p_reader->GetValidityRec();
       ValInterval val_p1 = p1_rec->GetValInterval();
       ValTimeStamp p1_cDate  = p1_rec->GetCreationDate();
       ValTimeStamp p1_update = val_p1.GetTimeStart();
       ValTimeStamp p1_end  = val_p1.GetTimeEnd(); 
       string a = p1_cDate.AsString("s");
       string b = p1_update.AsString("s");
       string c = p1_end.AsString("s");
        
        cout << " Name: " << tname  
             << " creation date: " << a << " modified: " 
             << b <<  " end: " << c << endl;
  
       fParInfo[tname] = parInfo(a,b,c,&p_reader->GetTableInterface()); 
       fDbReaders[tname] = p_reader;
      }      
    }


    template <typename T>
    shared_ptr<T> GetParamPtr(shared_ptr<FairDbParSet> par)
    {
      return (dynamic_pointer_cast<T> (par) );
    }  

    template <typename T>
    shared_ptr<T> GetParameter()
    {
      const string tname = GetTableName<T>(); 
      shared_ptr<FairDbParSet> par = fParObjects[tname];
      return (GetParamPtr<T> (par));
    }  




    template < typename T > 
    FairDbReader<T>* GetReader()
    {
      const string tname = GetTableName<T>(); 
      cout << "------- FairDbWtParFactory::GetReader() : "  <<  tname << endl;
      return any_cast<FairDbReader<T>*> (fDbReaders[tname]);
    } 

    template <typename T> 
    const string GetCreationDate(){
     const string tname = GetTableName<T>(); 
     return fParInfo[tname].fCreation;
    }

    template <typename T> 
    const string GetModifiedDate(){
     const string tname = GetTableName<T>(); 
     return fParInfo[tname].fModified;
    }


    template <typename T> 
    const string GetEndDate(){
     const string tname = GetTableName<T>(); 
     return fParInfo[tname].fEnd;
    }

  
    const parInfo& GetParInfo(const string& pName){
     return fParInfo[pName];

    }

    // Create all WtTable for all Qa Parameters
    void CreateWTables(WContainerWidget* aParent);
    // Create a specific WtTable for a Parameter pName
    WTable* CreateTable(WContainerWidget* aParent,string pName); 

     
    
  private:
    FairDbWtParFactory();
    virtual ~FairDbWtParFactory();

    mStore<parInfo>       fParInfo;
    mStore<shared_ptr<FairDbParSet>> fParObjects;
    mStore<WTable*>       fWTableObjects;
    anyStore              fDbReaders;
    anyStore              fDbWriters; 

    FairDbWtParFactory(const FairDbWtParFactory&);
    FairDbWtParFactory& operator=(const FairDbWtParFactory&);

    static FairDbWtParFactory* fgInstance;

};

#endif
