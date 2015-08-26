#ifndef FAIRDBWTPARTREETABLE_H
#define FAIRDBWTPARTREETABLE_H

#include <WTreeTable>



using namespace Wt;

class FairDbWtParTreeTableNode;

class FairDbWtParTreeTable : public Wt::WTreeTable
{
public:
    FairDbWtParTreeTable( Wt::WContainerWidget *parent = 0);
    virtual void createTableLayout(FairDbWtParTreeTableNode *node);
};

#endif // FAIRDBWTPARTREETABLE_H
