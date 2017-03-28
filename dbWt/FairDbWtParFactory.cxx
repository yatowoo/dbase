#include "FairDbWtParFactory.h"



using namespace std;


FairDbWtParFactory* FairDbWtParFactory::fgInstance  = 0;


FairDbWtParFactory::FairDbWtParFactory()
{
  cout  << " -I- ~ Creating ~ FairDbWtParFactory" << endl;
}


FairDbWtParFactory& FairDbWtParFactory::Instance()
{
  if ( ! fgInstance ) {
    fgInstance = new FairDbWtParFactory();
  }

  return *fgInstance;
}

FairDbWtParFactory::~FairDbWtParFactory()
{ 
  cout  << " -I- ~ Destroying ~ FairDbWtParFactory" << endl;

}



void FairDbWtParFactory::CreateWTables(WContainerWidget* aParent)
{
 // Create WTable and register them in the map

}

WTable* FairDbWtParFactory::CreateTable(WContainerWidget* aParent,string pName)
{
 // Create Specific WTable for dedicated container

 return nullptr; 
}





