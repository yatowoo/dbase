#include "CbmStsQaWParFactory.h"
#include "FairRunIdGenerator.h"
#include "FairDbValrecord.h"
#include "ValInterval.h"


using namespace std;

CbmStsQaWParFactory* CbmStsQaWParFactory::fgInstance  = 0;


CbmStsQaWParFactory::CbmStsQaWParFactory()
{
  cout  << " -I- ~ Creating ~ CbmStsQaWParFactory" << endl;
}


CbmStsQaWParFactory& CbmStsQaWParFactory::Instance()
{
  if ( ! fgInstance ) {
         fgInstance = new CbmStsQaWParFactory();
  }

  return *fgInstance;
}

CbmStsQaWParFactory::~CbmStsQaWParFactory()
{ 
  cout  << " -I- ~ Destroying ~ CbmStsQaWParFactory" << endl;

}


void CbmStsQaWParFactory::InitQaContainer(int rid)
{

 cout << "------- CbmStsQaWParInitQaContainer   ------ " << endl;

 auto pQaPar = FairDbParFactory::Instance().Create("CbmStsDbQaPar");
 

 if (pQaPar == nullptr) {
   cout << "-E- CbmStsQaWParFactory::initQaContainer(): can not create CbmQaPar Class ! " << endl;
   return;
 }

 // Define a runId from process execution time
 FairRunIdGenerator runID;
 UInt_t runId =  runID.generateId();

 if (rid==0){
       //Init at Date Now() !
       pQaPar->SetVersion(0);
       pQaPar->SetComboNo(-1);
       pQaPar->SetDbEntry(0);
       pQaPar->SetLogTitle("CbmStsDbQaPar");

       pQaPar->fill(runId); 
 } else
  {
       pQaPar->SetVersion(0);
       pQaPar->SetComboNo(-1);
       pQaPar->SetDbEntry(0);
       pQaPar->SetLogTitle("CbmStsDbQaPar");
       pQaPar->fill(rid); 
  }

 // Needed touse function template 
 shared_ptr<CbmStsDbQaPar> qaPar = dynamic_pointer_cast<CbmStsDbQaPar> (pQaPar);
 
  if(qaPar) {
   fRSensor = qaPar->GetReaderSensor();
   Register<CbmStsDbQaSensorPar>(fRSensor);
  }

}


template <typename T>
void CbmStsQaWParFactory::Register(FairDbReader<T>* p_reader)
{ 
 
  string tname = FairDbReader<T>::GetTableInterface().GetTableName(); 
  std::transform(tname.begin(), tname.end(), tname.begin(), ::tolower);
  
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
     } 

}



void CbmStsQaWParFactory::CreateWTables(WContainerWidget* aParent)
{
 // Create WTable and register them in the map

}

WTable* CbmStsQaWParFactory::CreateTable(WContainerWidget* aParent,string pName)
{
 // Create Specific WTable for dedicated container

 return nullptr; 
}



