#ifndef CBMSTSQAWPARTREETABLE_H
#define CBMSTSQAWPARTREETABLE_H

#include "FairDbWtParTreeTable.h"
#include "FairDbWtParTreeTableNode.h"


class CbmStsQaWParTreeTableNode;

class CbmStsQaWParTreeTable : public FairDbWtParTreeTable
{
public:
    CbmStsQaWParTreeTable( Wt::WContainerWidget *parent = 0);
    virtual ~CbmStsQaWParTreeTable();
    CbmStsQaWParTreeTableNode* getTableNode(){return fTableNode;}   

protected:
CbmStsQaWParTreeTableNode * fTableNode;
     
};

#endif // CBMSTSQAWPARTREETABLE_H
