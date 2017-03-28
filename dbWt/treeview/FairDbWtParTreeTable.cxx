#include "FairDbWtParTreeTable.h"
#include "FairDbWtParTreeTableNode.h"

#include <Wt/WText>

using namespace Wt;

FairDbWtParTreeTable::FairDbWtParTreeTable(WContainerWidget *parent)
  : WTreeTable(parent)
{
 
}

FairDbWtParTreeTable::~FairDbWtParTreeTable()
{
cout << "~FairDbWtParTreeTable " << endl;
}


void FairDbWtParTreeTable::createTableLayout(FairDbWtParTreeTableNode *pNode)
 {
 
 // <DB> tree of parameter objects 
 // should be as light as possible  
 // addColumn("Last Modified", 150);
 // header(1)->setStyleClass("date");
 
   // Create the tree  
   pNode->createTree();
   setTreeRoot(pNode, "FairDb Contents");

   treeRoot()->setImagePack("icons/");
   treeRoot()->expand();
   resize(300,300);
}
