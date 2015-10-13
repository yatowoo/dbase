#ifndef CBMSTSQAWPARTREETABLENODE_H
#define CBMSTSQAWPARTREETABLENODE_H

#include "FairDbWtParTreeTableNode.h"


class CbmStsQaWParTreeTableNode : public FairDbWtParTreeTableNode 
{
public:
  CbmStsQaWParTreeTableNode(FairDbObjTableMap* aPar,
                           WString& pName, bool isDir);

 virtual ~CbmStsQaWParTreeTableNode();

 virtual WTable* createDataTable(WContainerWidget* aParent,string pName); 
// virtual WTableView* createDataTableView(WContainerWidget* aParent,string pName); 
 WContainerWidget* createDataTableView(WContainerWidget* aParent,string pName); 
 virtual WTable* createValTable(WContainerWidget* aParent,string pName); 
 virtual WTable* createMetaTable(WContainerWidget* aParent,string pName); 
 virtual WTable* createConnTable(WContainerWidget* aParent,string pName); 

 virtual void fillTableRows(WTable* table,string pName); 

 virtual void createTree();
 virtual void initParameter(string pName); 


 private:
     WMessageBox *fPopupActionBox;

};

#endif // CBMSTSQAWPARTREETABLENODE_H
