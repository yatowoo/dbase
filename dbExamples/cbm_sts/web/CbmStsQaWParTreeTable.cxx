#include "CbmStsQaWParTreeTable.h"
#include "CbmStsQaWParTreeTableNode.h"

#include <WText>

using namespace Wt;

CbmStsQaWParTreeTable::CbmStsQaWParTreeTable(WContainerWidget *parent)
  : FairDbWtParTreeTable(parent)
{
  cout << "------ STS QA CbmStsQaWParTreeTable initialise called !.... " << endl;
  
  WString topName="ParamList";
  fTableNode = new CbmStsQaWParTreeTableNode(0,topName,1);
  
  // Create Layout for TreeTables
  createTableLayout( fTableNode);
 
 }

 CbmStsQaWParTreeTable::~CbmStsQaWParTreeTable()
 {
    cout << "~CbmStsQaParTreeTable" << endl;
 }

 